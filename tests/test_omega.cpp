#include <cassert>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <algorithm>

#include "../src/omega/omega.hpp"

using namespace omega;

static bool code_eq(const Code& a, const Code& b) {
    return (a.pos ^ b.pos).popcount() == 0u &&
           (a.neg ^ b.neg).popcount() == 0u;
}

static void test_encode_weights() {
    Rng rng(1234567ull);
    for (unsigned trial = 0; trial < 300; ++trial) {
        Code c = encode(trial, rng);
        assert(c.pos.popcount() == K_PER_PLANE);
        assert(c.neg.popcount() == K_PER_PLANE);
        assert(invariant_ok(c));
    }
    Rng r1(42ull), r2(42ull);
    Code a = encode(7ull, r1);
    Code b = encode(7ull, r2);
    assert(code_eq(a, b));
    Rng r3(42ull);
    Code c1 = encode(1ull, r3);
    Code c2 = encode(2ull, r3);
    assert(!code_eq(c1, c2));
    printf("encode weights/invariant/reproducibility: OK\n");
}

static int32_t brute_sim(const Code& x, const Code& y) {
    int32_t s = 0;
    for (unsigned i = 0; i < N_BITS; ++i) {
        int vx = (x.pos.test(i) ? 1 : 0) - (x.neg.test(i) ? 1 : 0);
        int vy = (y.pos.test(i) ? 1 : 0) - (y.neg.test(i) ? 1 : 0);
        s += vx * vy;
    }
    return s;
}

static void test_sim() {
    Rng rng(777777ull);
    for (unsigned trial = 0; trial < 100; ++trial) {
        Code a = encode(trial * 2ull, rng);
        Code b = encode(trial * 2ull + 1ull, rng);
        int32_t sab = static_cast<int32_t>(sim(a, b));
        int32_t sba = static_cast<int32_t>(sim(b, a));
        assert(sab == sba);
        assert(sab == brute_sim(a, b));
        assert(static_cast<int32_t>(sim(a, a)) ==
               static_cast<int32_t>(K_PER_PLANE + K_PER_PLANE));
    }
    printf("sim symmetry/self/brute-force agreement: OK\n");
}

static Code manual_code(const std::vector<unsigned>& pos_idx,
                        const std::vector<unsigned>& neg_idx) {
    Code c;
    for (unsigned i : pos_idx) c.pos.set(i);
    for (unsigned i : neg_idx) c.neg.set(i);
    return c;
}

static void test_bind_unbind_roundtrip() {
    Rng rng(555ull);
    for (unsigned trial = 0; trial < 100; ++trial) {
        Code x = encode(trial * 3ull, rng);
        Code r = encode(trial * 3ull + 1ull, rng);
        Code y = bind(x, r);
        Code z = unbind(y, r);
        assert(code_eq(z, x));
        Code zz = unbind(bind(z, r), r);
        assert(code_eq(zz, z));
    }
    std::vector<unsigned> p64, n63, p63, n64;
    for (unsigned i = 0; i < 64; ++i) p64.push_back(i * 31u + 5u);
    for (unsigned i = 0; i < 63; ++i) n63.push_back(i * 19u + 700u);
    for (unsigned i = 0; i < 63; ++i) p63.push_back(i * 29u + 150u);
    for (unsigned i = 0; i < 64; ++i) n64.push_back(i * 19u + 800u);
    Code role_rho0 = manual_code(p64, n63);
    Code role_rho63 = manual_code(p63, n64);
    assert(role_rho0.pos.popcount() % 64u == 0u);
    assert(role_rho0.neg.popcount() % 64u == 63u);
    for (unsigned trial = 0; trial < 10; ++trial) {
        Code x = encode(trial * 5ull + 900ull, rng);
        assert(code_eq(unbind(bind(x, role_rho0), role_rho0), x));
        assert(code_eq(unbind(bind(x, role_rho63), role_rho63), x));
    }
    printf("bind/unbind roundtrip incl rho=0 and rho=63 edges: OK\n");
}

