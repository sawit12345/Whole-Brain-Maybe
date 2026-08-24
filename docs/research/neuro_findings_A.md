# neuro_findings_A — Wave-1 neuroscience scan for ΩDR (agent A)

Scope: 2020–2026 literature with computational relevance, mapped to the ΩDR charter
(`docs/spec/00_CHARTER.md`). Every numeric claim below was read directly from the cited
paper (local copy in `papers/` where listed) or from its open abstract page. Pre-existing
`papers/` files from earlier waves are reused and flagged as `[local]`.

---

## Topic 1 — Neocortical sparse coding, k-WTA, predictive processing

- **[Osaulenko & Ulianych 2021]** Iterative winners-take-all (iWTA): explicit inhibitory
  population balances excitation iteratively instead of a fixed-k cut; supports habituation,
  decorrelation, clustering over binary assemblies; notes plain kWTA forces a constant
  number of active units, unlike biology. *ΩDR*: validates k-WTA column encoders but warns
  that **fixed-weight codes are a modeling choice** — capacity proofs (charter T2) should
  treat variable-k outputs as out-of-spec noise, not valid states. Decorrelation-by-inhibition
  maps to inhibitory-threshold shifts (±2^m on the neuromodulator bus).
  `papers/osaulenko2021_iwta_cell_assemblies.pdf`, https://arxiv.org/pdf/2108.00706 (arXiv:2108.00706)

- **[Lehky, Tanaka & Sereno 2021]** "Pseudosparse" coding: across macaque visual-cortex
  datasets population responses to different stimuli are highly correlated — pseudosparseness
  index 0.59–0.98 (authentic sparseness ⇒ ~0). Apparent cortical sparseness may be a
  correlation artifact. *ΩDR*: do not assume random-set independence of sparse patterns;
  the Chernoff/Hoeffding false-match analysis (T3) must be stated for worst-case correlated
  codes, and the ternary sign structure helps because correlated excitatory mass cancels in
  signed overlap s(A,B). `papers/lehky2021_pseudosparse_coding.pdf`,
  https://www.nature.com/articles/s42003-020-01572-2 (doi:10.1038/s42003-020-01572-2)

- **[Dalgleish et al. 2020]** Few-neuron perception: optogenetic stimulation of very small
  subsets of mouse barrel-cortex neurons is sufficient for behavior-level detection;
  perceptual read-out rides on small, sparse ensembles rather than population-average codes.
  *ΩDR*: supports retrieval thresholds operating on tens-of-bits active sets at n=2048,
  i.e., w≈40-style budgets remain behaviorally plausible. [local]
  `papers/dalgleish2020_few_neurons_perception_sparse_coding.pdf`

- **[Millidge, Tschantz & Buckley 2021]** Predictive coding review: PC = variational inference;
  only deviations from top-down predictions propagate upward (sparsified residual traffic);
  precision weights act as multiplicative gains. *ΩDR*: prediction error = XOR-difference
  masks between predicted bundle and bottom-up code; nonzero-error sparsity is exactly the
  low-overlap regime where POPCOUNT-based early-exit saves work; precision ≈ threshold
  shift ±2^m. `papers/millidge2021_predictive_coding_review.pdf`

- **[Jiang & Rao 2022]** Predictive-coding theory survey: internal simulation in absence of
  input, attention as robust PC, what–where/equivariant networks, active inference.
  *ΩDR*: motivates the hippocampal/thalamic modules re-entering predicted codes into the
  cortical pool as bundles (XOR-bound context), with surprise = signed overlap below margin.
  `papers/jiang_rao2022_predictive_coding_theories.pdf`

- **[Levy & Calvert 2021]** Cortical energy audit: signaling/communication dominates the
  brain ATP budget (>two-thirds of the ~4.94 W total; spike computation is a minor term).
  *ΩDR*: bitwise kernel set {AND/OR/XOR/POPCOUNT/SHL/SHR} with zero mult/div mirrors the
  biological pressure to make *transmission*, not arithmetic, the costly resource to minimize.
  `papers/levy_calvert2021_energy_audit_cortex.pdf`

## Topic 2 — Dendritic computation & nonlinear segments

- **[Bicknell & Häusser 2021]** Biologically detailed pyramidal models trained with one
  general learning rule; single synapses approximate the global rule using only local signals.
  Active dendrites (supralinear NMDA spikes in basal, sublinear apical integration, local
  dendritic inhibition) solve a nonlinear feature-binding task a passive point neuron cannot;
  active versions gain accuracy, flexibility, and noise robustness.
  *ΩDR*: dendritic segment ≈ local k-WTA classifier over disjoint bit-fields (charter memory
  row); per-segment hard-threshold = AND+POPCOUNT+CMP, no multiply.
  `papers/bicknell_hausser2021_dendritic_learning_rule.md`,
  https://pmc.ncbi.nlm.nih.gov/articles/PMC8691952/ (doi:10.1016/j.neuron.2021.09.044)

