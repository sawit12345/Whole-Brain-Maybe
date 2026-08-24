# Benchmark summary — agent H3: chunked-BDD (pooled) noise sweep + capacity (V4)

Benchmarks: `bench/noise_bench.cpp`, `bench/capacity_bench.cpp` (run via `make bench`).
All numbers below are pasted from actual runs (master seeds printed in each log;
fresh seeded Rng per point, deterministic xorshift from `src/common/rng.hpp`).

This run supersedes the V1/G result below (kept verbatim-in-summary under
"Superseded run (legacy BDD)"). The redesign tested here is **chunked BDD**:
content-true redundancy instead of independent branches.

## Headline results

1. **Does ΩDR-pooled beat SDR on noise at f ≥ 0.40? NO — still.** Under the
   protocol below, SDR hit-rate ≥ ΩDR-pooled hit-rate at **every** f on all
   three channels; there is no crossover. ΩDR-pooled clears 0.95 at f ≥ 0.40
   only on erasure (0.99654 @ 0.40) but SDR is higher there (0.99984), so the
   success criterion fails on every channel. Honest negative again; details
   and interpretation below.
2. **But the redesign is a large real gain over legacy BDD** at every
   operating point measured, e.g. flip f=0.20: 0.95893 pooled vs 0.55527
   legacy-at-fixed-θ=24 and 0.94446 legacy-at-θ=16 (superseded run);
   erasure f=0.40: 0.99654 vs 0.56124 (θ=24). Pooled also dominates every
   point of the superseded θ=16 sweep (see §Superseded).
3. **Capacity M₉₉%**: neither system drops below 99 % within M ≤ 1600 →
   **M₉₉% > 1600 for both** (associative-memory capacity, not state-space
   size; analytic 10^95.51 vs 10^84.38 stands untouched).
4. **Throughput @ M=200**: `recall_pooled` ≈ 10.8–11.1 µs/op vs
   `recall_legacy` ≈ 27.4–28.1 µs/op vs `SDR::nearest_decode` ≈ 8.4–8.5 µs/op
   → pooled is **2.6× faster than legacy BDD** and now only **≈1.3× slower
   than SDR** (was 3.27×): a pooled scan reads one base code (32 words), not
   three independent branches.

## Redesign under test (chunked BDD)

- Item content = ONE base code c (`omega::encode`, N=2048, K=20/plane),
  stored per slot and generated from `item_seed(stream_id)` =
  `mix_streams(stream_id, 0x1D5EED51D5EED5B7)`.
- m=3 CHUNK VIEWS of the same c via word-aligned boundaries
  `CHUNK_WORD_OFF = {0,10,21,32}`: chunk sizes **640/704/704 bits =
  10/11/11 words** (total 2048). Word-alignment was preferred over the naive
  682/682/684 split so the hot loop needs no sub-word masks; boundaries are
  shifts/masks only.
- Pooled score: `pooled_sim(q,c) = Σ_t max(0, sim_chunk_t(q_view_t, c_view_t))`
  using the signed 4-POPCNT overlap restricted to each chunk's word range.
  A chunk destroyed by wrong-sign corruption contributes 0, never negative —
  this is the redundancy mechanism: corruption hits chunks i.i.d., so
  simultaneous failure of 2-of-3 chunks is a squared-probability event,
  while legacy summed raw sims over unrelated branches.
- API: `recall()` now = pooled (stable name); `recall_pooled()` explicit
  alias; `recall_legacy(q, out, θ)` preserves pre-redesign behavior
  (sum of 3 whole-code sims over independently-seeded branches) purely for
  the A/B columns. Multiply/divide/float-free preserved in src/omega
  (compliance grep: pointer/include-path hits only, zero `%`).

## Protocol

- 200 stored items both systems (`SDR::encode_random`, W=40; ΩDR base +
  legacy branches per above). Query = stored item corrupted at level f.
  Hit = correct item is argmax similarity AND sim ≥ threshold.
- Channels applied to retrieval queries: erasure (support symbol →0 w.p. f);
  mixed50 (→0 w.p. f/2, →opposite sign w.p. f/2; SDR: bit cleared w.p. f);
  flip (support symbol →opposite sign w.p. f; SDR analog: support bit
  cleared AND one random clear bit set — weight-preserving relocation).
- **Probe model change vs V1**: the ΩDR query is a corrupted copy of the
  stored BASE CODE (the item itself), so every chunk carries signal — the
  redesign's intended use. V1 was forced to corrupt one of three unrelated
  branches (API limitation, see Superseded). Legacy arm keeps the V1
  uniform-branch probe for exact A/B comparability.
