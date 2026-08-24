# ΩDR Baselines: SDR / HTM / Monty / HDC-VSA — Rigorous, Citable Numbers

Agent B deliverable (research only). Companion to `docs/spec/00_CHARTER.md`.
Local primary-source PDFs live in `papers/`. Every number below is either
**(D) derived** (Python shown, reproducible with `python3`) or **(C) cited**
(with source). No invented figures.

---

## 1. SDR capacity baseline — exact combinatorics

### 1.1 Derivation

An SDR is a binary vector of length `n` with exactly `w` ON bits
(Ahmad & Hawkins 2015, §II.A; BaMI-SDR ch.). The set of representable patterns is
the constant-weight binary code of length n, weight w:

```
|SDR space| = C(n, w) = n! / (w! (n-w)!)
log10 C(n,w) = [lgamma(n+1) - lgamma(w+1) - lgamma(n-w+1)] / ln(10)
```

```python
import math
def log10C(n, w):
    return (math.lgamma(n+1) - math.lgamma(w+1) - math.lgamma(n-w+1)) / math.log(10)

log10C(2048, 40)   # 84.375074   (D)
log10C(1024, 21)   # 43.418291   (D)
log10C(512, 13)    # 25.359522   (D)
```

### 1.2 Capacity table

| Code | n | w | log10 |C(n,w)| | Value | Source |
|---|---|---|---|---|---|
| HTM canonical SDR | 2048 | 40 | **84.375074** | ≈ 8.9×10^84 distinct patterns | (D); consistent with Ahmad & Hawkins 2015 Table 1 & BaMI ("2.37×10^84 encodings"; "more than the estimated ~10^80 atoms in the observable universe") |
| Smaller SDR | 1024 | 21 | 43.418291 | ≈ 2.6×10^43 | (D) |
| Small SDR | 512 | 13 | 25.359522 | ≈ 2.3×10^25 | (D) |
| ΩDR TCHC Σ(2048,k=20) = C(2048,20)·C(2028,20) | 2048 | k+k | **95.514470** | ≈ 3.3×10^95 | (D); confirms charter's "≈10^95" claim |
| ΩDR headroom variant Σ(2048,k=25) | 2048 | 25+25 | 114.923317 | ≈ 10^114.9 | (D) |

**Conclusion for charter T2:** at equal bit budget (2048 bits), ternary
constant-weight codes with k=20 already give ≥ 10^95 states vs SDR's 10^84.38 —
a factor of ≈ 3.7×10^10 (≈ 11.14 orders of magnitude) more raw codewords.
Note this is *representational* capacity; distinguishability-with-margin is
treated in §7.

---

## 2. Noise robustness numbers (Numenta's own math)

Primary sources: `papers/ahmad-hawkins-2015-properties-of-sdr.pdf` (arXiv:1503.07469),
`papers/numenta-bami-sdr.pdf`, `papers/ahmad-hawkins-2016-how-neurons-operate-sdr.pdf`
(arXiv:1601.00720), `papers/hawkins-ahmad-2016-thousands-of-synapses.pdf`
(doi:10.3389/fncir.2016.00023).

### 2.1 Overlap-set mathematics (Ahmad & Hawkins 2015)

Definitions (C):
- overlap(x,y) = x·y (count of co-ON bits; deliberately *not* Hamming/Euclidean).
- match(x,y) ⟺ overlap(x,y) ≥ θ.
- Overlap set Ω(n,w,b) = {vectors of size n, weight w, with exactly b bits overlapping x}:

```
Ω(n,w,b) = C(w, b) · C(n-w, w-b)          Eq.(3), valid b ≤ w_x, b ≤ w
```

False-match probability against one random SDR (Eq. 4):

```
fp(n,w,θ) = Σ_{b=θ}^{w} C(w,b)·C(n-w,w-b) / C(n,w)
```

with excellent approximation from the θ-term alone when w>7, θ>w/2 (their Eq. 5).