- **[Mikulasch, Rudelt, Wibral & Priesemann 2022]** Dendritic predictive coding with spiking
  neurons: somatic compartment carries the prediction-driving signal while segregated distal
  dendrites compute the mismatch to top-down predictions — an error channel inside one cell.
  *ΩDR*: two-register layout per unit (state word vs error word, both packed uint64_t);
  error = XOR(state, prediction) then POPCOUNT — the apical/basal split becomes an ALU split.
  `papers/mikulasch2022_dendritic_predictive_coding.pdf`,
  https://arxiv.org/abs/2205.05303 (arXiv:2205.05303)

- **[Golkar et al. 2022]** BioCCPC: covariance-constrained predictive coding with
  pyramidal-like connectivity (segregated apical feedback, basal feed-forward); competitive
  with less-constrained PC implementations while respecting dendritic anatomy.
  *ΩDR*: precedent that anatomically-motivated wiring constraints need not cost accuracy —
  argues for keeping ΩDR's fixed random sparse permutations instead of dense learned weights.
  `papers/golkar2022_bioccpc_dendritic_pc.pdf`

- **[Schubert & Gros 2021]** Two-compartment coincidence detection: plain Hebbian/BCM rules on
  proximal synapses align basal input with an apical "teaching" signal; the compartment model
  tolerates substantially stronger distractors than a point neuron.
  *ΩDR*: credit assignment via co-activation of two sparse masks (context ∩ content) —
  implementable as POPCOUNT(AND) gating plasticity increments in the 2-bit counter SDM.
  No local file; https://www.frontiersin.org/articles/10.3389/fncom.2021.718020/full
  (doi:10.3389/fncom.2021.718020)

- **[Hawkins & Ahmad 2016]** (foundational, pre-window): thousands-of-synapses model where
  distal dendritic segments act as independent pattern detectors triggering depolarization —
  the original justification for "dendritic segments as local k-WTA classifiers" in the
  charter systems table. [local]
  `papers/hawkins-ahmad-2016-thousands-of-synapses.pdf`

## Topic 3 — Thalamocortical routing & gating

- **[Sherman & Usrey 2024]** Transthalamic pathways run parallel to most direct
  corticocortical connections; higher-order (HO) relays receive extra GABAergic control
  (basal ganglia → VA/VL, zona incerta/pretectum → POm/pulvinar), so inhibition can select
  *which* area pairs communicate transthalamically vs direct-only; pulvinar attention raises
  response magnitude and lowers variability of signals passed to cortex.
  *ΩDR*: the thalamic router is literally a bitmask selector: per-pair enable/disable masks
  + top-k selection, gated by the basal-ganglia module's popcount argmax.
  No local file (paywalled); https://www.jneurosci.org/content/44/35/e0909242024
  (doi:10.1523/JNEUROSCI.0909-24.2024)

- **[Mease & Gonzalez 2021]** L5 thick-tufted cells provide strong, sparse *driver* input to
  HO thalamus; extrathalamic inhibition (ZI, APT) plus core/shell TRN segregation gates HO
  loops; HO TC returns target L5A/upper layers (transthalamic) or same region (recurrent).
  *ΩDR*: driver-vs-modulator duality maps to two bit-plane classes: driver bits pass through
  OR-mux, modulator bits only bias thresholds — keeps routing branch-free.
  `papers/winner2021_L5_corticothalamic_higher_order.pdf`
  (file name legacy; verified content = Mease & Gonzalez),
  https://www.frontiersin.org/articles/10.3389/fncir.2021.730211/full (doi:10.3389/fncir.2021.730211)

- **[Ibrahim, Murphy, …, Llano 2021]** All-or-none auditory-cortex population responses are
  gated by layer-6 corticothalamic projections acting via TRN-mediated desynchronization of
  MGB ensembles; silencing L6 or TRN linearizes the stochastic all-or-none responses.
  *ΩDR*: gating acts on *synchrony/coherence* of an ensemble, not firing rate — supports
  selecting whole bundled words (union-of-bindings) rather than individual bits at the router.
  No local file; https://elifesciences.org/articles/56645 (doi:10.7554/eLife.56645)

- **[McCreesh & Cortés 2022]** Control-theoretic analysis of linear-threshold
  thalamocortical networks: feedforward+feedback laws achieve selective inhibition/recruitment;
  the star topology (thalamus as relay) is a failsafe equivalent of the damaged hierarchy.
  *ΩDR*: formal precedent that mask-gated routing has provable convergence guarantees —
  analog of the Lean T1/T3 statements for the router module.
  `papers/mccreesh2022_thalamocortical_inhibition_recruitment.pdf`,
  https://arxiv.org/pdf/2201.00850 (arXiv:2201.00850)

