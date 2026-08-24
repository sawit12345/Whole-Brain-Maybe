#include <cassert>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "../src/common/bitvec.hpp"
#include "../src/common/rng.hpp"
#include "../src/omega/omega.hpp"
#include "../src/sdr_baseline/sdr.hpp"

using namespace omega;

namespace {

constexpr unsigned kNumM = 8;
constexpr unsigned kMs[kNumM] = {10, 25, 50, 100, 200, 400, 800, 1600};
constexpr unsigned kCalIdx = 3;  // M=100
constexpr unsigned kCalTrials = 20000;
constexpr unsigned kTrials = 10000;
constexpr uint64_t kMasterSeed = 0x3AB0C1A7ull;
constexpr unsigned kOmegaThr[3] = {16, 24, 32};
constexpr unsigned kSdrThr[3] = {13, 16, 21};

struct Counts {
    uint64_t tp = 0;
    uint64_t fp = 0;
    uint64_t fn = 0;
};

double f1_of(const Counts& c) {
    double denom = static_cast<double>(2 * c.tp + c.fp + c.fn);
    return 2.0 * static_cast<double>(c.tp) / denom;
}

uint64_t cfg_seed(unsigned mi, uint64_t salt) {
    return mix_streams(static_cast<uint64_t>(mi), kMasterSeed ^ salt);
}

void run_clean_omega(BddMemory& mem, const std::vector<Code>& bases,
                     const std::vector<uint64_t>& ids, unsigned items,
                     unsigned thr, uint64_t trials, uint64_t seed, Counts& out) {
    mem.set_threshold(static_cast<int32_t>(thr));
    Rng rng(seed);
    for (uint64_t t = 0; t < trials; ++t) {
        unsigned s = rng.fast_below(items);
        Code q = bases[s];
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

void run_clean_sdr(const std::vector<BitVec>& store, unsigned items,
                   unsigned thr, uint64_t trials, uint64_t seed, Counts& out) {
    Rng rng(seed);
    for (uint64_t t = 0; t < trials; ++t) {
        unsigned s = rng.fast_below(items);
        int idx = SDR::nearest_decode(store[s], store.data(), items);
        unsigned ov = SDR::overlap(store[s], store[static_cast<unsigned>(idx)]);
        if (idx == static_cast<int>(s) && ov >= thr) {
            ++out.tp;
        } else if (idx != static_cast<int>(s) && ov >= thr) {
            ++out.fp;
        } else {
            ++out.fn;
        }
    }
}

}  // namespace

int main() {
    printf("=== OmegaDR-BDD(chunked-pooled) vs SDR associative-recall "
           "capacity bench ===\n");
    printf("clean-copy queries; hit = correct argmax AND sim >= threshold; "
           "trials_per_point=%u master_seed=%llu\n",
           kTrials, static_cast<unsigned long long>(kMasterSeed));
    printf("NOTE: measures ASSOCIATIVE MEMORY capacity only (state-space "
           "sizes 10^95.51 vs 10^84.38 are analytic results, not measured "
           "here)\n");

    // ---- calibration once at M=100 on clean queries ----
    double best_o = -1.0;
    unsigned omega_thr = kOmegaThr[0];
    double best_s = -1.0;
    unsigned sdr_thr = kSdrThr[0];
    {
        constexpr unsigned M = 100;
        std::vector<BitVec> store(M, BitVec(SDR::N));
        {
            Rng rng(kMasterSeed ^ 0x5344435ull);
            for (unsigned i = 0; i < M; ++i)
                SDR::encode_random(store[i], rng);
        }
        std::vector<uint64_t> ids(M);
        std::vector<Code> bases(M, Code{});
        BddMemory mem(M);
        for (unsigned i = 0; i < M; ++i) {
            ids[i] = 700000ull + i;
            assert(mem.store(ids[i]));
            Rng br(item_seed(ids[i]));
            bases[i] = encode(ids[i], br);
        }
        printf("[calibration] M=100 clean queries metric=F1 (ties -> lowest "
               "threshold)\n");
        for (unsigned ti = 0; ti < 3; ++ti) {
            Counts c;
            run_clean_omega(mem, bases, ids, M, kOmegaThr[ti], kCalTrials,
                            cfg_seed(kCalIdx, 0xBEF5ull), c);
            double v = f1_of(c);
            printf("  omega_pooled thr=%2u F1=%.5f (tp=%llu fp=%llu fn=%llu)\n",
                   kOmegaThr[ti], v, static_cast<unsigned long long>(c.tp),
                   static_cast<unsigned long long>(c.fp),
                   static_cast<unsigned long long>(c.fn));
            if (v > best_o) {
                best_o = v;
                omega_thr = kOmegaThr[ti];
            }
        }
        for (unsigned ti = 0; ti < 3; ++ti) {
            Counts c;
            run_clean_sdr(store, M, kSdrThr[ti], kCalTrials,
                          cfg_seed(kCalIdx, 0xBEF5ull), c);
            double v = f1_of(c);
            printf("  sdr   thr=%2u F1=%.5f (tp=%llu fp=%llu fn=%llu)\n",
                   kSdrThr[ti], v, static_cast<unsigned long long>(c.tp),
                   static_cast<unsigned long long>(c.fp),
                   static_cast<unsigned long long>(c.fn));
            if (v > best_s) {
                best_s = v;
                sdr_thr = kSdrThr[ti];
            }
        }
        printf("[calibration] chosen omega_threshold=%u sdr_threshold=%u "
               "(fixed for all M)\n", omega_thr, sdr_thr);
    }

    // ---- capacity sweep ----
    double rates[kNumM][2];
    for (unsigned mi = 0; mi < kNumM; ++mi) {
        const unsigned M = kMs[mi];
        std::vector<BitVec> store(M, BitVec(SDR::N));
        {
            Rng rng(cfg_seed(mi, 0x51D2ull));
            for (unsigned i = 0; i < M; ++i)
                SDR::encode_random(store[i], rng);
        }
        std::vector<uint64_t> ids(M);
        std::vector<Code> bases(M, Code{});
        BddMemory mem(M);
        for (unsigned i = 0; i < M; ++i) {
            ids[i] = 800000ull + 7919ull * mi + i;
            assert(mem.store(ids[i]));
            Rng br(item_seed(ids[i]));
            bases[i] = encode(ids[i], br);
        }
        Counts cs, co;
        run_clean_sdr(store, M, sdr_thr, kTrials, cfg_seed(mi, 0xCA0Aull), cs);
        run_clean_omega(mem, bases, ids, M, omega_thr, kTrials,
                        cfg_seed(mi, 0xCA0Aull), co);
        rates[mi][0] =
            static_cast<double>(cs.tp) / static_cast<double>(kTrials);
        rates[mi][1] =
            static_cast<double>(co.tp) / static_cast<double>(kTrials);
        printf("[M=%4u] sdr_hit=%.5f omega_pooled_hit=%.5f\n", M, rates[mi][0],
               rates[mi][1]);
    }

    FILE* f = fopen("reports/capacity_curves.csv", "w");
    if (f == nullptr) {
        fprintf(stderr, "cannot open reports/capacity_curves.csv\n");
        return 1;
    }
    fprintf(f,
            "# ASSOCIATIVE MEMORY recall capacity curves (NOT representational "
            "state-space capacity; 10^95.51 vs 10^84.38 is an analytic result "
            "measured nowhere here)\n");
    fprintf(f,
            "# clean-copy queries (base code per item); hit = correct item is "
            "argmax similarity AND sim >= threshold; %u trials per M\n",
            kTrials);
    fprintf(f,
            "# omega_pooled_threshold=%u sdr_threshold=%u calibrated once at "
            "M=100 (max F1 on clean queries)\n",
            omega_thr, sdr_thr);
    fprintf(f, "M,sdr_hit_rate,omega_pooled_hit_rate\n");
    for (unsigned mi = 0; mi < kNumM; ++mi)
        fprintf(f, "%u,%.5f,%.5f\n", kMs[mi], rates[mi][0], rates[mi][1]);
    fclose(f);

    const char* m99[2] = {"not reached (>=0.99 through M=1600)",
                          "not reached (>=0.99 through M=1600)"};
    unsigned m99_val[2] = {0, 0};
    bool has99[2] = {false, false};
    for (unsigned sys = 0; sys < 2; ++sys) {
        for (unsigned mi = 0; mi < kNumM; ++mi) {
            if (rates[mi][sys] < 0.99) {
                has99[sys] = true;
                m99_val[sys] = kMs[mi];
                break;
            }
        }
    }
    if (has99[0])
        printf("[M_99] sdr: first below 0.99 at M=%u\n", m99_val[0]);
    else
        printf("[M_99] sdr: %s\n", m99[0]);
    if (has99[1])
        printf("[M_99] omega_pooled: first below 0.99 at M=%u\n", m99_val[1]);
    else
        printf("[M_99] omega_pooled: %s\n", m99[1]);

    printf("CAPACITY_BENCH_DONE\n");
    return 0;
}