static void test_bdd_memory() {
    BddMemory mem(64);
    for (uint64_t s = 500; s < 550; ++s) assert(mem.store(s));
    assert(!mem.store(501));
    mem.set_threshold(24);
    uint64_t out = 0;
    unsigned hits = 0;
    for (uint64_t s = 500; s < 550; ++s) {
        for (unsigned t = 0; t < BddMemory::BRANCHES; ++t) {
            Rng br(branch_seed(s, t));
            Code q = encode(s, br);
            bool ok = mem.recall(q, out);
            assert(ok);
            assert(out == s);
            ++hits;
        }
    }
    assert(hits == 150u);
    Rng rr(424242ull);
    Code stray = encode(987654321ull, rr);
    assert(!mem.recall(stray, out));
    for (int i = 0; i < 10; ++i) mem.strengthen(500);
    assert(mem.conf(500, 0) == 3);
    assert(mem.conf(500, 1) == 3);
    for (int i = 0; i < 10; ++i) mem.decay_all();
    assert(mem.conf(500, 0) == 0);
    assert(mem.conf(500, 2) == 0);
    BddMemory::Stats st = mem.stats();
    assert(st.capacity == 64u);
    assert(st.slots_used == 50u);
    assert(st.recalls == 151u);
    assert(st.hits == 150u);
    assert(st.misses == 1u);
    assert(st.threshold == 24);
    printf("BDD memory store/recall/stats/counters: OK (noiseless 150/150)\n");
}

struct RScore {
    uint32_t s;
    unsigned idx;
};

static void test_router() {
    Rng rng(31415926ull);
    const unsigned A = 16;
    const unsigned KK = 4;
    Router rt(A, KK);
    for (unsigned i = 0; i < A; ++i)
        for (unsigned j = 0; j < A; ++j)
            rt.set_gate(i, j, (rng.next_u64() & 1ull) != 0ull);
    for (unsigned i = 0; i < A; ++i)
        for (unsigned j = 0; j < A; ++j)
            assert(rt.gate(i, j) ==
                   (((rt.gate_word(i) >> (j & 63u)) & 1ull) != 0ull));
    for (unsigned trial = 0; trial < 200; ++trial) {
        for (unsigned i = 0; i < A; ++i)
            for (unsigned j = 0; j < A; ++j)
                rt.set_gate(i, j, (rng.next_u64() & 1ull) != 0ull);
        Code x = encode(trial + 80000ull, rng);
        unsigned area[KK];
        uint32_t score[KK];
        unsigned got = rt.route(x, area, score);
        assert(got == KK);
        RScore brute[A];
        const uint64_t probe = x.pos.data()[0];
        for (unsigned i = 0; i < A; ++i) {
            brute[i].s =
                static_cast<uint32_t>(__builtin_popcountll(rt.gate_word(i) &
                                                           probe));
            brute[i].idx = i;
        }
        std::sort(brute, brute + A, [](const RScore& a, const RScore& b) {
            return a.s != b.s ? a.s > b.s : a.idx < b.idx;
        });
        std::sort(brute, brute + KK, [](const RScore& a, const RScore& b) {
            return a.s != b.s ? a.s < b.s : a.idx < b.idx;
        });
        for (unsigned t = 0; t < KK; ++t) {
            assert(area[t] == brute[t].idx);
            assert(score[t] == brute[t].s);
        }
    }
    Router tiny(1, 1);
    tiny.set_gate(0, 0, true);
    unsigned ta[1];
    uint32_t ts[1];
    assert(tiny.route(Code{}, ta, ts) == 1u);
    assert(ta[0] == 0u && ts[0] == 0u);
    printf("router top-k matches brute force: OK\n");
}

static void test_grid_clock() {
    assert(GridClock::COMPOSITE_PERIOD == 61ull * 127ull * 251ull);
    GridClock ck;
    uint64_t cycles = 0, c0 = 0, c1 = 0, c2 = 0;
    for (uint64_t t = 0; t < GridClock::COMPOSITE_PERIOD; ++t) {
        ck.tick();
        if (ck.cycle()) ++cycles;
        if (ck.phase(0)) ++c0;
        if (ck.phase(1)) ++c1;
        if (ck.phase(2)) ++c2;
    }
    assert(cycles == 1ull);
    assert(c0 == GridClock::COMPOSITE_PERIOD / 61ull);
    assert(c1 == GridClock::COMPOSITE_PERIOD / 127ull);
    assert(c2 == GridClock::COMPOSITE_PERIOD / 251ull);
    for (uint64_t t = 0; t < GridClock::COMPOSITE_PERIOD; ++t) {
        ck.tick();
        if (ck.cycle()) ++cycles;
    }
    assert(cycles == 2ull);
    printf("grid clock full-cycle count == 1944497: OK (cycles=%llu)\n",
           static_cast<unsigned long long>(cycles));
}

