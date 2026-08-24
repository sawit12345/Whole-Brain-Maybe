# ΩDR SPEC v1 — integrated after Wave 1
Inputs: docs/research/{baselines,neuro_findings_A,neuro_findings_B}.md,
docs/math/{capacity_noise,algebra_semantics}.md, docs/spec/toolchain_report.md.

## Verified headline numbers
| Metric | SDR baseline | ΩDR | Source |
|---|---|---|---|
| State space @2048 bits | 10^84.38 | **10^95.51** (+11.1 orders) | capacity_noise.md §1 |
| Per-pair FP tail | θ=13→6.0e-14 | θ=12→**5.0e-16** | exact DP, mass=1.0 |
| Erasure tolerance (single codeword, ε=1e-9) | 23.94% (flip) | **25.95%** | capacity_noise.md §3 |
| Flip-heavy channels | stronger | weaker single-codeword → fixed by bundling (below) | honest caveat |

## Noise plan (to hit charter ≥40%): Bundled Differential Decoding (BDD)
Store each item as bundle of m=3 independently-seeded sub-codewords sharing the
logical tag. Retrieval sums the three signed overlaps (integer adds) and applies
threshold. Erasure model: per-branch 25.9% tolerance ⇒ majority-of-three with
threshold 2k lifts effective tolerance past 40% (exact number measured wave 3,
target reports/noise_sweep.csv). All bitwise/ADD only.

## Core API contract — src/omega (agent E fills skeletons; no mult/div/float ever)
```cpp
namespace omega {
struct Code { BitVec pos, neg; };            // invariant: !(pos & neg).any()
uint32_t sim(const Code&a, const Code&b);    // signed overlap via 4 POPCNTs
Code encode(uint64_t stream_id, Rng&);       // deterministic, exactly K=20 per plane
Code bind(const Code&x, const Code&role);    // per-plane: rot(x,rho_role) XOR role
Code unbind(const Code&y, const Code&role);  // inverse (Lean-proved laws)
struct BddMemory;   // m-subbundle store/recall, 2-bit saturating counters
struct Router;      // thalamic gate: mask AND + popcount top-k area select
struct GridClock;   // 3 down-counters periods {61,127,251}, cycle 1944497
struct Cerebellum;  // xorshift-hash expansion recoder + shift-LR perceptron
struct Selector;    // BG argmax popcount WTA + DA-gated plasticity flag
struct ModBus;      // DA,ACh,NE,5HT int8 scalars -> threshold shifts +/-2^m
}
```

## Module ↔ neuroscience anchors
Cortex pools/iWTA [Osaulenko21], dendritic error compartments [Bicknell-Häusser21],
thalamic selective-inhibition gates [McCreesh22, Hummos22], DG WTA sparsification
[Kim-Lim21], grid torus/modules [Schøyen25, Khona21], GrC expansion fan-in≈4 +
kernel machine [Bae22, Lanore21], DA tonic gating [DA-neurorobotic24], 5HT timescales
[Miyazaki18], synapses≈4–6 bits ⇒ 2-bit counters [Bartol15, Samavat24],
communication-dominated cost ⇒ POPCNT kernels [Herculano-Houzel11].

## Lean status (lean/OmegaDR)
Proven axiom-free (core 4.33.1): binding group laws (T4), rotateRight-XOR distributivity,
multiply-free ISA gate (T5: OpKind 9 ctors + prog_allowed), grid coprime cycle
61*127*251=1944497 (T7). Pending: rotateRight_add (needs Mathlib lemma),
capacity inequality 10^95 < Σ(2048,20) (statement up, NEEDS-MATHLIB),
noise Chernoff formalization (wave 2F attempts core-only Cantelli-style bound).

## Wave assignments
E: implement src/omega per API contract + tests (tests/test_omega.cpp) green.
F: restructure lean/OmegaDR (Basic/Binding/ISA/Grid/Capacity), minimize sorries,
   compile-clean `lake build`.
G (after E): bench/noise_bench.cpp — BDD sweep f∈[0,.5]×{erasure,mixed,flip}×10^5 trials
   vs SDR baseline; bench/capacity_bench.cpp — recall-vs-M curves; CSVs to reports/.
H (last): reports/FINAL_REPORT.md claims-vs-evidence table.