- **[Hummos 2022]** "Thalamus" algorithm: mediodorsal-thalamus-inspired latent vectors are
  updated (task inference) alternately with weight updates, enabling unsupervised task/event
  discovery and switching; motivated by MD-PFC gating and error-feedback CT connections.
  *ΩDR*: fast lane (activity-space context vector, XOR-bound per event) vs slow lane (counter
  SDM weight updates) — exactly the two-timescale memory split.
  `papers/hummos2022_thalamus_task_discovery.pdf`,
  https://arxiv.org/pdf/2205.11713 (arXiv:2205.11713)

- **[Hesselmann et al. 2021]** Thalamic excitability shapes human cortical state: rising
  contextual uncertainty shifts cortex from alpha-rhythmic toward aperiodic (shifted 1/f)
  dynamics via thalamus-centered neuromodulatory influence.
  *ΩDR*: uncertainty scalar ↔ global threshold shift ±2^m (neuromodulator bus row), trading
  exploration (more winners) vs exploitation (fewer).
  `papers/hesselmann2021_thalamic_excitability_uncertainty.pdf`
  (doi:10.1038/s41467-021-22511-7)

## Topic 4 — Hippocampal episodic indexing + entorhinal grid multi-scale periodicity

- **[Gardner et al. 2022]** Toroidal topology confirmed: joint activity of many hundreds of
  Neuropixels-recorded grid cells per module lies on a torus (persistent homology: 1×0D,
  2×1D, 1×2D holes); toroidal coordinates persist across environments and wake/sleep
  (mean cross-environment coordinate distance 31.5°±6.3° vs 135.8°±1.7° shuffled; r=0.79 vs
  0.01); bursting cells carry the code most stably; environment geometry distorts the
  space→torus map, not the torus itself.
  *ΩDR*: each grid module = one rotation clock phase register; the torus is the state space
  of a pair of coprime-period rotations — position = tuple of module phases, decoded by
  POPCOUNT template match. [local]
  `papers/gardner2022_toroidal_topology_grid_cells.pdf`
  (doi:10.1038/s41586-021-04268-7)

- **[Khona, Chandra & Fiete 2021]** Multiscale instability theory: discrete grid modules with
  quantized periods emerge spontaneously from smooth dorsoventral gradients plus a fixed-scale
  lateral interaction; localized eigenmodes have constant periods equal to module periods;
  yields analytic predictions for the whole sequence of period ratios, robust via topology.
  *ΩDR*: justification that a *small set of discrete coprime periods* (not a continuum) is
  natural — the composite cycle length of nested rotations is maximized by choosing periods
  pairwise coprime. `papers/khona2021_grid_modules_multiscale_instability.pdf`,
  https://www.biorxiv.org/content/10.1101/2021.10.28.466284v1.full.pdf
  (doi:10.1101/2021.10.28.466284)

- **[Redman, Acosta-Mendoza, Wei & Goard 2024]** Within-module variability of grid properties
  (spacing/orientation jitter) is robust and *enhances* encoding of local space versus ideal
  identical-module grids. *ΩDR*: keep small per-clock phase offsets in the rotation ensemble;
  heterogeneous rotations improve local resolution the way they improve grid decoding.
  [local] `papers/redman2025_grid_module_variability.pdf` (doi:10.7554/eLife.100652)

- **[Schøyen et al. 2025]** Grid modules as conformal-isometric (CI) maps: ≥7 cells suffice
  for near-perfect CI with optimally arranged phases; a toroidal population code needs only
  ~6 cells with optimized phases (~20 with random phases); at matched rate and cell count the
  hexagonal pattern's encoding manifold scale is 50% larger than a square pattern's,
  giving higher resolution/noise margin. *ΩDR*: hexagonal phase packing ≈ minimal basis of
  three shifted rotation directions; informs how few distinct phase registers per module the
  decoder needs before ambiguity. `papers/schoyen2025_grid_conformal_isometry.pdf`,
  https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1012804
  (doi:10.1371/journal.pcbi.1012804)

- **[Kim & Lim 2021]** Dentate-gyrus WTA dynamics: sparse granule-cell activation (~5%) arises
  from E–I conductance-ratio competition against basket-cell feedback within lamellar
  clusters, sharpened by hilar mossy cells; removing competition raises activation to ~25–33%.
  *ΩDR*: pattern separation = expansion recoding into a much wider field followed by strict
  k-WTA; both steps are threshold/compare ops; supports DG as the hash-spreader feeding the
  hippocampal index. `papers/kim_lim2021_dg_wta_sparse_activation.pdf`,
  https://arxiv.org/pdf/2105.06057 (arXiv:2105.06057)