- Calibration ONCE at f=0.20 (20 k trials/channel/θ, F1 metric, max mean-F1
  over the 3 channels, ties → lowest threshold): ΩDR-pooled from {16,24,32};
  SDR from {13,16,21}; legacy FIXED θ=24 (not calibrated, per protocol).
  Then frozen for the whole sweep.
- Noise sweep: f ∈ {0.05…0.50 step 0.05}, 100 000 trials/point, fresh seed
  per point (`point_seed`, identical seed shared across the three arms).
- Capacity: clean-copy queries (base code), M ∈ {10…1600}, 10 000
  trials/point, thresholds calibrated once at M=100.

## Chosen calibrations (real calibration output, noise bench)

```
[calibration] f=0.20 metric=F1 (ties -> lowest threshold)
  omega_pooled thr=16 erasure=1.00000 mixed50=0.99862 flip=0.97912 mean=0.99258
  omega_pooled thr=24 erasure=0.99940 mixed50=0.92586 flip=0.74470 mean=0.88999
  omega_pooled thr=32 erasure=0.74710 mixed50=0.33534 flip=0.13953 mean=0.40733
  sdr   thr=13 erasure=1.00000 mixed50=1.00000 flip=1.00000 mean=1.00000
  sdr   thr=16 erasure=1.00000 mixed50=1.00000 flip=1.00000 mean=1.00000
  sdr   thr=21 erasure=0.99997 mixed50=1.00000 flip=1.00000 mean=0.99999
  legacy  thr=24 erasure=0.99917 mixed50=0.92163 flip=0.71155 mean=0.87745 (fixed, not calibrated)
[calibration] chosen omega_pooled_threshold=16 sdr_threshold=13 legacy_threshold=24
```

Chosen: **ΩDR-pooled θ=16, SDR θ=13, legacy θ=24 (fixed)**. Capacity bench
calibrated at M=100 (clean): F1=1.0 for all ΩDR θ∈{16,24,32} and all SDR θ →
tie-break picks **ΩDR θ=16, SDR θ=13**, fixed for all M.

## Noise sweep (100 000 trials/point; reports/noise_sweep.csv)

```
[channel] erasure
    f     sdr_hit   omega_hit   omega_legacy_hit
  0.05   1.00000   1.00000   1.00000
  0.10   1.00000   1.00000   1.00000
  0.15   1.00000   1.00000   0.99995
  0.20   1.00000   1.00000   0.99808
  0.25   1.00000   1.00000   0.98392
  0.30   1.00000   0.99997   0.92645
  0.35   1.00000   0.99965   0.78407
  0.40   0.99984   0.99654   0.56124
  0.45   0.99880   0.98090   0.32282
  0.50   0.99161   0.92320   0.14070
[channel] mixed50
    f     sdr_hit   omega_hit   omega_legacy_hit
  0.05   1.00000   1.00000   0.99999
  0.10   1.00000   1.00000   0.99721
  0.15   1.00000   0.99980   0.96716
  0.20   1.00000   0.99735   0.85481
  0.25   1.00000   0.97892   0.63603
  0.30   1.00000   0.91601   0.38969
  0.35   0.99999   0.76917   0.18353
  0.40   0.99988   0.55332   0.06799
  0.45   0.99875   0.32579   0.02097
  0.50   0.99144   0.15037   0.00426
[channel] flip
    f     sdr_hit   omega_hit   omega_legacy_hit
  0.05   1.00000   1.00000   0.99956
  0.10   1.00000   0.99989   0.97523
  0.15   1.00000   0.99595   0.83241
  0.20   1.00000   0.95893   0.55527
  0.25   1.00000   0.82718   0.27410
  0.30   1.00000   0.58307   0.10193
  0.35   0.99999   0.32211   0.02730
  0.40   0.99986   0.13577   0.00623
  0.45   0.99919   0.04158   0.00080
  0.50   0.99343   0.00860   0.00007
[crossover] erasure: none (sdr_hit >= omega_pooled_hit at all f)
[crossover] mixed50: none (sdr_hit >= omega_pooled_hit at all f)
[crossover] flip: none (sdr_hit >= omega_pooled_hit at all f)
[criterion] erasure: omega_pooled>=0.95 at some f>=0.40 with sdr lower: NO
[criterion] mixed50: omega_pooled>=0.95 at some f>=0.40 with sdr lower: NO
[criterion] flip: omega_pooled>=0.95 at some f>=0.40 with sdr lower: NO
[criterion] any channel meets criterion: NO
```

