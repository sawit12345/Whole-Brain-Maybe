#include "bitvec.hpp"

#include <cassert>

BitVec::BitVec(unsigned nbits)
    : w_(static_cast<size_t>((nbits + 63u) >> 6), 0), nbits_(nbits),
      words_((nbits + 63u) >> 6) {}

void BitVec::set(unsigned i) {
    assert(i < nbits_);
    w_[i >> 6] |= (1ull << (i & 63u));
}

void BitVec::clear(unsigned i) {
    assert(i < nbits_);
    w_[i >> 6] &= ~(1ull << (i & 63u));
}

void BitVec::toggle(unsigned i) {
    assert(i < nbits_);
    w_[i >> 6] ^= (1ull << (i & 63u));
}

bool BitVec::test(unsigned i) const {
    assert(i < nbits_);
    return ((w_[i >> 6] >> (i & 63u)) & 1ull) != 0;
}

void BitVec::zero() {
    for (unsigned j = 0; j < words_; ++j) w_[j] = 0;
}

unsigned BitVec::popcount() const {
    unsigned s = 0;
    for (unsigned j = 0; j < words_; ++j) s += __builtin_popcountll(w_[j]);
    return s;
}

BitVec BitVec::operator&(const BitVec& o) const {
    check_same(o);
    BitVec r(nbits_);
    for (unsigned j = 0; j < words_; ++j) r.w_[j] = w_[j] & o.w_[j];
    return r;
}

BitVec BitVec::operator|(const BitVec& o) const {
    check_same(o);
    BitVec r(nbits_);
    for (unsigned j = 0; j < words_; ++j) r.w_[j] = w_[j] | o.w_[j];
    return r;
}

BitVec BitVec::operator^(const BitVec& o) const {
    check_same(o);
    BitVec r(nbits_);
    for (unsigned j = 0; j < words_; ++j) r.w_[j] = w_[j] ^ o.w_[j];
    return r;
}

BitVec BitVec::operator~() const {
    BitVec r(nbits_);
    for (unsigned j = 0; j < words_; ++j) r.w_[j] = ~w_[j];
    r.clear_top();
    return r;
}

BitVec& BitVec::operator&=(const BitVec& o) {
    check_same(o);
    for (unsigned j = 0; j < words_; ++j) w_[j] &= o.w_[j];
    return *this;
}

BitVec& BitVec::operator|=(const BitVec& o) {
    check_same(o);
    for (unsigned j = 0; j < words_; ++j) w_[j] |= o.w_[j];
    return *this;
}

BitVec& BitVec::operator^=(const BitVec& o) {
    check_same(o);
    for (unsigned j = 0; j < words_; ++j) w_[j] ^= o.w_[j];
    return *this;
}

void BitVec::shl_inplace(unsigned k) {
    if (k >= nbits_) { zero(); return; }
    unsigned ws = k >> 6;
    unsigned bs = k & 63u;
    if (bs == 0) {
        for (unsigned i = words_; i-- > ws;) w_[i] = w_[i - ws];
    } else {
        for (unsigned i = words_; i-- > ws;) {
            uint64_t lo = w_[i - ws] << bs;
            uint64_t hi = (i > ws) ? (w_[i - ws - 1u] >> (64u - bs)) : 0ull;
            w_[i] = lo | hi;
        }
    }
    for (unsigned i = 0; i < ws; ++i) w_[i] = 0;
    clear_top();
}

void BitVec::shr_inplace(unsigned k) {
    if (k >= nbits_) { zero(); return; }
    unsigned ws = k >> 6;
    unsigned bs = k & 63u;
    unsigned lim = words_ - ws;
    if (bs == 0) {
        for (unsigned i = 0; i < lim; ++i) w_[i] = w_[i + ws];
    } else {
        for (unsigned i = 0; i < lim; ++i) {
            uint64_t hi = w_[i + ws] >> bs;
            uint64_t lo =
                (i + ws + 1u < words_) ? (w_[i + ws + 1u] << (64u - bs)) : 0ull;
            w_[i] = hi | lo;
        }
    }
    for (unsigned i = lim; i < words_; ++i) w_[i] = 0;
}

void BitVec::random_init(Rng& rng) {
    for (unsigned j = 0; j < words_; ++j) w_[j] = rng.next_u64();
    clear_top();
}

unsigned BitVec::rem_bits() const { return nbits_ & 63u; }

void BitVec::clear_top() {
    unsigned rem = rem_bits();
    if (rem != 0u) w_[words_ - 1u] &= ((~0ull) >> (64u - rem));
}

void BitVec::check_same(const BitVec& o) const {
    assert(o.nbits_ == nbits_ && o.words_ == words_);
}
