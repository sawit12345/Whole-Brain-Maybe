#pragma once
#include <cstdint>
#include <vector>
#include "rng.hpp"

class BitVec {
public:
    explicit BitVec(unsigned nbits);

    unsigned size() const { return nbits_; }
    unsigned word_count() const { return words_; }
    const uint64_t* data() const { return w_.data(); }

    void set(unsigned i);
    void clear(unsigned i);
    void toggle(unsigned i);
    bool test(unsigned i) const;
    void zero();
    unsigned popcount() const;

    BitVec operator&(const BitVec& o) const;
    BitVec operator|(const BitVec& o) const;
    BitVec operator^(const BitVec& o) const;
    BitVec operator~() const;
    BitVec& operator&=(const BitVec& o);
    BitVec& operator|=(const BitVec& o);
    BitVec& operator^=(const BitVec& o);

    void shl_inplace(unsigned k);
    void shr_inplace(unsigned k);

    void random_init(Rng& rng);

private:
    unsigned rem_bits() const;
    void clear_top();
    void check_same(const BitVec& o) const;

    std::vector<uint64_t> w_;
    unsigned nbits_;
    unsigned words_;
};