Expected overlap of two random SDRs: E[b] = w²/n (e.g. 0.78 bits at 2048/40). (D)

### 2.2 Error tolerance vs threshold — derived curves (uses their Eq. 4)

| θ (n=2048,w=40) | fp(random collision) | Interpretation |
|---|---|---|
| 36 (90% of w) | 2.60×10^-68 | tiny noise window |
| 30 (75%) | 1.03×10^-49 | |
| 24 (60%) | 8.38×10^-35 | |
| 20 (=w/2) | **2.49×10^-26** | canonical "50% ON-bit loss tolerated" point |
| 18 (45%) | 1.76×10^-22 | |
| 16 (40%) | 6.99×10^-19 | |

Same formula at other sizes (D): n=1024/w=21: θ=13→1.93×10^-19, θ=10→3.34×10^-13;
n=512/w=13: θ=8→1.43×10^-11, θ=6→1.12×10^-7.

Published check-points reproduced by the code above (C, Ahmad & Hawkins 2015 §II.D):
- n=1024,w=4,θ=2 → "one in 14,587" ⇒ 6.86×10^-5; computed: 6.855×10^-5 ✓
- n=1024,w=20,θ=10 → "< 1 in 10^10"; computed: 9.33×10^-14 ✓ (bound satisfied)
- Their headline claim: "reliable classification can be performed with as much as
  50% noise" (abstract) means: corrupt up to 50% of the ON bits, keep θ=w/2.
  With θ=w/2 the *false-positive* side still stays ≤ 2.5×10^-26 at 2048/40.

### 2.3 Symmetric bit-flip model (charter's "~30% bit flips" baseline)

If each bit flips independently with probability p, then
overlap(x_orig, x_corrupted) ~ Binomial(w, 1-p) exactly (bits outside the ON set
cannot add overlap because the original has 0s there). (D)

```
P(match failure) = Σ_{k<θ} C(w,k)(1-p)^k p^(w-k),  θ=w/2=20, w=40:
p=0.20 → 5.0×10^-06      p=0.40 → 7.4×10^-02
p=0.25 → 1.7×10^-04      p=0.45 → 2.1×10^-01
p=0.30 → 2.4×10^-03      p=0.50 → 4.4×10^-01
```

So the honest reading of the charter baseline: SDR *retrieval* survives 30%
symmetric flips with failure probability only 2.4×10^-3 per comparison (and
survives 50% ON-bit deletion deterministically if exactly half remain);
**but** the guarantee is purely probabilistic — there is no minimum-distance /
error-correcting-pairs structure (see §3.3).

### 2.4 Subsampling / dendritic classification (Ahmad & Hawkins 2016, arXiv:1601.00720)

(C) A segment storing s random synapses of a pattern matches with threshold
θ_s; false negatives vanish when noise ≤ s − θ_s. For M independent segments:

```
P(false positive over M segments) = 1 − (1 − P_fp_segment)^M   Eq.(8)
bounded by  M · P(h(x,s))                          Eq.(9)
```

Flagship number (§ examples): n=10,000, sparsity 3% (w=300), s=30 synapses,
threshold 15 ⇒ detect M=10^6 patterns with FP < 10^-9; error falls exponentially
in synapse count; 20–25 synapses often suffice for error < 10^-12 (their Fig. 5B).
Both high dimensionality AND sparsity are required (dense or small-n regimes fail).

### 2.5 Union property and its limits

Union X = OR of M SDRs; expected density (C, Eq. 12; erratum: original paper's
Eq. 13 had a typo corrected in BaMI/1601.00720 — see HTM Forum thread
"Found an error in SDR paper union false probability calculation", May 2016):

```
p_0 = (1 − w/n)^M ;  E[ON bits] = n·(1−p_0)
union FP (exact match) = (1 − p_0)^w
```

Derived at n=2048, w=40 (D):

