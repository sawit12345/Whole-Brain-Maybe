#include <cassert>
#include <cstdio>
#include <vector>
#include "../src/sdr_baseline/sdr.hpp"
#include "../src/common/rng.hpp"

static void test_encode_shape() {
    Rng rng(31415ull);
    for (unsigned t = 0; t < 32; ++t) {
        BitVec p(SDR::N);
        SDR::encode_random(p, rng);
        assert(p.popcount() == SDR::W);
    }
    printf("encode_random: exactly w=%u ones among n=%u: OK\n", SDR::W, SDR::N);
}

static void test_overlap_symmetry() {
    Rng rng(271828ull);
    for (unsigned t = 0; t < 64; ++t) {
        BitVec a(SDR::N), b(SDR::N);
        SDR::encode_random(a, rng);
        SDR::encode_random(b, rng);
        assert(SDR::overlap(a, b) == SDR::overlap(b, a));
    }
    BitVec a(SDR::N), b(SDR::N);
    for (unsigned i = 0; i < 10; ++i) { a.set(i); b.set(i); }
    assert(SDR::overlap(a, b) == 10);
    printf("overlap symmetry + exact small case: OK\n");
}

static void test_clean_decode() {
    const unsigned count = 16;
    Rng rng(161803ull);
    std::vector<BitVec> stored;
    for (unsigned j = 0; j < count; ++j) {
        BitVec p(SDR::N);
        SDR::encode_random(p, rng);
        stored.push_back(p);
    }
    for (unsigned k = 0; k < count; ++k) {
        BitVec query = stored[k];
        int idx = SDR::nearest_decode(query, stored.data(), count);
        assert(idx == static_cast<int>(k));
    }
    printf("clean decode recovers stored pattern (%u/%u): OK\n", count, count);
}

static void test_noisy_decode() {
    const unsigned count = 16;
    const unsigned trials = 100;
    unsigned hits = 0;
    for (unsigned t = 0; t < trials; ++t) {
        Rng rng(1000000ull + t);
        std::vector<BitVec> stored;
        for (unsigned j = 0; j < count; ++j) {
            BitVec p(SDR::N);
            SDR::encode_random(p, rng);
            stored.push_back(p);
        }
        BitVec target = stored[t % count];
        BitVec corrupted = target;
        SDR::noise_corrupt(corrupted, 1 + rng.fast_below(8), rng);
        int idx = SDR::nearest_decode(corrupted, stored.data(), count);
        if (idx == static_cast<int>(t % count)) ++hits;
    }
    printf("noisy decode (<=8 flips): %u/%u trials (threshold 95): %s\n",
           hits, trials, hits >= 95 ? "OK" : "FAIL");
    assert(hits >= 95);
}

int main() {
    test_encode_shape();
    test_overlap_symmetry();
    test_clean_decode();
    test_noisy_decode();
    printf("test_sdr: ALL PASSED\n");
    return 0;
}
