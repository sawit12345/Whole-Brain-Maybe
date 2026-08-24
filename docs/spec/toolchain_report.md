# ΩDR Toolchain Report — agent D (toolchain), 2026-08-24

## Versions (exact)

| Component | Version |
|---|---|
| g++ | 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04.1), linux x86_64 |
| Lean 4 | 4.33.1, x86_64-unknown-linux-gnu, commit `819816b2e0a3bf405af45ae5c7af2491d8f5bee6`, Release (elan stable channel) |
| Lake | 5.0.0-src+819816b (Lean version 4.33.1) |
| elan installer | `curl https://elan.lean-lang.org/elan-init.sh -sSfL \| sh -s -- -y --default-toolchain stable` — succeeded on first try; no release-binary fallback needed |

Lean lives in user space at `$HOME/.elan`. For every shell:
`export PATH="$HOME/.elan/bin:$PATH"`.

## Rebuild commands

```sh
# Lean project
export PATH="$HOME/.elan/bin:$PATH"
cd lean/OmegaDR && lake build

# C++ scaffold
make clean && make all    # build test_common, test_sdr, bench_sdr
make test                 # build + run both suites
make bench                # run benchmark
```

Makefile flags: `-O3 -std=c++20 -Wall -Wextra -mpopcnt`. Zero warnings.

## Lean sanity proof

`lean/OmegaDR/OmegaDR/Basic.lean`: `namespace OmegaDR ... example : 2 + 2 = 4 := rfl`.
No Mathlib by design; instructions for adding it later: `lean/OmegaDR/SETUP.md`.

```
$ lake build
info: OmegaDR: no previous manifest, creating one from scratch
info: toolchain not updated; no toolchain information found
✔ [2/4] Built OmegaDR.Basic (277ms)
✔ [3/4] Built OmegaDR (246ms)
Build completed successfully (4 jobs).
```

## C++ test output (real transcript)

```
$ make test
./build/test_common
roundtrip set/clear/test: OK
popcount vs __builtin_popcountll: OK
and/or/xor/not + compound: OK
inplace shl/shr vs reference: OK
rng determinism + random_init reproducibility: OK
test_common: ALL PASSED
./build/test_sdr
encode_random: exactly w=40 ones among n=2048: OK
overlap symmetry + exact small case: OK
clean decode recovers stored pattern (16/16): OK
noisy decode (<=8 flips): 100/100 trials (threshold 95): OK
test_sdr: ALL PASSED
```

Noisy-decode requirement "≤8 flips across ≥95/100 seeded trials": measured 100/100
(seeds 1000000+t, t∈[0,100); flips = 1 + rng.fast_below(8)).

## Benchmark (informational, single run)

```
bench_sdr: n=2048 w=40 pool=1024
overlaps:      1000000 ops |     46.9 ns/op |      21.31 Mops/s
nearest-decode: 10000 ops |  42717.3 ns/op |     0.0234 Mops/s
decode hit rate: 10000/10000 | sink=32710
```

## Kernel compliance: zero mult / div / float in src/**

Grep transcript over `src/**` (`bitvec.hpp/cpp`, `rng.hpp`, `sdr.hpp/cpp`):

```
=== A: every * in src/** ===
src/sdr_baseline/sdr.hpp:11:    static int nearest_decode(const BitVec& query, const BitVec* stored,
src/sdr_baseline/sdr.cpp:18:int SDR::nearest_decode(const BitVec& query, const BitVec* stored,
src/common/bitvec.cpp:70:    return *this;
src/common/bitvec.cpp:76:    return *this;
src/common/bitvec.cpp:82:    return *this;
src/common/bitvec.hpp:12:    const uint64_t* data() const { return w_.data(); }

=== B: spaced arithmetic mult pattern ===
(no matches)

=== C: *= compound mult ===
(no matches)

=== D: every / in src/** ===
src/sdr_baseline/sdr.hpp:2:#include "../common/bitvec.hpp"
src/sdr_baseline/sdr.hpp:3:#include "../common/rng.hpp"

=== E: every % in src/** ===
(no matches)

=== F: float/double keywords ===
(no matches)

=== G: div-by-pattern i/64 or i%64 ===
(no matches)
```

Classification of every hit:
- A hits 1–2, 6: pointer **declarators** (`const BitVec* stored`,
  `const uint64_t* data()`), not arithmetic.
- A hits 3–5: `return *this;` reference dereference in compound-assign
  operators, not multiplication.
- D hits: `#include` relative paths only.
- B/C/E/F/G: zero arithmetic multiplication, zero division, zero modulo,
  zero floating point.

Index math uses `i >> 6` and `i & 63u` exclusively (see
`src/common/bitvec.cpp`). Shift masks derived with shift-doubling
(`m = (m<<1)|1`), so even RNG setup is division-free.

## Notes / deviations

1. Charter suggests `-march=native`; task spec fixed `-mpopcnt`. Used the task's
   flag set verbatim for portability; `-march=native` can be added per-machine.
2. `noise_corrupt(pattern, num_flips)` and `encode_random(out)` take an extra
   `Rng&` parameter — determinism (charter V3) requires an explicit seeded
   source rather than hidden global state.
3. `fast_below(n)` computes its rejection mask via shift-doubling instead of
   modulo-at-init; strictly stronger than the permitted design (no `%` anywhere,
   even at init). Mask is cached per bound ("computed once").
4. Two initial test-expectation bugs (shift roundtrip losing top bits; out-of-
   range bit index) and one real bug (fast_below not advancing state between
   accepted draws → constant output) were found and fixed during bring-up; the
   RNG regression is now covered by a bucket-coverage assertion (>2000 distinct
   values in 10000 draws).