| M unioned SDRs | Expected density | Note |
|---|---|---|
| 1 | 2.0% | nominal |
| 10 | 17.9% | |
| 20 | 32.6% | TM multi-prediction regime |
| 40 | 54.6% | majority of bits ON |
| 80 | **79.4%** | near-saturated |

Published check-values (C): n=1024,w=2,M=20 → FP ≈ 1/680 (computed 1.47×10^-3 ✓);
n=1024,w=20,M=20 → ≈ 1 in 5.5 billion (computed 1.84×10^-10 ✓); with noisy
matching θ=w−2 at those params ≈ 1 in 123 million; Numenta's own text admits
"there are limits on the number of vectors that can be reliably stored" and
that beyond saturation "almost any other random vector will return a false
positive match".

Temporal-context capacity of TM: x cells/column gives x^w contexts; x=8, w=40 →
**1.33×10^36** (C, Eq. in §III; verified D). Sequence capacity scales linearly
with synapses per neuron (C, Hawkins & Ahmad 2016 abstract).

---

## 3. HTM Spatial Pooler + Temporal Memory: properties and weaknesses

Properties (C): SP learns fixed-sparsity (~2%) SDRs via competitive Hebbian
learning + homeostatic "boosting" + local/global inhibition (k-WTA), preserving
input topology; designed properties: similarity preservation, adaptivity,
fixed sparsity, noise robustness, fault tolerance
(`papers/` reference: HTM Spatial Pooler paper, PMC5712570; NuPIC Spatial
Pooling Algorithm Details PDF). TM adds high-order sequence memory via distal
dendritic segments (128 segments × ≤40 synapses per neuron in simulations;
2048 columns × 32 cells network in Hawkins & Ahmad 2016 Fig. 6).

### Weaknesses to exploit (ranked, all sourced)

1. **No guaranteed minimum distance / no deterministic error correction.** (D+C)
   Two legal SDRs may share w−1 bits (Hamming distance 2): δ_min = 2 ⇒ correctable
   worst-case errors t = ⌊(δ_min−1)/2⌋ = 0. All SDR guarantees are expectation/
   tail-probability statements under *random* noise (Eq. 4); an adversary or a
   correlated-noise source gets no protection. ΩDR's coding-theoretic δ (charter T1)
   is a categorical advantage.
