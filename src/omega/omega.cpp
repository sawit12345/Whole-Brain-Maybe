#include "omega.hpp"

#include <cassert>

namespace omega {

static inline uint32_t pop_word(uint64_t v) {
    return static_cast<uint32_t>(__builtin_popcountll(v));
}

static inline uint64_t rotr64(uint64_t x, unsigned r) {
    r &= 63u;
    if (r == 0u) return x;
    return (x >> r) | (x << (64u - r));
}

uint64_t mix_streams(uint64_t s, uint64_t t) {
    uint64_t x = s ^ (t + 0x9E3779B97F4A7C15ull + (s << 6) + (s >> 2));
    x ^= x >> 30;
    x += 0xBF58476D1CE4E5B9ull;
    x ^= x >> 27;
    x += 0x94D049BB133111EBull;
    x ^= x >> 31;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return x;
}

bool invariant_ok(const Code& c) {
    return (c.pos & c.neg).popcount() == 0u;
}

uint32_t sim(const Code& a, const Code& b) {
    const uint64_t* ap = a.pos.data();
    const uint64_t* an = a.neg.data();
    const uint64_t* bp = b.pos.data();
    const uint64_t* bn = b.neg.data();
    uint32_t pp = 0;
    uint32_t nn = 0;
    uint32_t pn = 0;
    uint32_t np = 0;
    for (unsigned j = 0; j < N_WORDS; ++j) {
        pp += pop_word(ap[j] & bp[j]);
        nn += pop_word(an[j] & bn[j]);
        pn += pop_word(ap[j] & bn[j]);
        np += pop_word(an[j] & bp[j]);
    }
    return static_cast<uint32_t>(static_cast<int32_t>(pp + nn - pn - np));
}

static void pick_subset(Rng& rng, BitVec& out, const BitVec* avoid) {
    out.zero();
    unsigned placed = 0;
    const uint64_t mask = static_cast<uint64_t>(N_BITS) - 1ull;
    while (placed < K_PER_PLANE) {
        unsigned p = static_cast<unsigned>(rng.next_u64() & mask);
        if (out.test(p)) continue;
        if (avoid != nullptr && avoid->test(p)) continue;
        out.set(p);
        ++placed;
    }
}

Code encode(uint64_t stream_id, Rng& rng) {
    Rng local(mix_streams(stream_id, rng.next_u64()));
    Code c;
    pick_subset(local, c.pos, nullptr);
    pick_subset(local, c.neg, &c.pos);
    assert(invariant_ok(c));
    assert(c.pos.popcount() == K_PER_PLANE);
    assert(c.neg.popcount() == K_PER_PLANE);
    return c;
}

static void rot_words_into(const BitVec& in, BitVec& out, unsigned r) {
    out.zero();
    const uint64_t* pw = in.data();
    for (unsigned j = 0; j < N_WORDS; ++j) {
        uint64_t w = rotr64(pw[j], r);
        while (w != 0ull) {
            unsigned b = static_cast<unsigned>(__builtin_ctzll(w));
            out.set((j << 6) | b);
            w &= w - 1ull;
        }
    }
}

Code bind(const Code& x, const Code& role) {
    Code y;
    unsigned rp = role.pos.popcount() & 63u;
    unsigned rn = role.neg.popcount() & 63u;
    rot_words_into(x.pos, y.pos, rp);
    rot_words_into(x.neg, y.neg, rn);
    y.pos ^= role.pos;
    y.neg ^= role.neg;
    return y;
}

Code unbind(const Code& y, const Code& role) {
    Code x;
    unsigned rp = role.pos.popcount() & 63u;
    unsigned rn = role.neg.popcount() & 63u;
    Code t;
    t.pos = y.pos;
    t.pos ^= role.pos;
    rot_words_into(t.pos, x.pos, (64u - rp) & 63u);
    t.neg = y.neg;
    t.neg ^= role.neg;
    rot_words_into(t.neg, x.neg, (64u - rn) & 63u);
    return x;
}

BddMemory::BddMemory(unsigned capacity_slots) : slots_(capacity_slots) {}

bool BddMemory::store(uint64_t stream_id) {
    for (const Slot& s : slots_) {
        if (s.used && s.stream == stream_id) return false;
    }
    for (Slot& s : slots_) {
        if (s.used) continue;
        s.used = true;
        s.stream = stream_id;
        for (unsigned t = 0; t < BRANCHES; ++t) {
            Rng branch_rng(branch_seed(stream_id, t));
            s.branch[t] = encode(stream_id, branch_rng);
            s.conf_bits[t] = 0;
        }
        return true;
    }
    return false;
}

bool BddMemory::recall(const Code& q, uint64_t& out_stream) {
    ++recalls_;
    bool found = false;
    int32_t best = 0;
    uint64_t best_stream = 0;
    for (const Slot& s : slots_) {
        if (!s.used) continue;
        int32_t total = static_cast<int32_t>(sim(q, s.branch[0])) +
                        static_cast<int32_t>(sim(q, s.branch[1])) +
                        static_cast<int32_t>(sim(q, s.branch[2]));
        if (!found || total > best) {
            found = true;
            best = total;
            best_stream = s.stream;
        }
    }
    if (found && best >= threshold_) {
        out_stream = best_stream;
        ++hits_;
        return true;
    }
    ++misses_;
    return false;
}

void BddMemory::strengthen(uint64_t stream_id) {
    for (Slot& s : slots_) {
        if (!s.used || s.stream != stream_id) continue;
        for (unsigned t = 0; t < BRANCHES; ++t)
            s.conf_bits[t] = inc2(s.conf_bits[t]);
        return;
    }
}

void BddMemory::decay_all() {
    for (Slot& s : slots_) {
        if (!s.used) continue;
        for (unsigned t = 0; t < BRANCHES; ++t)
            s.conf_bits[t] = dec2(s.conf_bits[t]);
    }
}

uint8_t BddMemory::conf(uint64_t stream_id, unsigned branch) const {
    for (const Slot& s : slots_) {
        if (s.used && s.stream == stream_id) return s.conf_bits[branch];
    }
    return 0;
}

BddMemory::Stats BddMemory::stats() const {
    Stats st;
    st.capacity = static_cast<unsigned>(slots_.size());
    st.slots_used = 0;
    for (const Slot& s : slots_)
        if (s.used) ++st.slots_used;
    st.recalls = recalls_;
    st.hits = hits_;
    st.misses = misses_;
    st.threshold = threshold_;
    return st;
}

Router::Router(unsigned num_areas, unsigned fanout)
    : n_(num_areas), k_(fanout) {
    for (unsigned i = 0; i < MAX_AREAS; ++i) masks_[i] = 0ull;
}

void Router::set_gate(unsigned area_i, unsigned area_j, bool on) {
    uint64_t bit = 1ull << (area_j & 63u);
    if (on) {
        masks_[area_i] |= bit;
    } else {
        masks_[area_i] &= ~bit;
    }
}

bool Router::gate(unsigned area_i, unsigned area_j) const {
    return (masks_[area_i] >> (area_j & 63u)) & 1ull;
}

unsigned Router::route(const Code& x, unsigned* out_area,
                       uint32_t* out_score) const {
    const uint64_t probe = x.pos.data()[0];
    unsigned cnt = 0;
    for (unsigned i = 0; i < n_; ++i) {
        uint32_t s = pop_word(masks_[i] & probe);
        if (cnt == k_) {
            if (!(s > out_score[0])) continue;
            unsigned m = 0;
            while (m + 1u < cnt && out_score[m + 1u] == out_score[0]) ++m;
            for (unsigned t = m; t + 1u < cnt; ++t) {
                out_score[t] = out_score[t + 1u];
                out_area[t] = out_area[t + 1u];
            }
            --cnt;
        }
        unsigned pos = 0;
        while (pos < cnt && out_score[pos] <= s) ++pos;
        for (unsigned t = cnt; t > pos; --t) {
            out_score[t] = out_score[t - 1u];
            out_area[t] = out_area[t - 1u];
        }
        out_score[pos] = s;
        out_area[pos] = i;
        ++cnt;
    }
    return cnt;
}

GridClock::GridClock() {
    static constexpr uint16_t PERIODS[3] = {61u, 127u, 251u};
    for (unsigned i = 0; i < 3; ++i) {
        remain_[i] = PERIODS[i];
        phase_[i] = false;
    }
}

void GridClock::tick() {
    static constexpr uint16_t PERIODS[3] = {61u, 127u, 251u};
    for (unsigned i = 0; i < 3; ++i) {
        remain_[i] = static_cast<uint16_t>(remain_[i] - 1u);
        phase_[i] = (remain_[i] == 0u);
        if (phase_[i]) remain_[i] = PERIODS[i];
    }
}

bool GridClock::cycle() const {
    return phase_[0] && phase_[1] && phase_[2];
}

bool GridClock::phase(unsigned which) const { return phase_[which]; }

Cerebellum::Cerebellum()
    : hashes_(static_cast<size_t>(EXP_BITS) << 6, 0), w_(EXP_BITS, 0) {
    static constexpr uint64_t FANIN_MASK = (1ull << INPUT_BITS_LOG2) - 1ull;
    for (unsigned j = 0; j < EXP_BITS; ++j) {
        uint64_t st = mix_streams(j, 0xA5A5A5A5A5A5A5A5ull);
        size_t base = static_cast<size_t>(j) << 6;
        uint64_t row[HASH_WORDS] = {};
        unsigned placed = 0;
        while (placed < GRANULE_FANIN) {
            st ^= st << 13;
            st ^= st >> 7;
            st ^= st << 17;
            unsigned b = static_cast<unsigned>(st & FANIN_MASK);
            uint64_t bit = 1ull << (b & 63u);
            uint64_t* slot = row + (b >> 6);
            if ((*slot & bit) != 0ull) continue;
            *slot |= bit;
            ++placed;
        }
        for (unsigned t = 0; t < HASH_WORDS; ++t)
            hashes_[base + t] = row[t];
    }
}

void Cerebellum::recode(const Code& in, BitVec& out) const {
    assert(out.size() == EXP_BITS);
    out.zero();
    uint64_t comb[N_WORDS + N_WORDS];
    const uint64_t* pp = in.pos.data();
    const uint64_t* pn = in.neg.data();
    for (unsigned t = 0; t < N_WORDS; ++t) {
        comb[t] = pp[t];
        comb[N_WORDS + t] = pn[t];
    }
    const uint64_t* base = hashes_.data();
    for (unsigned j = 0; j < EXP_BITS; ++j) {
        const uint64_t* row = base + (static_cast<size_t>(j) << 6);
        uint64_t acc = 0;
        for (unsigned t = 0; t < HASH_WORDS && acc == 0ull; ++t)
            acc |= row[t] & comb[t];
        if (acc != 0ull) out.set(j);
    }
}

int Cerebellum::predict(const Code& in) const {
    BitVec ex(EXP_BITS);
    recode(in, ex);
    const uint64_t* px = ex.data();
    int32_t sum = 0;
    for (unsigned t = 0; t < (EXP_BITS >> 6); ++t) {
        uint64_t wv = px[t];
        while (wv != 0ull) {
            unsigned b = static_cast<unsigned>(__builtin_ctzll(wv));
            sum += w_[(static_cast<size_t>(t) << 6) + b];
            wv &= wv - 1ull;
        }
    }
    return (sum >= 0) ? 1 : -1;
}

void Cerebellum::train(const Code& in, int label) {
    if (predict(in) == label) return;
    BitVec ex(EXP_BITS);
    recode(in, ex);
    int8_t d = (label > 0) ? static_cast<int8_t>(1) : static_cast<int8_t>(-1);
    const uint64_t* px = ex.data();
    for (unsigned t = 0; t < (EXP_BITS >> 6); ++t) {
        uint64_t wv = px[t];
        while (wv != 0ull) {
            unsigned b = static_cast<unsigned>(__builtin_ctzll(wv));
            size_t idx = (static_cast<size_t>(t) << 6) + b;
            int v = w_[idx] + d;
            if (v > 127) v = 127;
            if (v < -128) v = -128;
            w_[idx] = static_cast<int8_t>(v);
            wv &= wv - 1ull;
        }
    }
}

Selector::Selector(unsigned num_channels) : n_(num_channels) {}

unsigned Selector::select() const {
    unsigned best = 0;
    for (unsigned c = 1; c < n_; ++c) {
        if (scores_[c] > scores_[best]) best = c;
    }
    return best;
}

void ModBus::apply(int& threshold) const {
    int da_v = da;
    int ach_v = ach;
    int ne_v = ne;
    int h_v = s5ht;
    threshold += (da_v >> 3) - (ach_v >> 4) + (ne_v >> 5) - (h_v >> 6);
    if (threshold > 127) threshold = 127;
    if (threshold < -128) threshold = -128;
}

}
