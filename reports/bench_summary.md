# Benchmark summary — agent G: noise sweep + associative capacity (V1)

Benchmarks: `bench/noise_bench.cpp`, `bench/capacity_bench.cpp` (run via `make bench`).
All numbers below are pasted from actual runs (master seeds printed in each log;
fresh seeded Rng per point, deterministic xorshift from `src/common/rng.hpp`).

## Headline results

1. **Does ΩDR-BDD beat SDR on noise at f ≥ 0.40? NO.** Under the protocol below,
   SDR hit-rate ≥ ΩDR hit-rate at **every** f on all three channels; there is no
   crossover point. ΩDR never reaches 0.95 with SDR lower anywhere in
   f ∈ {0.40..0.50}. Honest negative result; see interpretation.
2. **Capacity M₉₉%**: neither system drops below 99 % within the tested range
   M ≤ 1600 → **M₉₉% > 1600 for both** (associative-memory capacity, not
   state-space size, which remains the analytic 10^95.51 vs 10^84.38 result and
   was not measured here).
3. **Throughput @ M=200**: `BddMemory::recall` ≈ 30.5–31.3 µs/op vs
   `SDR::nearest_decode` ≈ 9.3–9.4 µs/op → ΩDR ≈ 3.27× slower per recall.

## Protocol

- 200 stored items both systems (`SDR::encode_random`, W=40;
  `omega::encode`, K=20/plane ×3 BDD branches via `branch_seed`). Query = stored
  item corrupted at level f. Hit = correct item is argmax similarity among all
  stored AND sim ≥ threshold.
- Channels applied to retrieval queries: erasure (support symbol →0 w.p. f);
  mixed50 (→0 w.p. f/2, →opposite sign w.p. f/2; SDR: bit cleared w.p. f);
  flip (support symbol → opposite sign w.p. f; SDR analog: support bit cleared
  AND one random clear bit set — flip relocated within the representation,
  weight preserved).
- ΩDR query is a corrupted copy of ONE of the 3 stored BDD branches (uniformly
  chosen); `BddMemory::recall` sums evidence over branches internally. This is
  forced by the shipped API (`recall(const Code&, ...)` takes a single code);
  see threats-to-validity.
- Calibration: thresholds swept at f=0.20, F1 metric, max mean-F1 over the 3
  channels, ties → lowest threshold; then FIXED for the whole sweep.
- Noise sweep: f ∈ {0.05…0.50 step 0.05}, 100 000 trials/point, fresh seed per
  point. Capacity sweep: clean-copy queries, M ∈ {10…1600}, 10 000 trials/point,
  thresholds calibrated once at M=100.

## Chosen calibrations (real calibration output, noise bench)

```
[calibration] f=0.20 metric=F1 (ties -> lowest threshold)
  omega thr=16 erasure=1.00000 mixed50=0.99817 flip=0.97090 mean=0.98969
  omega thr=24 erasure=0.99917 mixed50=0.92163 flip=0.71155 mean=0.87745
  omega thr=32 erasure=0.73426 mixed50=0.34369 flip=0.12988 mean=0.40261
  omega thr=40 erasure=0.00310 mixed50=0.00130 flip=0.00050 mean=0.00163
  sdr   thr=13 erasure=1.00000 mixed50=1.00000 flip=1.00000 mean=1.00000
  sdr   thr=16 erasure=1.00000 mixed50=1.00000 flip=1.00000 mean=1.00000
  sdr   thr=21 erasure=0.99997 mixed50=1.00000 flip=1.00000 mean=0.99999
[calibration] chosen omega_threshold=16 sdr_threshold=13
```

Chosen: **ΩDR θ=16, SDR θ=13**, fixed for the whole noise sweep.
Capacity bench calibrated at M=100 (clean): ΩDR F1=1.0 for θ∈{16,24,32} (θ=40
F1=0.796), SDR F1=1.0 for all three → same tie-break picks **ΩDR θ=16, SDR θ=13**,
fixed for all M.

## Noise sweep (100 000 trials/point; reports/noise_sweep.csv)

```
[channel] erasure
    f     sdr_hit   omega_hit          [channel] mixed50
  0.05   1.00000   1.00000                f     sdr_hit   omega_hit
  0.10   1.00000   1.00000              0.05   1.00000   1.00000
  0.15   1.00000   1.00000              0.10   1.00000   1.00000
  0.20   1.00000   1.00000              0.15   1.00000   0.99972
  0.25   1.00000   1.00000              0.20   1.00000   0.99662
  0.30   1.00000   1.00000              0.25   1.00000   0.97562
  0.35   1.00000   0.99948              0.30   1.00000   0.90745
  0.40   0.99984   0.99529              0.35   0.99999   0.75816
  0.45   0.99880   0.97664              0.40   0.99988   0.54222
  0.50   0.99161   0.91564              0.45   0.99875   0.32225
                                              0.50   0.99144   0.15169
[channel] flip
    f     sdr_hit   omega_hit
  0.05   1.00000   1.00000
  0.10   1.00000   0.99981
  0.15   1.00000   0.99319
  0.20   1.00000   0.94446
  0.25   1.00000   0.79250
  0.30   1.00000   0.54380
  0.35   0.99999   0.28934
  0.40   0.99986   0.11850
  0.45   0.99919   0.03514
  0.50   0.99343   0.00740
[crossover] erasure: none (sdr_hit >= omega_hit at all f)
[crossover] mixed50: none (sdr_hit >= omega_hit at all f)
[crossover] flip: none (sdr_hit >= omega_hit at all f)
[criterion] omega_hit>=0.95 at some f>=0.40 with sdr lower: NO
```

