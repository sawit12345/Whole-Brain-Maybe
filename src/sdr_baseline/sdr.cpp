#include "sdr.hpp"

#include <cassert>

void SDR::encode_random(BitVec& out, Rng& rng) {
    assert(out.size() >= N);
    out.zero();
    for (unsigned chosen = 0; chosen < W; ++chosen) {
        unsigned pos = rng.fast_below(N);
        while (out.test(pos)) pos = rng.fast_below(N);
        out.set(pos);
    }
    assert(out.popcount() == W);
}

unsigned SDR::overlap(const BitVec& a, const BitVec& b) { return (a & b).popcount(); }

int SDR::nearest_decode(const BitVec& query, const BitVec* stored,
                        unsigned count) {
    assert(count > 0u);
    int best_idx = 0;
    unsigned best_ov = overlap(query, stored[0]);
    for (unsigned j = 1; j < count; ++j) {
        unsigned ov = overlap(query, stored[j]);
        if (ov > best_ov) {
            best_ov = ov;
            best_idx = static_cast<int>(j);
        }
    }
    return best_idx;
}

void SDR::noise_corrupt(BitVec& pattern, unsigned num_flips, Rng& rng) {
    assert(pattern.size() >= N);
    if (num_flips == 0u) return;
    BitVec flipped(pattern.size());
    for (unsigned f = 0; f < num_flips; ++f) {
        unsigned pos = rng.fast_below(N);
        while (flipped.test(pos)) pos = rng.fast_below(N);
        flipped.set(pos);
        pattern.toggle(pos);
    }
}