2. **Boosting is ad-hoc and fragile.** htm.core maintainers: SP has three boosting
   mechanisms and "none of them are very good" (GitHub issue #284). The 2016
   boosting-rule rewrite was a breaking change after instability of the discrete
   rule; without boosting ~50% of columns went unused on NYC-taxi (HTM Forum,
   Dec 2016). Numenta's own default for production anomaly benchmarks is
   maxBoost=1.0, i.e., boosting disabled (Ahmad, HTM Forum May 2016). A 2024
   information-theoretic study shows strong sensitivity: boostStrength 0 → near-
   random SP; 100 → catastrophic forgetting; optimum ≈10
   (Neural Proc. Lett., doi:10.1007/s11063-024-11546-8). Boosting also fights
   stability: oscillating column assignment forces TM to relearn sequences
   (HTM Forum "Understanding Boosting in Spatial Pooler").
3. **Union/prediction saturation.** Multi-step predictions OR together into ever-
   denser unions — 79% dense after 80 unions at canonical parameters (§2.5) —
   degrading precision step by step (Ahmad & Hawkins 2015 §II.G–H; practitioners:
   "your SDRs will get denser and denser with each timestep" — HTM Forum 2017).
   ΩDR's bounded-weight superposition avoids uncontrolled density growth.
4. **Capacity claims are representational, not memory capacity.** 10^84 counts
   codewords, not storable/recallable associations; actual associative capacity
   scales linearly in synapses (Hawkins & Ahmad 2016) and segment subsampling
   trades recall fidelity for capacity (1601.00720). ΩDR can quote both
   code-size *and* SDM-style storage bounds.
5. **Global-inhibition crutch / topology vs speed conflict.** Global inhibition
   "boosts performance x60" over biologically-motivated local inhibition
   (NuPIC docs); forum analysis shows deployed models effectively run fully
   connected with topology off, weakening the cortical-topology story
   ("SDR theoretical properties and HTM", Jan 2019).
6. **Encoder-dependence of semantics.** SDR quality hinges on hand-built encoders
   distributing semantic bits; mis-encoded inputs defeat the overlap math
   (same 2019 forum thread; Purdy 2016 on categorical inputs).
7. **Binary-only evidence.** No inhibitory channel: overlap cannot express
   *negative* evidence; two patterns that should mutually exclude still only
   lose overlap linearly. Ternary signed overlap (charter) subsumes XOR-style
   cancellation that BSC/MAP get but SDR lacks.

---

## 4. Monty / Thousand Brains Project (2023–2026): scope and admitted gaps

Sources: `papers/clay-et-al-2024-thousand-brains-project.pdf` (arXiv:2412.18354),
`papers/leadholm-et-al-2025-thousand-brains-systems.pdf` (arXiv:2507.04494),
TBP docs (docs.thousandbrains.org FAQ / Other Aspects / Application Criteria),
heterarchy theory paper (arXiv:2507.05888).

What Monty IS (C): sensorimotor learning system of repeating "learning modules"
(cortical-column analogues) that build object models in explicit reference
frames, communicate via a cortical messaging protocol (CMP), vote, and act
through a motor system; rapid continual Hebbian-style learning; first
quantitative results on 3D object recognition/pose (Leadholm et al. 2025).

What it does NOT cover / limitations (all C):

- **No SDR/HTM substrate:** FAQ states they "have not yet included lower-level
  neuroscience components such as HTM, sparse distributed representations
  (SDRs), grid-cells, and active dendrites," deferring them "where a clear case
  for a comparative advantage exists." ⇒ ΩDR occupies exactly the abandoned
  representation layer beneath TBT-style architecture.
- **Subcortical structures:** the brain-side processing Monty assigns to its
  "motor area" corresponds to what is "subcortical" biologically, and the docs
  concede a future role for deep RL for model-free subcortical policy. No
  cerebellum, basal ganglia, neuromodulator modeling in the current system;
  thalamus exists so far only as *theory* (pose-converter proposal,
  arXiv:2507.05888), not implementation.
- **Sensorimotor-only framing:** "Monty is NOT made for learning from static
  datasets"; movement must currently live in ≤3D Euclidean space tracked with
  Euclidean coordinates (custom-app doc). Abstract/conceptual spaces remain
  aspirational.
- **Compositional objects unresolved:** on Omniglot, unseen-character
  generalization "degrades a lot"; compositional models require hierarchical
  module stacking that the shipped configs don't do — "our research team is hard
  at work getting Monty to model compositional objects."
- **Maturity:** explicitly "still a research project… major version zero… public
  API not stable" (Application Criteria). Benchmarks are narrow (YCB-style 3D
  objects, Habitat sim, Monty-meets-world image scans).
- Positioning note: TBP FAQ contrasts TBP (higher-level principles: sensorimotor,
  reference frames) vs HTM (lower-level principles: SDRs, sequence memory) —
  i.e., Numenta itself split the stack, leaving a representation-layer vacuum.

---

## 5. HDC / VSA state of the art (and what ΩDR borrows)

Sources: `papers/kleyko-et-al-vsa-survey-part1.pdf` (arXiv:2111.06077; Part II:
ACM Comput. Surv. 55(9), Art. 175, 2023, arXiv:2112.15424),
`papers/kanerva-2009-hyperdimensional-computing.pdf`,
Schlegel et al., "A comparison of vector symbolic architectures"
(AI Review, doi:10.1007/s10462-021-10110-3), Thomas et al.,
"A Theoretical Perspective on Hyperdimensional Computing" (JAIR 72:215–249, 2021),
"Capacity Analysis of Vector Symbolic Architectures" (JAIR,
jair.org/index.php/jair/article/view/18335), classification review (NSF PAR 10195394).