static void test_cerebellum() {
    Cerebellum cb;
    Rng rng(20260824ull);
    const unsigned NP = 64;
    std::vector<Code> pats;
    for (unsigned i = 0; i < NP; ++i) pats.push_back(encode(i + 7000ull, rng));

    BitVec ex(Cerebellum::EXP_BITS);
    BitVec ex2(Cerebellum::EXP_BITS);
    cb.recode(pats[0], ex);
    unsigned dens = ex.popcount();
    assert(dens > 1024u && dens < Cerebellum::EXP_BITS - 1024u);
    cb.recode(pats[0], ex2);
    assert((ex ^ ex2).popcount() == 0u);

    std::vector<int> tw(Cerebellum::EXP_BITS);
    std::vector<int> labs(NP);
    uint64_t salt = 1;
    for (;; ++salt) {
        Rng tr(salt * 7919ull + 13ull);
        for (unsigned j = 0; j < Cerebellum::EXP_BITS; ++j)
            tw[j] = static_cast<int>(tr.next_u64() % 17ull) - 8;
        int pos_n = 0;
        for (unsigned i = 0; i < NP; ++i) {
            cb.recode(pats[i], ex);
            const uint64_t* px = ex.data();
            int32_t sum = 0;
            for (unsigned w = 0; w < (Cerebellum::EXP_BITS >> 6); ++w) {
                uint64_t wv = px[w];
                while (wv != 0ull) {
                    unsigned b = static_cast<unsigned>(__builtin_ctzll(wv));
                    sum += tw[(static_cast<size_t>(w) << 6) + b];
                    wv &= wv - 1ull;
                }
            }
            labs[i] = (sum >= 0) ? 1 : -1;
            if (labs[i] > 0) ++pos_n;
        }
        if (pos_n > 4 && pos_n < static_cast<int>(NP) - 4) break;
    }

    for (unsigned step = 0; step < 1000; ++step)
        cb.train(pats[step % NP], labs[step % NP]);

    unsigned correct = 0;
    for (unsigned i = 0; i < NP; ++i)
        if (cb.predict(pats[i]) == labs[i]) ++correct;
    printf("cerebellum separable task accuracy: %u/%u after 1000 steps "
           "(salt=%llu)\n",
           correct, NP, static_cast<unsigned long long>(salt));
    assert(correct * 20u >= NP * 19u);

    for (unsigned j = 0; j < Cerebellum::EXP_BITS; ++j) {
        int8_t v = cb.weight(j);
        assert(v >= -128 && v <= 127);
    }
    printf("cerebellum expansion recode + perceptron readout: OK\n");
}

static void test_selector() {
    Selector sel(16);
    Rng rng(88883ull);
    for (unsigned trial = 0; trial < 100; ++trial) {
        uint32_t chosen[16];
        unsigned filled = 0;
        while (filled < 16) {
            uint32_t v = static_cast<uint32_t>(rng.next_u64());
            bool dup = false;
            for (unsigned i = 0; i < filled; ++i) dup = dup || chosen[i] == v;
            if (dup) continue;
            chosen[filled] = v;
            sel.set_score(filled, v);
            ++filled;
        }
        unsigned best = 0;
        for (unsigned i = 1; i < 16; ++i)
            if (chosen[i] > chosen[best]) best = i;
        assert(sel.select() == best);
    }
    assert(!sel.plasticity_on());
    sel.set_plasticity(true);
    assert(sel.plasticity_on());
    Selector ties(4);
    for (unsigned i = 0; i < 4; ++i) ties.set_score(i, 5u);
    assert(ties.select() == 0u);
    printf("selector argmax WTA 100/100 + plasticity gate: OK\n");
}

static void test_modbus() {
    ModBus bus;
    int thr = 0;
    bus.apply(thr);
    assert(thr == 0);
    bus.da = 64;
    bus.apply(thr);
    assert(thr == 8);
    bus.da = 0;
    bus.ach = 64;
    bus.apply(thr);
    assert(thr == 4);
    bus.ach = 0;
    bus.ne = 32;
    bus.apply(thr);
    assert(thr == 5);
    bus.ne = 0;
    bus.s5ht = 64;
    bus.apply(thr);
    assert(thr == 4);
    ModBus neg;
    neg.da = -128;
    neg.apply(thr);
    assert(thr == -12);
    ModBus hot;
    hot.da = 127;
    hot.ne = 127;
    thr = 120;
    hot.apply(thr);
    assert(thr == 127);
    ModBus cold;
    cold.da = -128;
    thr = -120;
    cold.apply(thr);
    assert(thr == -128);
    printf("modbus shift-only threshold modulation + clamps: OK\n");
}

int main() {
    test_encode_weights();
    test_sim();
    test_bind_unbind_roundtrip();
    test_bdd_memory();
    test_router();
    test_grid_clock();
    test_cerebellum();
    test_selector();
    test_modbus();
    printf("ALL OMEGA TESTS PASSED\n");
    return 0;
}