### Crossover table summary

| channel | crossover vs SDR | ΩDR-pooled best point ≥0.95 at f≥0.40 | SDR there | criterion |
|---|---|---|---|---|
| erasure | none | f=0.40 (0.99654); also ≥0.95 at 0.45 (0.98090) | 0.99984 / 0.99880 | **NO** (SDR never lower) |
| mixed50 | none | none (max 0.55332 at f=0.40) | 0.99988 | **NO** |
| flip | none | none (max 0.13577 at f=0.40) | 0.99986 | **NO** |

### Why pooling beats legacy but still loses to SDR (honest reading)

- **Erasure**: with no wrong-sign symbols the clamp never fires; the pooled
  score IS the survivor count ~Bin(40, 1−f) — statistically the same
  statistic SDR decodes. With equal statistics, the higher admissible
  threshold (16 vs 13, set by ΩDR's false-match budget) decides, and it
  costs ΩDR ~0.3 pp at f=0.40 and ~1.7 pp at f=0.50. There is no mechanism
  in this channel by which chunking can produce a crossover.
- **mixed50 / flip**: wrong-sign symbols cost −2 each and sit in the same
  chunk as their surviving peers. Clamping rescues a chunk only when its
  WHOLE signed sum goes negative; at f ≥ 0.30 typical per-chunk sums are
  small positives (~+3 to +5 expected at f=0.40 flip) far below the θ=16
  floor, so pooling cannot manufacture signal — it can only stop bleeding.
  That is exactly what the data shows: massive gains over legacy at low/mid
  f (flip f=0.20: +0.40 absolute), converging to similar collapse by f=0.45+.
- Net: the redesign fixes precisely what §3 of capacity_noise.md flagged
  (single-codeword fragility under flips) at the system's OWN layer, but the
  charter's "beat SDR at f ≥ 0.40" remains out of reach for i.i.d.-corrupted
  ternary queries against weight-preserving binary relocation.

## Associative-memory capacity curves (reports/capacity_curves.csv)

```
[calibration] chosen omega_threshold=16 sdr_threshold=13 (fixed for all M)
[M=  10] sdr_hit=1.00000 omega_pooled_hit=1.00000
[M=  25] sdr_hit=1.00000 omega_pooled_hit=1.00000
[M=  50] sdr_hit=1.00000 omega_pooled_hit=1.00000
[M= 100] sdr_hit=1.00000 omega_pooled_hit=1.00000
[M= 200] sdr_hit=1.00000 omega_pooled_hit=1.00000
[M= 400] sdr_hit=1.00000 omega_pooled_hit=1.00000
[M= 800] sdr_hit=1.00000 omega_pooled_hit=1.00000
[M=1600] sdr_hit=1.00000 omega_pooled_hit=1.00000
[M_99] sdr: not reached (>=0.99 through M=1600)
[M_99] omega_pooled: not reached (>=0.99 through M=1600)
```

Both systems at ceiling through M=1600; M₉₉% > 1600 for both. Label: these
are ASSOCIATIVE MEMORY capacities under clean-copy cued recall — they do not
measure representational state-space (analytic: ΩDR 10^95.51 vs SDR
10^84.38), and the CSV header says so.

## Throughput (M=200, chrono, 10 000 recalls; stderr + reports/throughput.csv)

```
[throughput] M=200 recalls=10000 omega_pooled=10761.9 ns/op omega_legacy=28142.7 ns/op sdr_nearest_decode=8393.0 ns/op pooled/sdr=1.28x pooled/legacy=0.38x sink=36005970000   (run 1)
[throughput] M=200 recalls=10000 omega_pooled=11069.0 ns/op omega_legacy=27446.1 ns/op sdr_nearest_decode=8431.1 ns/op pooled/sdr=1.31x pooled/legacy=0.40x sink=36005970000   (make bench run)
```

Triple: **ΩDR-pooled ≈ 10.8–11.1 µs/op · ΩDR-legacy ≈ 27.4–28.1 µs/op ·
SDR ≈ 8.4–8.5 µs/op** → pooled/SDR ≈ 1.28–1.31×, pooled/legacy ≈ 0.38–0.40×.
Chunking made ΩDR recall 2.6× CHEAPER (one 32-word pass instead of three)
while simultaneously improving accuracy — the accuracy win comes from the
probe/content model, not from spending more cycles. Timings live on stderr
and in `reports/throughput.csv`, keeping stdout byte-identical across runs.

## Superseded run (legacy BDD) — V1/G negative result, kept for honesty

Legacy design: m=3 INDEPENDENTLY-seeded branches sharing only a label;
query = corrupted copy of ONE branch; evidence = sum of three whole-code
signed sims; θ calibrated to 16 from {16,24,32,40}. Key numbers (V1 run):
SDR hit-rate ≥ ΩDR hit-rate at every f on all channels; flip f=0.30:
SDR 1.00000 vs ΩDR 0.54380; erasure f=0.40: 0.99984 vs 0.99529;
criterion line "[criterion] omega_hit>=0.95 at some f>=0.40 with sdr lower:
NO"; throughput bdd_recall ≈ 30.5–31.3 µs/op (3.27× SDR). Diagnosis
(verified): a query resembled at most one branch; the other two contributed
zero-mean crosstalk, so bundling added noise but no redundancy. The full V1
protocol, tables, and threats remain in git history and are summarized in
docs/spec/SPEC.md's honest caveats; nothing in that text claimed a win.

## Threats to validity

- **Threshold floor**: ΩDR's lowest calibrated candidate is 16; SDR operates
  at 13. On erasure (statistically identical survivor statistic) this alone
  decides the ordering. A per-channel or lower (e.g. θ=12–14) ΩDR grid might
  close the erasure gap but was outside the mandated calibration set; it
  would NOT create a mixed50/flip crossover (signal, not threshold, is
  binding there — pooled scores sit far below even θ=13 at f ≥ 0.40).
- **Single calibration point** (f=0.20, mean-F1 across channels) favors
  low-f performance; per-channel thresholds would shift mid-f points by a
  few percent, not the ordering.
- **Trial count**: 10⁵ trials/point → SE ≈ 0.09–0.15 % near rates 0.9–1.0;
  observed SDR–ΩDR gaps at f ≥ 0.30 are ≥ 0.8 pp ≫ noise. Capacity's 10⁴
  trials/M bound detectable dips at ≳ 0.5 %.
- **Correlated-code caveat (Lehky-2021)**: both stores use i.i.d. random
  codes; real cortical populations are pseudosparse/correlated, which would
  fatten competitor-similarity tails for BOTH systems; relative ordering
  under correlated codes untested.
- **Determinism**: fixed master seeds, fresh per-point seeds via
  `mix_streams`; double-run check passed (`diff` of two full runs of each
  binary: "NOISE_STDOUT_IDENTICAL", "CAP_STDOUT_IDENTICAL"); timing lines
  excluded from stdout by design.

## Compliance & acceptance

- `make` clean build: 0 errors, 0 warnings (-O3 -std=c++20 -Wall -Wextra
  -mpopcnt); `make test`: test_common, test_sdr, test_omega ALL PASSED
  (test_omega includes noiseless pooled recall 150/150 and a new chunk-
  isolation test: brute-force agreement of `sim_chunk`/`pooled_sim`,
  whole-chunk zeroing, single-chunk view, and wrong-sign-chunk clamp cases).
- src/omega multiply/divide/float-free: grep transcript shows ONLY pointer
  declaration/dereference hits (`uint64_t* ap`, `*slot`), zero `%`, zero
  float/double, zero arithmetic `*`; offsets are `CHUNK_WORD_OFF` lookups,
  `<< 6`, and masks.
- Determinism double-run: byte-identical stdout for both benches (markers
  above).

## Deviations from task spec

1. Chunk geometry: word-aligned **640/704/704 bits (10/11/11 words)** instead
   of 682/682/684, as recommended by the task ("implement cleanly with
   word-aligned boundaries"); totals match 2048 exactly. Documented here
   rather than in code comments.
2. Base code storage: task suggested pure views with zero copying; slots
   store the base `Code` plus the 3 legacy branch codes (kept ONLY for
   `recall_legacy` A/B). Scoring itself is view-only (word ranges into the
   base — no copies at recall time); extra memory exists solely to keep the
   superseded system runnable side-by-side.
3. `recall_legacy` takes an explicit threshold parameter so the pooled arm
   (calibrated θ) and legacy arm (fixed θ=24) can be evaluated against one
   memory instance without mutating shared state between trials.
4. Probe model: ΩDR-pooled query = corrupted copy of the stored base code
   (the redesign's content-true intent); V1's uniform-branch probe survives
   only in the legacy arm. This changes what "omega_hit" measures vs V1 and
   is called out wherever the two are compared.
5. Calibration grid for ΩDR trimmed to {16,24,32} per protocol (dropped 40);
   legacy reported at fixed θ=24 rather than its old calibrated 16 — its
   θ=16 numbers are preserved in the Superseded section for fairness.