### 5.1 Model zoo and operations

| VSA | Vector space | Binding | Unbinding | Bundling | Notes (C) |
|---|---|---|---|---|---|
| BSC (Kanerva '96) | dense binary p=.5 | XOR (self-inverse) | XOR | majority vote, random tie-break | binding dissimilar to operands; bundling similar |
| MAP-B/-I/-C | ±1/int/real | elementwise mult | mult by inverse | sum+normalize | Gayler '98 |
| HRR (Plate '95) | real | circular convolution | correlation | sum | needs float |
| FHRR (Plate '03) | unit complex | angle addition (elem mult) | conjugate mult | vector sum | fractional binding ⇒ periodic encodings |
| BSDC family (Rachkovskij '01) | sparse binary, optimal p≈1/√D | context-dependent thinning / shift / segment-shift | inverse op | OR/sum | highest bundling dimension-efficiency along with FHRR (Schlegel exp.) |
| MBAT, VTB, TPR, GAHRR | matrices/geom. algebra | matrix mult / outer product | — | sum | see Kleyko Part I taxonomy |

Universal ops: similarity (Hamming/cosine), permutation ρ (often cyclic shift;
ρ^i for sequences/n-grams; permutations distribute over bundling), clean-up
memory = nearest-neighbor item memory (= ΩDR's k-WTA classifier analogue).

### 5.2 Capacity & noise numbers worth quoting

- Kanerva 2009 (C): N=10,000 dense binary; distance concentrates at 5000±50·…
  (binomial σ=50): <10^-6 of hyperspace lies within distance 4760 of a point and
  <10^-9 within 4700; >⅓ random bit corruption still identifies the correct
  vector "with near certainty"; related pair 2500 apart + ε=1/3 noise stays 17σ
  closer to the true partner (his d′=(1−2d)ε rule).
- BSC bundling capacity ≈ linear in D; signal-detection analysis says capacity
  "increases roughly linearly with dimension and is quite insensitive to the
  particular model" (VSA-for-hardware review, Proc. IEEE / PMC10588678).
- Quiroz-Mercado 2022 (C): semantic-feature retrieval in BSC saturates ≈30–40
  feature-relation pairs even at N=10,000 ⇒ superposition capacity is the real
  bottleneck, not code count.
- Schlegel 2021 (C, experiments): sparse-binary (BSDC variants) and FHRR need
  fewest dimensions for reliable bundles; dense binary (BSC/MAP-B) worst;
  HRR/FHRR gap shows quantization matters.
- QuantHD (C via NSF review): projecting class HVs to ternary {−1,0,+1} +
  retraining improves accuracy 17.2% on average over binarized HD at similar
  cost — evidence that *ternary* is the sweet spot ΩDR pushes further.
- SparseHD (FCCM 2019): sparsifies trained HVs for inference efficiency.
  LeakyHD/OnlineHD (DATE 2021), NeuralHD (DAC 2022), DistHD, CompHD, QubitHD,
  TrainableHD (DATE 2023; up to +22.26% acc, INT8 QAT, 56× faster than DL on
  Jetson Xavier) — the modern accuracy/efficiency line (torchhd classifiers list).
- Yan et al. 2023 (arXiv:2301.10902) (C): worst/average-case HDC accuracy bound
  *decreases* once D exceeds what K classes need; MNIST at D=64 reaches 91.12% —
  dimensionality is being wasted in mainstream HDC.
- Başaklar et al. 2021 (arXiv:2103.06709) (C): optimized hypervector design cuts
  D by 32–128× while keeping accuracy and doubling noise robustness (Pareto
  front between accuracy and class-cluster separation).
- Thomas et al. JAIR 2021 (C): rigorous non-asymptotic decoding/noise bounds via
  incoherence for dense-binary and sparse-binary codewords (Lemmas 11–13);
  binding algebra requirements = abelian group + distribution over bundling —
  precisely ΩDR's Lean targets T4/T5.
- JAIR "Capacity Analysis of VSA": connects VSAs to Bloom filters/counting Bloom
  filters for membership & intersection-size estimation — same lineage Ahmad &
  Hawkins acknowledge for SDRs (Bloom 1970).

### 5.3 What ΩDR borrows vs where ternary constant-weight wins

Borrow (mechanism-level): XOR/permutation binding group laws (→ T4), rotation
sequences à la MAP (→ word-rotation clock, coprime multi-period grids), clean-up
memory as k-WTA (→ SDM + counters), superposition-as-evidence with saturating
counters (QuantHD-style ternary accumulation, but shift-add only).

Improve (representation-level):
- Dense-binary HDC has *no* designed minimum distance either — same probabilistic
  weakness as SDR (Thomas et al. treat noise statistically, not worst-case).
  TCHC supplies δ and t-error correction by construction.
- QuantHD's {-1,0,+1} vectors are post-hoc quantizations of real hypervectors;
  weights are unconstrained ⇒ no constant-weight control of density, no closed-
  form state-space size, drift under bundling. TCHC fixes k₊=k₋ exactly:
  state space known analytically (Σ(n,k)), density invariant under every legal op.
- Signed components let bundling *cancel* false-positive mass (excitatory/
  inhibitory balance), attacking the union-saturation failure (§3 item 3) that
  both SDR unions and BSC majority bundles suffer.
- Sparse-binary VSAs (BSDC) validate constant-weight-ish sparse codes' superior
  bundling economics; TCHC generalizes to signed constant weight while keeping
  POPCNT-only kernels (multiply-free, per charter op set).

---

## 6. Published "beyond-SDR" prior art to position against

- **Active-dendrite subsampling theory** (Ahmad & Hawkins 2016, arXiv:1601.00720):
  Numenta's own step beyond plain SDR matching — segments as subsample
  classifiers; still binary, still probabilistic-only guarantees.
- **Sparse deep nets** (Ahmad & Scheinkman 2019, arXiv:1903.11257): k-WTA layers
  + boosting gain noise robustness on MNIST/GSC/CIFAR-10 while test accuracy
  stays flat — "test accuracy by itself is not a reliable indicator of overall
  robustness." Uses the same overlap-set math (their Eq. 2 cites Ahmad-Hawkins).
  ΩDR differs: representation-level code design vs architecture-level sparsity.
- **SDM lineage** (Kanerva 1988; Kanerva 2009 tutorial in papers/): hard-location
  sparse memory; ΩDR's SDM-with-2-bit-counters is direct descendant — must cite
  as prior art, differentiate via ternary codes + correction guarantees.
- **BSDC/context-dependent thinning** (Rachkovskij 2001; Kleyko surveys):
  closest sparse-VSA relative of constant-weight coding; unsigned only.
- **Constant-weight ECC tradition** (classical coding theory; e.g., optical
  communications): ΩDR's δ-by-construction claim should be framed as importing
  classical constant-weight code theory into neural representation — no
  neuro-AI system we found ships a proven minimum distance with multiply-free
  kernels.
- **TPR/HRR compositional structure** (Smolensky 1990; Plate 1995): role-filler
  composition predates everything; ΩDR must claim only the *sparse ternary
  constant-weight, correction-guaranteed, multiply-free* instantiation.
- **Frady/Kleyko/Olshausen/Sommer "Computing on functions" line** (arXiv:2109.03429)
  and VSA-universality work (Proc. IEEE 2023): theory neighbors; position ΩDR as
  the systems/whole-brain completion with provable margins.

---

## 7. Summary scorecard vs charter targets

| Charter axis | SDR baseline (verified) | ΩDR target | Status of baseline |
|---|---|---|---|
| Capacity @2048 bits | log10 = **84.375074** (D) | ≥10^95 (Σ(2048,20)=95.51) (D) | beatable by 11.1 orders |
| Noise tolerance | 50% ON-bit loss @θ=w/2; fp≤2.5×10^-26 @θ=20 (C/D); 30% symmetric flips ⇒ fail prob 2.4×10^-3 (D) | ≥40% corruption w/ Chernoff proof, equal precision | baseline quantified; no δ guarantee to beat |
| Coverage | neocortex only (SP/TM); Monty: cortex+motor stub, no SDR substrate, no basal ganglia/cerebellum/neuromodulation (C) | whole-brain modules | clear whitespace |
| Efficiency | float permanences, multiplicative boosting in practice (C) | uint64 bitwise ops only | kernel-compatible |

## 8. References

Local (`papers/`, all verified `%PDF`):
1. ahmad-hawkins-2015-properties-of-sdr.pdf — Ahmad & Hawkins, arXiv:1503.07469 (2015).
2. ahmad-hawkins-2016-how-neurons-operate-sdr.pdf — Ahmad & Hawkins, arXiv:1601.00720 (2016).
3. hawkins-ahmad-2016-thousands-of-synapses.pdf — Hawkins & Ahmad, Front. Neural Circuits 10:23 (2016), doi:10.3389/fncir.2016.00023.
4. numenta-bami-sdr.pdf — Numenta, Biological and Machine Intelligence, SDR chapter.
5. kanerva-2009-hyperdimensional-computing.pdf — Kanerva, Cognitive Computation 1(2):139–159 (2009).
6. clay-et-al-2024-thousand-brains-project.pdf — Clay, Leadholm, Hawkins, arXiv:2412.18354 (2024).
7. leadholm-et-al-2025-thousand-brains-systems.pdf — Leadholm, Clay, Knudstrup, Lee, Hawkins, arXiv:2507.04494 (2025).
8. kleyko-et-al-vsa-survey-part1.pdf — Kleyko, Rachkovskij, Osipov, Rahimi, arXiv:2111.06077 (Part I; Part II = ACM Comput. Surv. 55(9):175, 2023, arXiv:2112.15424).

Web (URL + DOI where applicable):
9. Numenta, Spatial Pooling Algorithm Details — numenta.com/assets/pdf/spatial-pooling-algorithm/.
10. HTM Spatial Pooler (online) — PMC5712570.
11. HTM Forum threads: boosting rewrite (Dec 2016); "Problems with boosting?" (May 2016); "Understanding Boosting" (Aug 2016); SDR-paper Eq.13 erratum (May 2016); "SDR theoretical properties and HTM" (Jan 2019); multi-step prediction density (2017–2018); htm.core issue #284.
12. Öztan et al.-style SP study: Performance Comparison of HTM-SP algorithms via information-theoretic measures, Neural Processing Letters 62, doi:10.1007/s11063-024-11546-8 (2024).
13. Schlegel, Neubert, Protzel, A comparison of vector symbolic architectures, Artif. Intell. Rev. 55, doi:10.1007/s10462-021-10110-3 (2022).
14. Thomas et al., A Theoretical Perspective on HD Computing, JAIR 72:215–249 (2021); Capacity Analysis of VSA, JAIR (jair.org article 18335).
15. SparseHD (FCCM 2019); QuantHD (DAC 2019); OnlineHD/LeakyHD (DATE 2021); NeuralHD (DAC 2022); Başaklar et al. arXiv:2103.06709; Yan et al. arXiv:2301.10902; TrainableHD (DATE 2023); HDC classification review (NSF PAR 10195394); VSA-for-emerging-hardware review (Proc. IEEE, PMC10588678); Quiroz-Mercado 2022 (Scielo S1405-55462022000201027).
16. Thousand Brains Project docs: docs.thousandbrains.org (FAQ, Other Aspects, Application Criteria, Custom Applications); heterarchy/thalamus theory arXiv:2507.05888.
17. Ahmad & Scheinkman, How Can We Be So Dense?, arXiv:1903.11257 (2019).
