#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "../src/common/bitvec.hpp"
#include "../src/common/rng.hpp"
#include "../src/omega/omega.hpp"
#include "../src/sdr_baseline/sdr.hpp"

using namespace omega;

namespace {

constexpr unsigned kItems = 200;
constexpr unsigned kChannels = 3;
constexpr unsigned kFSteps = 10;
constexpr unsigned kCalTrials = 20000;
constexpr unsigned kTrials = 100000;
constexpr unsigned kNSym = 40;
constexpr unsigned kTimeN = 10000;
constexpr uint64_t kMasterSeed = 0x51D5EEDBULL;
constexpr uint64_t kSaltCal = 0xCA1B02A5ull;
constexpr uint64_t kSaltSweep = 0x5EEBAA11ull;
constexpr unsigned kOmegaPooledThr[3] = {16, 24, 32};
constexpr unsigned kLegacyThr = 24;
constexpr unsigned kSdrThr[3] = {13, 16, 21};
const char* kChName[kChannels] = {"erasure", "mixed50", "flip"};

struct Counts {
    uint64_t tp = 0;
    uint64_t fp = 0;
    uint64_t fn = 0;
};

double f1_of(const Counts& c) {
    double denom = static_cast<double>(2 * c.tp + c.fp + c.fn);
    return 2.0 * static_cast<double>(c.tp) / denom;
}

uint64_t point_seed(unsigned ch, unsigned fstep, uint64_t salt) {
    return mix_streams((static_cast<uint64_t>(ch) << 32) ^
                           static_cast<uint64_t>(fstep),
                       kMasterSeed ^ salt);
}

void extract_support(const Code& c, unsigned* idx, int* sgn) {
    unsigned n = 0;
    for (unsigned w = 0; w < N_WORDS; ++w) {
        uint64_t v = c.pos.data()[w];
        while (v != 0ull) {
            idx[n] = (w << 6) | static_cast<unsigned>(__builtin_ctzll(v));
            sgn[n] = 1;
            ++n;
            v &= v - 1ull;
        }
        v = c.neg.data()[w];
        while (v != 0ull) {
            idx[n] = (w << 6) | static_cast<unsigned>(__builtin_ctzll(v));
            sgn[n] = -1;
            ++n;
            v &= v - 1ull;
        }
    }
    assert(n == kNSym);
}

// channels: 0 erasure (symbol->0 w.p. f), 1 mixed50 (->0 w.p. f/2, ->opposite
// w.p. f/2), 2 flip (->opposite w.p. f). u[i] in [0,200): f pct = 2*fpct/200.
void corrupt_code(const Code& src, Code& q, int ch, unsigned fpct,
                  const unsigned* u) {
    q = src;
    unsigned idx[kNSym];
    int sgn[kNSym];
    extract_support(src, idx, sgn);
    for (unsigned i = 0; i < kNSym; ++i) {
        bool del = false;
        bool flip = false;
        if (ch == 0) {
            del = u[i] < 2u * fpct;
        } else if (ch == 1) {
            del = u[i] < fpct;
            flip = u[i] >= fpct && u[i] < 2u * fpct;
        } else {
            flip = u[i] < 2u * fpct;
        }
        if (del) {
            if (sgn[i] > 0) {
                q.pos.clear(idx[i]);
            } else {
                q.neg.clear(idx[i]);
            }
        } else if (flip) {
            if (sgn[i] > 0) {
                q.pos.clear(idx[i]);
                q.neg.set(idx[i]);
            } else {
                q.neg.clear(idx[i]);
                q.pos.set(idx[i]);
            }
        }
    }
}

// SDR analogs: erasure/mixed50 -> support bit cleared w.p. f (SPEC); flip ->
// support bit cleared w.p. f and one random clear bit set (weight-preserving
// relocation of the flip).
void corrupt_sdr(const BitVec& src, BitVec& q, int ch, unsigned fpct,
                 const unsigned* u, Rng& rng) {
    (void)ch;
    q = src;
    unsigned sup[SDR::W];
    unsigned n = 0;
    for (unsigned w = 0; w < (SDR::N >> 6); ++w) {
        uint64_t v = src.data()[w];
        while (v != 0ull) {
            sup[n++] = (w << 6) | static_cast<unsigned>(__builtin_ctzll(v));
            v &= v - 1ull;
        }
    }
    assert(n == SDR::W);
    unsigned cleared[SDR::W];
    unsigned nc = 0;
    for (unsigned i = 0; i < SDR::W; ++i) {
        if (u[i] < 2u * fpct && q.test(sup[i])) {
            q.clear(sup[i]);
            cleared[nc++] = sup[i];
        }
    }
    if (ch == 2) {
        for (unsigned i = 0; i < nc; ++i) {
            unsigned p = cleared[i];
            unsigned b = rng.fast_below(SDR::N);
            while (b == p || q.test(b)) b = rng.fast_below(SDR::N);
            q.set(b);
        }
    }
}

void run_sdr(const std::vector<BitVec>& store, int ch, unsigned fpct,
             unsigned thr, uint64_t trials, uint64_t seed, Counts& out) {
    Rng rng(seed);
    BitVec q(SDR::N);
    for (uint64_t t = 0; t < trials; ++t) {
        unsigned s = rng.fast_below(kItems);
        unsigned tb = rng.fast_below(3u);
        (void)tb;
        unsigned u[kNSym];
        for (unsigned i = 0; i < kNSym; ++i) u[i] = rng.fast_below(200u);
        corrupt_sdr(store[s], q, ch, fpct, u, rng);
        int idx = SDR::nearest_decode(q, store.data(), kItems);
        unsigned ov = SDR::overlap(q, store[static_cast<unsigned>(idx)]);
        if (idx == static_cast<int>(s) && ov >= thr) {
            ++out.tp;
        } else if (idx != static_cast<int>(s) && ov >= thr) {
            ++out.fp;
        } else {
            ++out.fn;
        }
    }
}

// Pooled arm: query = corrupted copy of the stored ITEM (base code); recall
// scores chunk views of that one code and clamps negative chunks to zero.
void run_pooled(BddMemory& mem, const std::vector<Code>& bases,
                const std::vector<uint64_t>& ids, int ch, unsigned fpct,
                unsigned thr, uint64_t trials, uint64_t seed, Counts& out) {
    mem.set_threshold(static_cast<int32_t>(thr));
    Rng rng(seed);
    Code q;
    for (uint64_t t = 0; t < trials; ++t) {
        unsigned s = rng.fast_below(kItems);
        unsigned u[kNSym];
        for (unsigned i = 0; i < kNSym; ++i) u[i] = rng.fast_below(200u);
        corrupt_code(bases[s], q, ch, fpct, u);
        uint64_t got = 0;
        if (mem.recall_pooled(q, got)) {
            if (got == ids[s]) {
                ++out.tp;
            } else {
                ++out.fp;
            }
        } else {
            ++out.fn;
        }
    }
}

// Legacy arm: pre-redesign system. Query = corrupted copy of ONE of 3
// independently-seeded branches (uniform); evidence = sum of 3 whole-code
// signed sims; fixed threshold 24.
void run_legacy(BddMemory& mem, const std::vector<Code>& branches,
                const std::vector<uint64_t>& ids, int ch, unsigned fpct,
                uint64_t trials, uint64_t seed, Counts& out) {
    Rng rng(seed);
    Code q;
    for (uint64_t t = 0; t < trials; ++t) {
        unsigned s = rng.fast_below(kItems);
        unsigned tb = rng.fast_below(3u);
        unsigned u[kNSym];
        for (unsigned i = 0; i < kNSym; ++i) u[i] = rng.fast_below(200u);
        corrupt_code(branches[s * 3u + tb], q, ch, fpct, u);
        uint64_t got = 0;
        if (mem.recall_legacy(q, got,
                              static_cast<int32_t>(kLegacyThr))) {
            if (got == ids[s]) {
                ++out.tp;
            } else {
                ++out.fp;
            }
        } else {
            ++out.fn;
        }
    }
}

}  // namespace

