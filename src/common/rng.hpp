#pragma once
#include <cstdint>

class Rng {
public:
    explicit Rng(uint64_t seed)
        : x_(seed ? seed : 0x9E3779B97F4A7C15ull), mask_n_(0), mask_(0) {}

    uint64_t next_u64() {
        step();
        return x_;
    }

    unsigned fast_below(unsigned n) {
        if (n <= 1) return 0;
        if (n != mask_n_) rebuild_mask(n);
        step();
        uint64_t v = (x_ >> 33) & mask_;
        while (v >= static_cast<uint64_t>(n)) {
            step();
            v = (x_ >> 33) & mask_;
        }
        return static_cast<unsigned>(v);
    }

private:
    void step() {
        x_ ^= x_ << 13;
        x_ ^= x_ >> 7;
        x_ ^= x_ << 17;
    }

    void rebuild_mask(unsigned n) {
        uint64_t m = 1;
        while (m < static_cast<uint64_t>(n)) m = (m << 1) | 1ull;
        mask_n_ = n;
        mask_ = m;
    }

    uint64_t x_;
    unsigned mask_n_;
    uint64_t mask_;
};
