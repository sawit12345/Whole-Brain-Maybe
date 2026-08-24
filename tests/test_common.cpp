#include <cassert>
#include <cstdint>
#include <cstdio>
#include "../src/common/bitvec.hpp"
#include "../src/common/rng.hpp"

static void test_roundtrip() {
    for (unsigned n : {1u, 63u, 64u, 65u, 100u, 127u, 128u, 129u, 2048u}) {
        BitVec v(n);
        assert(v.popcount() == 0);
        unsigned expect = 0;
        for (unsigned i = 0; i < n; i += 3) {
            v.set(i);
            ++expect;
        }
        assert(v.popcount() == expect);
        for (unsigned i = 0; i < n; ++i)
            assert(v.test(i) == (i % 3 == 0 && i < n));
        for (unsigned i = 0; i < n; i += 3) v.clear(i);
        assert(v.popcount() == 0);
    }
    printf("roundtrip set/clear/test: OK\n");
}

static void test_popcount_vs_builtin() {
    Rng rng(123456789ull);
    for (unsigned trial = 0; trial < 64; ++trial) {
        BitVec v(2048);
        v.random_init(rng);
        unsigned acc = 0;
        for (unsigned j = 0; j < v.word_count(); ++j)
            acc += __builtin_popcountll(v.data()[j]);
        assert(v.popcount() == acc);
    }
    printf("popcount vs __builtin_popcountll: OK\n");
}

static void test_logic_ops() {
    BitVec a(256), b(256);
    a.set(1); a.set(5); a.set(130);
    b.set(5); b.set(9); b.set(131);

    BitVec anb = a & b;
    assert(anb.test(5) && anb.popcount() == 1);

    BitVec aorb = a | b;
    assert(aorb.popcount() == 5);
    assert(aorb.test(1) && aorb.test(5) && aorb.test(9) &&
           aorb.test(130) && aorb.test(131));

    BitVec axb = a ^ b;
    assert(axb.popcount() == 4);
    assert(axb.test(1) && !axb.test(5) && axb.test(9));

    BitVec na = ~a;
    assert(na.popcount() == 256 - 3);
    for (unsigned i = 0; i < 256; ++i) assert(na.test(i) != a.test(i));

    a &= b;
    assert(a.popcount() == 1 && a.test(5));
    printf("and/or/xor/not + compound: OK\n");
}

static void test_shifts() {
    BitVec v(2048);
    v.set(10); v.shl_inplace(20);
    assert(v.popcount() == 1 && v.test(30));

    v.zero(); v.set(70); v.shl_inplace(30);
    assert(v.popcount() == 1 && v.test(100));

    v.zero(); v.set(1900); v.shl_inplace(100);
    assert(v.popcount() == 1 && v.test(2000));

    v.zero(); v.set(2000); v.shl_inplace(500);
    assert(v.popcount() == 0);

    v.zero(); v.set(100); v.shr_inplace(40);
    assert(v.popcount() == 1 && v.test(60));

    v.zero(); v.set(70); v.set(71); v.shr_inplace(8);
    assert(v.popcount() == 2 && v.test(62) && v.test(63));

    v.zero(); v.set(10); v.shr_inplace(11);
    assert(v.popcount() == 0);

    Rng rng(777ull);
    BitVec r(2048), ref(2048);
    r.random_init(rng);
    bool ref_bits[2048];
    for (unsigned i = 0; i < 2048; ++i) ref_bits[i] = r.test(i);
    r.shl_inplace(137);
    for (unsigned i = 0; i < 2048; ++i) {
        bool want = (i >= 137) && ref_bits[i - 137];
        assert(r.test(i) == want);
    }
    r.shr_inplace(137);
    for (unsigned i = 0; i < 2048 - 137; ++i) assert(r.test(i) == ref_bits[i]);
    for (unsigned i = 2048 - 137; i < 2048; ++i) assert(!r.test(i));
    printf("inplace shl/shr vs reference: OK\n");
}

static void test_rng_and_random_init() {
    Rng r1(42ull), r2(42ull);
    for (unsigned t = 0; t < 1000; ++t) assert(r1.next_u64() == r2.next_u64());

    Rng r3(7ull);
    bool seen[2048] = {};
    for (unsigned t = 0; t < 10000; ++t) {
        unsigned v = r3.fast_below(2048);
        assert(v < 2048);
        seen[v] = true;
    }
    unsigned buckets = 0;
    for (unsigned i = 0; i < 2048; ++i) buckets += seen[i];
    assert(buckets > 2000);
    Rng r3b(7ull);
    bool saw_nonzero = false;
    for (unsigned t = 1; t < 100; ++t)
        if (r3b.fast_below(2048) != 0) saw_nonzero = true;
    assert(saw_nonzero);
    assert(r3.fast_below(1) == 0);

    Rng ra(999ull), rb(999ull);
    BitVec a(2048), b(2048);
    a.random_init(ra);
    b.random_init(rb);
    for (unsigned i = 0; i < 2048; ++i) assert(a.test(i) == b.test(i));
    printf("rng determinism + random_init reproducibility: OK\n");
}

int main() {
    test_roundtrip();
    test_popcount_vs_builtin();
    test_logic_ops();
    test_shifts();
    test_rng_and_random_init();
    printf("test_common: ALL PASSED\n");
    return 0;
}
