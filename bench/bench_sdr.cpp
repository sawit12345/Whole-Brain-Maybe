#include <chrono>
#include <cstdint>
#include <cstdio>
#include <vector>
#include "../src/sdr_baseline/sdr.hpp"
#include "../src/common/rng.hpp"

using Clock = std::chrono::steady_clock;

static double ns_since(Clock::time_point t0, uint64_t ops) {
    auto t1 = Clock::now();
    double ns = static_cast<double>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
    return ns / static_cast<double>(ops);
}

int main() {
    const unsigned pool = 1024;
    const uint64_t overlap_iters = 1000000ull;
    const uint64_t decode_iters = 10000ull;

    Rng rng(20260824ull);
    std::vector<BitVec> stored;
    stored.reserve(pool);
    for (unsigned j = 0; j < pool; ++j) {
        BitVec p(SDR::N);
        SDR::encode_random(p, rng);
        stored.push_back(p);
    }

    uint64_t sink = 0;
    auto t0 = Clock::now();
    for (uint64_t t = 0; t < overlap_iters; ++t) {
        const BitVec& a = stored[static_cast<unsigned>(t & 1023u)];
        const BitVec& b =
            stored[static_cast<unsigned>((t >> 10) & 1023u)];
        sink += SDR::overlap(a, b);
    }
    double ov_ns = ns_since(t0, overlap_iters);

    t0 = Clock::now();
    uint64_t hits = 0;
    for (uint64_t t = 0; t < decode_iters; ++t) {
        BitVec query = stored[static_cast<unsigned>(t & 1023u)];
        SDR::noise_corrupt(query,
                           1 + static_cast<unsigned>((t >> 10) & 7u), rng);
        int idx = SDR::nearest_decode(query, stored.data(), pool);
        if (idx == static_cast<int>(t & 1023u)) ++hits;
    }
    double dec_ns = ns_since(t0, decode_iters);

    double ov_mops = 1000.0 / ov_ns;
    double dec_mops = 1000.0 / dec_ns;

    printf("bench_sdr: n=%u w=%u pool=%u\n", SDR::N, SDR::W, pool);
    printf("overlaps:      %llu ops | %8.1f ns/op | %10.2f Mops/s\n",
           (unsigned long long)overlap_iters, ov_ns, ov_mops);
    printf("nearest-decode: %llu ops | %8.1f ns/op | %10.4f Mops/s\n",
           (unsigned long long)decode_iters, dec_ns, dec_mops);
    printf("decode hit rate: %llu/%llu | sink=%llu\n",
           (unsigned long long)hits, (unsigned long long)decode_iters,
           (unsigned long long)(sink & 0xffffull));
    return 0;
}