- **[Tang & Jadhav 2022]** Multi-timescale spatial sequences: the same place-code runs at
  seconds (behavior), ~100–200 ms theta cycles, and 50–200 ms compressed replay during SWRs,
  linking navigation to episodic recall. *ΩDR*: one stored pattern, three readout clocks —
  the rotation-clock hierarchy already models timescale pooling; replay = reverse traversal
  of the permutation sequence (SHL/SHR only). No local file (review);
  https://www.annualreviews.org/content/journals/10.1146/annurev-neuro-111020-084824
  (doi:10.1146/annurev-neuro-111020-084824)

- **[Kang & Balasubramanian 2019]** (edge of window, kept for continuity): attractor
  self-organization yields grid modules with scale ratios clustered at discrete values;
  average ratio constant across hierarchy (reported means 1.42–1.64 across datasets;
  individual pairs span ~1.1–2.0). *ΩDR*: pick consecutive rotation periods with roughly
  geometric spacing; constancy of the average ratio, not exact values, is the constraint.
  [local] `papers/kang_balasubramanian2019_grid_module_selforganization.pdf`
  (doi:10.7554/eLife.46687)

---

## Design implications (for SPEC.md v1)

1. Keep capacity proofs for **constant-weight** codes only; treat inhibition-driven
   variable-k outputs as noise inputs subject to decode-correction (T3), citing iWTA and
   Lehky as evidence real circuits violate fixed-k and independence assumptions.
2. Implement prediction error as XOR + POPCOUNT between predicted and observed words;
   early-exit retrieval when error popcount < t — this is the bitwise analogue of
   dendritic error compartments (Mikulasch; Bicknell & Häusser).
3. Dendritic segments = disjoint bit-field sub-classifiers with local k-WTA (AND/CMP);
   segment count × width is the knob that buys nonlinearity (feature binding) without
   multiplication (Boahen-style ordered synapses unnecessary at our abstraction level).
4. Thalamic router = per-target boolean enable-mask + top-k by POPCOUNT; driver/modulator
   split = separate bit-planes; basal ganglia argmax writes the masks. Star-topology failsafe
   (McCreesh & Cortés) should become a bench scenario (V1: route-around-lesion sweep).
5. Grid/positioning module = ensemble of coprime-period rotation clocks; choose period set
   pairwise coprime with ~geometric spacing (ratios informed by 1.4–1.7 literature range);
   add small per-module phase jitter (Redman) before claiming capacity.
6. Hippocampal index = union-of-bindings store addressed by DG-expanded, strictly k-WTA'd
   keys (~5%-style activity); pattern completion = majority-vote bundle reconstruction;
   pattern separation quality is measurable as pre/post overlap histograms (bench V1).
7. Timescales: one stored trace, multiple readout rates (theta/replay analogy) via SHL/SHR
   traversal direction and step size — no new storage format needed.
8. Energy argument (Levy & Calvert) belongs in the report narrative: ΩDR minimizes the
   communication/comparison cost class that dominates biological budgets; quantify as
   bytes-touched/pattern in bench V1 throughput table.

---

## References

Downloaded this wave (verified %PDF or full-text):
1. Osaulenko & Ulianych 2021, iWTA cell assemblies — https://arxiv.org/pdf/2108.00706
2. McCreesh & Cortés 2022, thalamocortical selective inhibition/recruitment — https://arxiv.org/pdf/2201.00850
3. Hummos 2022, Thalamus task discovery — https://arxiv.org/pdf/2205.11713
4. Kim & Lim 2021, DG WTA sparse activation — https://arxiv.org/pdf/2105.06057
5. Lehky, Tanaka & Sereno 2021, pseudosparse coding — doi:10.1038/s42003-020-01572-2
6. Schøyen et al. 2025, grid conformal isometry — doi:10.1371/journal.pcbi.1012804
7. Khona, Chandra & Fiete 2021, multiscale instability modules — doi:10.1101/2021.10.28.466284
8. Bicknell & Häusser 2021, dendritic learning rule (full text .md) — doi:10.1016/j.neuron.2021.09.044

URL-only citations (no OA file obtainable after retries):
9. Sherman & Usrey 2024 — doi:10.1523/JNEUROSCI.0909-24.2024
10. Ibrahim et al. 2021 — doi:10.7554/eLife.56645
11. Tang & Jadhav 2022 — doi:10.1146/annurev-neuro-111020-084824
12. Schubert & Gros 2021 — doi:10.3389/fncom.2021.718020

Reused local copies from earlier waves: dalgleish2020, millidge2021, jiang_rao2022,
levy_calvert2021, mikulasch2022 (arXiv:2205.05303), golkar2022, hesselmann2021,
winner2021 (=Mease & Gonzalez 2021, doi:10.3389/fncir.2021.730211), gardner2022,
redman2025 (=Redman et al. 2024, doi:10.7554/eLife.100652), kang_balasubramanian2019
(doi:10.7554/eLife.46687), hawkins-ahmad-2016.
