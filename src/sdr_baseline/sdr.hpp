#pragma once
#include "../common/bitvec.hpp"
#include "../common/rng.hpp"

struct SDR {
    static constexpr unsigned N = 2048;
    static constexpr unsigned W = 40;

    static void encode_random(BitVec& out, Rng& rng);
    static unsigned overlap(const BitVec& a, const BitVec& b);
    static int nearest_decode(const BitVec& query, const BitVec* stored,
                              unsigned count);
    static void noise_corrupt(BitVec& pattern, unsigned num_flips, Rng& rng);
};