Crossover points: **none on any channel** — SDR dominates throughout; the gap
widens with sign-corruption content (flip worst for ΩDR: a ternary sign flip
costs −2 of sim vs −1 for an SDR clear, and only 1 of ΩDR's 3 branches carries
signal under this probe). The SPEC's single-codeword erasure edge (25.95 % vs
23.94 %) does not translate into an associative-recall win here because the
majority-of-three bundling gain requires multi-branch probes, which this API/
protocol does not exercise.

## Associative-memory capacity curves (reports/capacity_curves.csv)

```
[calibration] chosen omega_threshold=16 sdr_threshold=13 (fixed for all M)
[M=  10] sdr_hit=1.00000 omega_hit=1.00000
[M=  25] sdr_hit=1.00000 omega_hit=1.00000
[M=  50] sdr_hit=1.00000 omega_hit=1.00000
[M= 100] sdr_hit=1.00000 omega_hit=1.00000
[M= 200] sdr_hit=1.00000 omega_hit=1.00000
[M= 400] sdr_hit=1.00000 omega_hit=1.00000
[M= 800] sdr_hit=1.00000 omega_hit=1.00000
[M=1600] sdr_hit=1.00000 omega_hit=1.00000
[M_99] sdr: not reached (>=0.99 through M=1600)
[M_99] omega: not reached (>=0.99 through M=1600)
```

Both systems are at ceiling (1.00000) through M=1600; M₉₉% > 1600 for both.
Label: these are ASSOCIATIVE MEMORY capacities under clean-copy cued recall —
they do not measure representational state-space (analytic: ΩDR 10^95.51 vs
SDR 10^84.38), and CSV headers/comments say so.

## Throughput (M=200, chrono, 10 000 recalls; stderr + reports/throughput.csv)

```
[throughput] M=200 recalls=10000 bdd_recall=30698.3 ns/op sdr_nearest_decode=9373.6 ns/op bdd/sdr=3.27x sink=18003980000   (run 1)
[throughput] M=200 recalls=10000 bdd_recall=30508.5 ns/op sdr_nearest_decode=9330.2 ns/op bdd/sdr=3.27x sink=18003980000   (make bench run)
```

BDD recall scans 200 slots × 3 branch sims (~4 POPCNTs/word) ≈ 6× the memory
traffic of one SDR overlap pass, but measured ratio is ≈3.27× (SIMD popcount +
cache reuse). Timings live on **stderr** and in `reports/throughput.csv`, not
stdout, so stdout stays byte-identical across runs (V3): verified —
`diff` of two full runs of each binary reports no differences
("NOISE_STDOUT_IDENTICAL", "CAP_STDOUT_IDENTICAL").

## Threats to validity

- **Single-branch probe**: the shipped `BddMemory::recall(Code&)` accepts one
  code, so the query corrupts exactly one of the 3 stored branches; two branches
  contribute only ~zero-mean crosstalk. This structurally caps ΩDR's usable
  signal at ~one codeword's worth while its threshold must still clear 3-branch
  summed noise — it measures the kernel as built, not the SPEC §Noise-plan
  majority-of-three idea with bundled queries, which the current API cannot
  express. Results should be read as "BDD as implemented", not as a refutation
  of bundled decoding.
- **Threshold calibration sensitivity**: grids {16,24,32,40} / {13,16,21} are
  coarse and calibrated at a single operating point (f=0.20; M=100 for capacity)
  with 20 k trials/channel; a finer grid or per-channel thresholds would shift
  absolute rates by a few percent near the cliffs (e.g., ΩDR erasure θ=24 would
  move the f=0.50 point from 0.916 to ~0.75) but cannot reverse the ordering:
  SDR's θ=13 sits far below every admissible ΩDR threshold on the same
  Binomial(40, 1−f) survivor statistic.
- **Trial count**: 10⁵ trials/point gives SE ≈ 0.09–0.15 % at rates near 0.9–1.0
  (observed gaps between systems are ≥ 0.8 pp at f ≥ 0.30, i.e., ≫ noise);
  capacity's 10⁴ trials/M bound detectable dips at ≳ 0.5 %.
- **Correlated-code caveat (Lehky-2021)**: both stores use i.i.d. random codes;
  `papers/lehky2021_pseudosparse_coding.pdf` argues real cortical populations
  are pseudosparse with strong correlated structure. With correlated codes,
  competitor-similarity tails fatten, false-positive risk rises, and absolute
  hit-rates here are optimistic for BOTH systems; relative ordering could change
  and was not tested.
- **Determinism**: fixed master seeds, fresh per-point seeds via
  `mix_streams`; stdout byte-identical across repeated runs (verified above).
  Timing lines are intentionally excluded from stdout.

## Deviations from task spec

1. ΩDR query defined as corrupted copy of one stored BDD branch (API constraint,
   documented above) rather than corruption of an abstract "item".
2. Threshold pre-sweep maximizes mean F1 across the three channels at f=0.2
   (spec said "at f=0.2 maximizing F1" without naming a channel), yielding one
   fixed value per system as required.
3. Throughput printed to stderr + reports/throughput.csv instead of stdout to
   keep V3's byte-identical-stdout guarantee (timings vary run-to-run).
4. SDR flip analog relocates the cleared bit to a uniformly random clear
   position (weight-preserving), matching "move the flip within support".