int main() {
    std::vector<BitVec> sdr_store(kItems, BitVec(SDR::N));
    {
        Rng rng(kMasterSeed ^ 0x5344525ull);
        for (unsigned i = 0; i < kItems; ++i)
            SDR::encode_random(sdr_store[i], rng);
    }
    std::vector<uint64_t> ids(kItems);
    std::vector<Code> bases(kItems, Code{});
    std::vector<Code> branches(kItems * 3u, Code{});
    BddMemory mem(kItems);
    for (unsigned i = 0; i < kItems; ++i) {
        ids[i] = 900000ull + i;
        assert(mem.store(ids[i]));
        {
            Rng br(item_seed(ids[i]));
            bases[i] = encode(ids[i], br);
        }
        for (unsigned t = 0; t < BddMemory::BRANCHES; ++t) {
            Rng br(branch_seed(ids[i], t));
            branches[i * 3u + t] = encode(ids[i], br);
        }
    }

    printf("=== OmegaDR-BDD(chunked-pooled) vs SDR noise bench ===\n");
    printf("stored_items=%u channels=3 f_steps=10 trials_per_point=%u "
           "cal_trials_per_point=%u master_seed=%llu\n",
           kItems, kTrials, kCalTrials,
           static_cast<unsigned long long>(kMasterSeed));

    // ---- calibration at f = 0.20, metric F1, mean over channels ----
    printf("[calibration] f=0.20 metric=F1 (ties -> lowest threshold)\n");
    double best_o = -1.0;
    unsigned omega_thr = kOmegaPooledThr[0];
    for (unsigned ti = 0; ti < 3; ++ti) {
        double mean = 0.0;
        double per[kChannels];
        for (unsigned ch = 0; ch < kChannels; ++ch) {
            Counts c;
            run_pooled(mem, bases, ids, static_cast<int>(ch), 20u,
                       kOmegaPooledThr[ti], kCalTrials,
                       point_seed(ch, 3u, kSaltCal), c);
            per[ch] = f1_of(c);
            mean += per[ch];
        }
        mean /= static_cast<double>(kChannels);
        printf("  omega_pooled thr=%2u erasure=%.5f mixed50=%.5f flip=%.5f "
               "mean=%.5f\n",
               kOmegaPooledThr[ti], per[0], per[1], per[2], mean);
        if (mean > best_o) {
            best_o = mean;
            omega_thr = kOmegaPooledThr[ti];
        }
    }
    double best_s = -1.0;
    unsigned sdr_thr = kSdrThr[0];
    for (unsigned ti = 0; ti < 3; ++ti) {
        double mean = 0.0;
        double per[kChannels];
        for (unsigned ch = 0; ch < kChannels; ++ch) {
            Counts c;
            run_sdr(sdr_store, static_cast<int>(ch), 20u, kSdrThr[ti],
                    kCalTrials, point_seed(ch, 3u, kSaltCal), c);
            per[ch] = f1_of(c);
            mean += per[ch];
        }
        mean /= static_cast<double>(kChannels);
        printf("  sdr   thr=%2u erasure=%.5f mixed50=%.5f flip=%.5f mean=%.5f\n",
               kSdrThr[ti], per[0], per[1], per[2], mean);
        if (mean > best_s) {
            best_s = mean;
            sdr_thr = kSdrThr[ti];
        }
    }
    {
        double mean = 0.0;
        double per[kChannels];
        for (unsigned ch = 0; ch < kChannels; ++ch) {
            Counts c;
            run_legacy(mem, branches, ids, static_cast<int>(ch), 20u,
                       kCalTrials, point_seed(ch, 3u, kSaltCal), c);
            per[ch] = f1_of(c);
            mean += per[ch];
        }
        mean /= static_cast<double>(kChannels);
        printf("  legacy  thr=%2u erasure=%.5f mixed50=%.5f flip=%.5f "
               "mean=%.5f (fixed, not calibrated)\n",
               kLegacyThr, per[0], per[1], per[2], mean);
    }
    printf("[calibration] chosen omega_pooled_threshold=%u sdr_threshold=%u "
           "legacy_threshold=%u\n",
           omega_thr, sdr_thr, kLegacyThr);

    // ---- sweep ----
    double rates[kChannels][kFSteps][3];
    for (unsigned ch = 0; ch < kChannels; ++ch) {
        printf("[channel] %s\n"
               "    f     sdr_hit   omega_hit   omega_legacy_hit\n",
               kChName[ch]);
        for (unsigned fi = 0; fi < kFSteps; ++fi) {
            unsigned fpct = 5u * (fi + 1u);
            Counts cs, co, cl;
            run_sdr(sdr_store, static_cast<int>(ch), fpct, sdr_thr, kTrials,
                    point_seed(ch, fi, kSaltSweep), cs);
            run_pooled(mem, bases, ids, static_cast<int>(ch), fpct, omega_thr,
                       kTrials, point_seed(ch, fi, kSaltSweep), co);
            run_legacy(mem, branches, ids, static_cast<int>(ch), fpct, kTrials,
                       point_seed(ch, fi, kSaltSweep), cl);
            rates[ch][fi][0] =
                static_cast<double>(cs.tp) / static_cast<double>(kTrials);
            rates[ch][fi][1] =
                static_cast<double>(co.tp) / static_cast<double>(kTrials);
            rates[ch][fi][2] =
                static_cast<double>(cl.tp) / static_cast<double>(kTrials);
            printf("  0.%02u   %.5f   %.5f   %.5f\n", fpct, rates[ch][fi][0],
                   rates[ch][fi][1], rates[ch][fi][2]);
        }
    }

    FILE* f = fopen("reports/noise_sweep.csv", "w");
    if (f == nullptr) {
        fprintf(stderr, "cannot open reports/noise_sweep.csv\n");
        return 1;
    }
    fprintf(f,
            "# OmegaDR-BDD(chunked-pooled) vs SDR baseline noise sweep; M=%u "
            "stored items; %u trials/point; fresh seeded xorshift Rng per "
            "point\n",
            kItems, kTrials);
    fprintf(f,
            "# hit = correct item is argmax similarity AND sim >= threshold; "
            "omega_pooled_threshold=%u sdr_threshold=%u (calibrated at f=0.2, "
            "max mean F1 over channels, fixed for whole sweep); "
            "legacy_threshold=%u (fixed)\n",
            omega_thr, sdr_thr, kLegacyThr);
    fprintf(f,
            "# CHUNKED redesign: one base code per item, m=3 word-aligned "
            "chunk views (640/704/704 bits = 10/11/11 words); pooled score = "
            "sum over chunks of max(0, signed chunk sim)\n");
    fprintf(f,
            "# omega query = corrupted copy of the stored base code; legacy "
            "query = corrupted copy of one independently-seeded branch "
            "(uniform); SDR channels per SPEC (mixed50/flip analogs)\n");
    fprintf(f, "# ASSOCIATIVE RECALL under query corruption; channels applied "
               "to retrieval query, stored data intact\n");
    fprintf(f, "channel,f,sdr_hit_rate,omega_hit_rate,omega_legacy_hit_rate\n");
    for (unsigned ch = 0; ch < kChannels; ++ch)
        for (unsigned fi = 0; fi < kFSteps; ++fi)
            fprintf(f, "%s,0.%02u,%.5f,%.5f,%.5f\n", kChName[ch],
                    5u * (fi + 1u), rates[ch][fi][0], rates[ch][fi][1],
                    rates[ch][fi][2]);
    fclose(f);

    // ---- crossover + success criterion ----
    for (unsigned ch = 0; ch < kChannels; ++ch) {
        bool crossed = false;
        for (unsigned fi = 0; fi + 1u < kFSteps; ++fi) {
            double d0 = rates[ch][fi][1] - rates[ch][fi][0];
            double d1 = rates[ch][fi + 1u][1] - rates[ch][fi + 1u][0];
            if ((d0 < 0.0 && d1 > 0.0) || (d0 > 0.0 && d1 < 0.0)) {
                double fa = 0.05 * static_cast<double>(fi + 1u);
                double fb = fa + 0.05;
                double fst = fa + (fb - fa) * (d0 / (d0 - d1));
                printf("[crossover] %s: omega_pooled crosses sdr at f~%.3f\n",
                       kChName[ch], fst);
                crossed = true;
            }
        }
        if (!crossed)
            printf("[crossover] %s: none (sdr_hit >= omega_pooled_hit at all "
                   "f)\n",
                   kChName[ch]);
    }
    bool beat_any = false;
    for (unsigned ch = 0; ch < kChannels; ++ch) {
        bool beat = false;
        for (unsigned fi = 4u; fi < kFSteps; ++fi) {
            if (rates[ch][fi][1] >= 0.95 &&
                rates[ch][fi][1] > rates[ch][fi][0])
                beat = true;
        }
        printf("[criterion] %s: omega_pooled>=0.95 at some f>=0.40 with sdr "
               "lower: %s\n",
               kChName[ch], beat ? "YES" : "NO");
        beat_any = beat_any || beat;
    }
    printf("[criterion] any channel meets criterion: %s\n",
           beat_any ? "YES" : "NO");

    // ---- throughput (stderr: keeps stdout byte-identical per V3) ----
    {
        std::vector<Code> oq_base;
        std::vector<Code> oq_leg;
        std::vector<BitVec> sq;
        oq_base.reserve(kTimeN);
        oq_leg.reserve(kTimeN);
        sq.reserve(kTimeN);
        for (unsigned i = 0; i < kTimeN; ++i) {
            oq_base.push_back(bases[i % kItems]);
            oq_leg.push_back(branches[(i % kItems) * 3u + (i % 3u)]);
            sq.push_back(sdr_store[i % kItems]);
        }
        uint64_t sink = 0;
        mem.set_threshold(static_cast<int32_t>(omega_thr));
        for (unsigned i = 0; i < kTimeN; ++i) {
            uint64_t o = 0;
            if (mem.recall_pooled(oq_base[i], o)) sink += o;
        }
        for (unsigned i = 0; i < kTimeN; ++i) {
            uint64_t o = 0;
            if (mem.recall_legacy(oq_leg[i], o,
                                  static_cast<int32_t>(kLegacyThr)))
                sink += o;
        }
        for (unsigned i = 0; i < kTimeN; ++i)
            sink += static_cast<uint64_t>(
                SDR::nearest_decode(sq[i], sdr_store.data(), kItems));
        auto t0 = std::chrono::steady_clock::now();
        for (unsigned i = 0; i < kTimeN; ++i) {
            uint64_t o = 0;
            if (mem.recall_pooled(oq_base[i], o)) sink += o;
        }
        auto t1 = std::chrono::steady_clock::now();
        for (unsigned i = 0; i < kTimeN; ++i) {
            uint64_t o = 0;
            if (mem.recall_legacy(oq_leg[i], o,
                                  static_cast<int32_t>(kLegacyThr)))
                sink += o;
        }
        auto t2 = std::chrono::steady_clock::now();
        for (unsigned i = 0; i < kTimeN; ++i)
            sink += static_cast<uint64_t>(
                SDR::nearest_decode(sq[i], sdr_store.data(), kItems));
        auto t3 = std::chrono::steady_clock::now();
        double ns_po =
            std::chrono::duration<double, std::nano>(t1 - t0).count() /
            static_cast<double>(kTimeN);
        double ns_le =
            std::chrono::duration<double, std::nano>(t2 - t1).count() /
            static_cast<double>(kTimeN);
        double ns_sd =
            std::chrono::duration<double, std::nano>(t3 - t2).count() /
            static_cast<double>(kTimeN);
        fprintf(stderr,
                "[throughput] M=%u recalls=%u omega_pooled=%.1f ns/op "
                "omega_legacy=%.1f ns/op sdr_nearest_decode=%.1f ns/op "
                "pooled/sdr=%.2fx pooled/legacy=%.2fx sink=%llu\n",
                kItems, kTimeN, ns_po, ns_le, ns_sd, ns_po / ns_sd,
                ns_po / ns_le, static_cast<unsigned long long>(sink));
        FILE* tf = fopen("reports/throughput.csv", "w");
        if (tf != nullptr) {
            fprintf(tf, "system,items,recalls,mean_ns_per_op\n");
            fprintf(tf, "omega_pooled_recall,%u,%u,%.1f\n", kItems, kTimeN,
                    ns_po);
            fprintf(tf, "omega_legacy_recall,%u,%u,%.1f\n", kItems, kTimeN,
                    ns_le);
            fprintf(tf, "sdr_nearest_decode,%u,%u,%.1f\n", kItems, kTimeN,
                    ns_sd);
            fclose(tf);
        }
        (void)sink;
    }

    printf("NOISE_BENCH_DONE\n");
    return 0;
}
