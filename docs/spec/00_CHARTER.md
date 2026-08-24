# ΩDR — Whole-Brain Sparse Ternary Hyperdimensional Framework
## Charter v0 (alignment contract for all swarm agents)

## Mission
Design, implement, prove, and verify a brain-inspired computational framework that
**strictly dominates Numenta's Sparse Distributed Representations (SDR)** on:
1. **Capacity**: SDR baseline capacity = number of distinct patterns C(2048,40) ≈ 10^84.
   ΩDR target: ≥ 10^95 distinguishable-with-margin states at equal bit budget (2048 bits).
2. **Noise robustness**: beat SDR's corruption tolerance at equal retrieval precision.
   Baseline: SDR survives ~30% bit flips at n=2048,w=40 with overlap threshold w/2.
   ΩDR target: ≥ 40% corruption tolerance, proven via Chernoff/Hoeffding bounds.
3. **Coverage**: whole brain (neocortex + thalamus + hippocampus/EC + cerebellum +
   basal ganglia + neuromodulation), not just neocortex. Every module must map to a
   documented neuroscientific finding (2020–2026 literature preferred).
4. **Efficiency**: pure C++20, uint64_t-packed bit vectors. Kernel ops restricted to
   {NOT, AND, OR, XOR, SHL, SHR, POPCOUNT, CMP, ADD}. **Zero multiplication,
   zero division, zero floating point** in all kernel paths (shift-add scaling allowed).

## Core idea (why ΩDR beats SDR)
- **Code**: Ternary Constant-Weight Hyperdimensional Codes (TCHC). Pattern =
  fixed k excitatory (+1) and k inhibitory (−1) bits among n; rest 0. State space
  Σ(n,k) = C(n,k)·C(n−k,k) ≈ 10^95 for n=2048,k=20 > SDR's 10^84.
- **Distance by construction**: sign-interleaved blocks guarantee minimum distance δ;
  decoding corrects t=⌊(δ−1)/2⌋ errors (coding-theoretic guarantee SDR lacks).
- **Similarity**: signed overlap s(A,B)=|A₊∩B₊| − |A₊∩B₋| − |A₋∩B₊| + |A₋∩B₋|
  via POPCNT of AND/XOR masks — branch-free, multiply-free.
- **Composition**: role-filler binding = XOR with random sparse permutation;
  bundling = popcount-majority vote; sequence = word-rotation permutation
  (multi-period rotations model entorhinal grid-cell multi-scale periods,
  coprime periods ⇒ huge composite cycle, multiply-free).
- **Memory**: Sparse Distributed Memory with 2-bit saturating counters
  (shift-based increment/decrement), dendritic segments as local k-WTA classifiers.

## Systems layer (whole-brain map)
| Module | Neuro basis | Mechanism |
|---|---|---|
| Cortical pool | sparse coding, k-WTA columns | encoder + competition |
| Hierarchy | cortical timescales gradient | pooled codes + rotation clocks |
| Thalamic router | thalamocortical gating | bitwise mask select, top-k |
| Hippocampal index | episodic recall, pattern separation | union-of-bindings store |
| Grid cells | multi-period periodicity | nested coprime rotations |
| Cerebellum | expansion recoding, supervised fast learning | hash spreader + perceptron shift-LR |
| Basal ganglia | action selection | popcount argmax WTA, DA-gated plasticity |
| Neuromodulator bus | DA/ACh/NE/5HT scalars | threshold shifts ±2^m |

## Claims to formalize in Lean 4 (`lean/OmegaDR`)
- T1: construction yields min distance δ₀; corrects t errors.
- T2: capacity lower bound: |{valid patterns}| ≥ 10^85 for n=2048,k=20.
- T3: false-match probability under ≤p corruption ≤ Chernoff bound; crossover vs SDR.
- T4: binding algebra is an abelian group (XOR laws); unbinding correctness.
- T5: operational typing: every kernel primitive ∈ allowed op set (multiply-free proof).

## Verification protocol (C++, `bench/`)
- V1: capacity curves vs SDR baseline (n=2048,w=40), noise sweeps 0–50%,
  throughput (patterns/s/core), memory footprint. All numbers land in `reports/`.
- V2: differential fuzzing: decode(encode(x)+noise) accuracy ≥ spec across 10^6 trials.
- V3: deterministic seeds; results reproducible in CI.

## Repo layout
```
papers/            downloaded source PDFs (agent A)
docs/research/     neuro_findings.md (A), baselines.md (B)
docs/math/         theorems.md — Lean-ready statements + proof sketches (C)
docs/spec/         SPEC.md v1 (orchestrator, after wave 1 integration)
src/common/        bitvec, rng, counters (D)
src/sdr_baseline/  faithful SDR/SP+TM-lite reference (D, then E)
src/omega/         ΩDR implementation (wave 2)
lean/OmegaDR/      Lean 4 project (D scaffolds, F proves)
bench/             benchmarks + fuzz harnesses (wave 2–3)
reports/           benchmark outputs, final report
```

## Rules of engagement for swarm agents
- Read this charter first. Stay inside your file contract; do not edit other
  agents' files or the charter. Report blockers instead of silently deviating.
- All claims must cite papers (path in papers/ or URL+DOI). No invented numbers.
- Code: no comments unless essential; no mult/div/float in src/** kernels;
  compile clean with -O3 -march=native -Wall -Wextra.
- Math: every theorem statement must be syntactically valid Lean 4 syntax even if
  the proof is `sorry` pending wave 2.
