# neuro_findings_B — topics 5–8 (ΩDR research wave, agent B)

Scope: cerebellar expansion recoding + supervised fast learning; basal ganglia
action selection; neuromodulators as global scalars; brain-scale efficiency
facts. Literature 2020–2026 preferred; canonical pre-2020 anchors included where
they carry quantitative facts. All files referenced live in `papers/`.
Compiled 2026-08-24. RESEARCH ONLY.

---

## (5) Cerebellar expansion recoding & supervised fast learning

- **[Sanger, Yamashita & Kawato 2020]** — 50-year update of the Marr–Albus codon
  theory: mossy-fiber inputs are expanded by granule cells (GrC) so that any
  nonlinear input–output map becomes a LINEAR combination at Purkinje cells (PC);
  supervised learning is confined to PF–PC synapses driven by climbing-fiber
  instructive signals. Six organizational principles: feature engineering at the
  input layer, massively recurrent circuitry, linear readout, regulated/predictive
  instructive signals, multi-timescale plasticity, task-specific hardware.
  ΩDR: justifies the `hash spreader → linear/perceptron readout` split — expansion
  must be random-sparse and fixed; all learning lives in few-bit weights.
  File: papers/sanger2020_expansion_coding_cerebellum.pdf |
  https://doi.org/10.1113/JP278745

- **[Bae et al. 2022]** — GrCs as kernel basis functions: each GrC ≈ tuned kernel
  over mossy-fiber space ("lazy" random projection vs "rich"/SVM-like regimes);
  sparse GrC activity increases pattern-separation learning speed and decorrelates
  populations. ΩDR: expansion recoding = implicit kernel machine; ΩDR's sparse
  conjunction hashing reproduces the lazy regime with AND/XOR ops only.
  File: papers/cerebellar_kernel_bae2022.pdf | https://doi.org/10.3389/fncom.2022.1062392

- **[Lanore et al. 2021]** — in-vivo two-photon of hundreds of GrC AXONS:
  parallel-fiber population activity IS high-dimensional and spatially distributed,
  and distinct behavioral states occupy orthogonal subspaces — resolving the earlier
  "low-dimensional GrC activity" reports in favor of Marr/Albus pattern separation.
  ΩDR: empirical support that an expanded sparse ternary code keeps states
  near-orthogonal (signed-overlap ≈ 0) even under behaviorally correlated inputs.
  File: papers/granule_axons_lanore2021.md | https://doi.org/10.1038/s41593-021-00873-x

- **[Gilmer & Person 2017]** — anatomical constraint: each GrC samples ~4 mossy-fiber
  rosettes via ~4 short dendrites; combinatorial diversity saturates quickly, favoring
  dense local sampling + temporal diversification. ΩDR: cap conjunction fan-in at
  k≈4 inputs per hash slot — wider conjunctions buy no capacity in realistic layouts.
  URL: https://www.jneurosci.org/content/37/50/12153

## (6) Basal ganglia action selection (WTA, gating)

- **[Zhu et al. 2024]** — cortico-BG model with TAN(ChI)–DA interaction: direct
  pathway promotes, indirect suppresses, hyperdirect STOPS; action committed via
  TAN-DA reinforcement mechanism that transfers to Hebbian (habit) control;
  dopamine deficiency breaks reinforcement (parkinsonism), levodopa partially
  restores it. ΩDR: three-op semantics {promote, veto, global-stop} map to
  per-channel threshold bias + one global abort line; DA-gated→Hebbian transfer
  = LR annealing schedule.
  File: papers/bg_tanda_zhu2024.md | https://doi.org/10.1007/s11571-023-10046-0

- **[Simulated DA modulation of a neurorobotic BG model 2024]** — tonic DA level
  in an embedded BG robot controller shifts selection regime: low DA biases toward
  indirect-pathway suppression/exploration deficits, high DA toward direct-pathway
  commitment; confirms DA as a closed-loop scalar controlling selection sharpness,
  not just a training signal. ΩDR: DA scalar = runtime knob on WTA margin/threshold
  (±2^m), validated as a control loop.
  File: papers/bg_neurorobotic_da2024.pdf |
  https://pmc.ncbi.nlm.nih.gov/articles/PMC10967936/

- **[Chen et al. 2024]** — within-subject pharmacology in mice on a restless bandit:
  DA bidirectionally sets exploration level (↑DA ⇒ ↓exploration via decision noise /
  inverse temperature β); NE sets outcome sensitivity instead. BG-relevant because
  striatal DA gates D1(go)/D2(no-go) channel competition during choice.
  ΩDR: exploration temperature implemented as shift-controlled noise injection into
  popcount margins before argmax.
  File: papers/explore_exploit_dane_chen2024.md |
  https://doi.org/10.1523/JNEUROSCI.1194-23.2024

- **Architecture facts** (BG as disinhibition/WTA): tonically active GABAergic GPi/SNr
  output holds thalamus inhibited; striatal activation releases exactly one channel
  ("releasing the brakes"); striatal fast-spiking interneurons implement lateral
  inhibition yielding winner-take-all among MSN channels; dopamine scales the
  direct-vs-indirect balance by receptor type. ΩDR: popcount-argmax over channels +
  default-inhibit output register is the bitwise analogue.
  Supporting: [Gurney, Prescott & Redgrave 2001 lineage, cited throughout Zhu 2024];
  spiking instantiation with overlapping pathways: Girard, Lienard & Gutierrez 2020
  (Neural Netw), summarized in bg_tanda_zhu2024.md references.

## (7) Neuromodulators as global scalar signals

- **DA = learning rate / gating / choice precision** — phasic DA carries reward
  prediction error (Schultz lineage); pharmacology shows DA also scales decision
  noise (β) and BG gate openness [Chen 2024; Zhu 2024; neurorobotic 2024 above].
  ΩDR: DA register scales counter-update step (shift-LR) and WTA margin.
- **ACh = surprise / reset / orienting** — ACh tracks attentional effort, orienting,
  detection of behaviorally significant stimuli [PMC review 2022];
  surprise taxonomy formalized by **[Modirshanechi et al. 2023]**: separate
  *unexpectedness* (Bayesian/KL surprise about predictions) from *unfamiliarity*
  (novelty); different physiological signals (pupil, DA transients, cortical state)
  attach to different definitions. ΩDR: compute overlap-based surprise between
  predicted and actual bundles; ACh register triggers state reset or transient LR
  boost — keep novelty (hash-unfamiliarity) as a second, distinct signal.
  Files: papers/modirshanechi2023_surprise_novelty_brain.pdf |
  https://doi.org/10.1016/j.conb.2023.102758 ;
  review: https://pmc.ncbi.nlm.nih.gov/articles/PMC9320657/ ;
  classic: Yu & Dayan 2005, Neural Comput 17(3):533–583,
  https://doi.org/10.1162/neco.2005.17.3.533
- **NE = gain / exploration** — adaptive-gain theory: LC-NE phasic mode supports
  exploitation/task focus, tonic mode exploration/disengagement; catecholamines
  raise network gain & SNR [Aston-Jones & Cohen 2005]; modern pharmacology splits
  NE's role to outcome sensitivity [Chen 2024]. ΩDR: NE register = arithmetic-shift
  gain applied to similarity margins before CMP (branch-free).
  https://doi.org/10.1146/annurev.neuro.28.061604.135709
- **5HT = timescales / patience** — DRN stimulation prolongs waiting for delayed
  reward (~30% longer give-up latency); effect strongest at high reward-timing
  uncertainty; modeling as raising the prior probability of future reward fits the
  data better than pure temporal-discounting change — i.e., 5HT tunes the
  timescale/confidence horizon, not immediate value. ΩDR: 5HT register scales
  rotation-period selection (multi-period clocks) and decay constants of counters.
  File: papers/serotonin_patience_miyazaki2018.pdf | https://doi.org/10.1038/s41467-018-04496-y
- **Broadcast mechanics justify a scalar bus** — monoamine volume transmission is
  extrasynaptic and diffuse: release sites without synaptic appositions, slow
  spatial spread, receptor placement determines effect; firing modes are coarse
  (e.g., serotonergic neurons fire tonically 0.03–3 Hz, bursts up to ~17 Hz).
  ΩDR: whole-framework neuromodulation as four shared registers (threshold shifts
  ±2^m), not per-synapse parameters.
  File: papers/neuromod_volume_transmission2024.pdf | https://doi.org/10.1038/s41380-024-02608-3

## (8) Brain-scale efficiency facts

- **Energy budget numbers** — human brain ≈ 20 W (~20% of body energy at ~2% of body
  mass), with a roughly FIXED energy budget per neuron across species; cerebral-cortex
  neurons cost ≥10× cerebellar neurons. ΩDR: module-level budgets should mirror this —
  cheap massively-parallel expanders (cerebellum-like) are affordable, expensive
  flexible cortex-like pools are not.
  File: papers/energy_per_neuron_herculano2011.pdf | https://doi.org/10.1371/journal.pone.0017514
- **Communication ≫ computation** — cortical energy audit: computation proper costs
  only ~0.1 W while long-distance communication costs ~3.5 W (35×); communication
  alone accounts for ~two-thirds of the available ~4.94 ATP-W. ΩDR: minimize data
  movement; POPCNT-local kernels and packed uint64_t words directly attack the
  dominant cost term.
  File: papers/levy_calvert2021_energy_audit_cortex.pdf | https://doi.org/10.1073/pnas.2008173118
- **Sparse, event-driven coding** — electrical signalling dominates brain energy
  (excitatory synaptic signalling ~57% + action potentials ~23% of signalling budget);
  bits-per-ATP is optimized at LOW mean rates (≤~10 Hz observed; pushing to
  information-maximizing ~200 Hz would cost ≥20× more ATP); cortical glutamate
  release probability 25–50% sits at the energetic optimum; perception survives with
  information carried by very few neurons (few-sparse codes). ΩDR: keep active set
  small (k=40/2048 ≈ 2% active) and evaluate lazily — event-driven POPCNT only on
  nonzero words.
  Files: papers/dalgleish2020_few_neurons_perception_sparse_coding.pdf |
  https://doi.org/10.7554/eLife.58889 ;
  energy partition source (no local file): "Paying the brain's energy bill",
  Curr Opin Behav Sci 2023, https://doi.org/10.1016/j.conb.2022.102668 ;
  see also papers/lehky2021_pseudosparse_coding.pdf
- **Low effective precision (~4–8 bits)** — synaptic strengths carry only ~26
  distinguishable levels ≈ 4.7 bits (CA1 EM reconstruction), and observed precision
  requires averaging over minutes due to stochastic vesicle release [Bartol et al.
  2015]; information-theoretic reanalysis gives lower bound 4.1 bits / upper bound
  4.59 bits from 24 distinguishable sizes [Samavat/Bartol et al. 2024]. ΩDR:
  2-bit saturating counters + sparse ternary weights sit inside the biological
  envelope; do not spend bits on weight precision — spend them on n (width).
  Files: papers/bartol2015_synaptic_bits.pdf | https://doi.org/10.7554/eLife.10778 ;
  papers/synaptic_info_capacity_samavat2024.pdf |
  https://direct.mit.edu/neco/article/36/5/781/120323
- **Engineering echoes** — quantization+pruning keeps SNN accuracy while cutting
  spikes/energy [Schaefer et al. 2023]; presynaptically stochastic consolidation
  improves lifelong-learning retention at low cost [Schug et al. 2021]; inhibitory
  WTA cell assemblies with binary Hebbian weights + pruning preserve similarity
  structure like k-WTA but adapt active-set size [Osaulenko et al. 2021].
  ΩDR: stochasticity in updates is a feature (free noise-averaging), pruning keeps
  address tables sparse.
  Files: papers/schaefer2023_snn_quantization_pruning.pdf ;
  papers/schug2021_presynaptic_stochasticity_energy.pdf ;
  papers/osaulenko2021_iwta_cell_assemblies.pdf

---

## Design implications for ΩDR

1. Expansion before readout: fixed random-sparse conjunction layer (fan-in ≈4,
   GrC-style) mapping n→m≫n; learn ONLY in the readout (PF–PC analogue) with
   perceptron-style shift-LR on saturating counters.
2. Keep expanded codes near-orthogonal by construction (signed overlap ≈0);
   behavioral/context subspaces should land on disjoint bit blocks (Lanore's
   orthogonal-state finding).
3. Action layer = popcount argmax over channels with default-inhibit output
   (disinhibition), per-channel promote/suppress thresholds, one hyperdirect
   global-stop line.
4. Four neuromodulator registers as global scalars, branch-free: DA→LR step &
   WTA margin; NE→margin gain (SHL before CMP); ACh→surprise/reset + transient LR
   boost; 5HT→timescale/decay & rotation-period selection. Threshold shifts ±2^m.
5. Surprise ≠ novelty: track both prediction-mismatch (KL-like, from overlap
   statistics) and unfamiliarity (hash-table miss rate) separately.
6. Precision discipline: ≤6 bits/synaptic state (2-bit counters + ternary weights);
   inject stochastic rounding/noise in updates rather than precision.
7. Energy discipline mirrors biology: communication dominates (35×) — prefer
   register-resident bitwise ops; target ≤~10% active fraction per evaluation;
   lazy/event-driven POPCNT.
8. Anneal gating: DA-gated plasticity should decay into Hebbian (habit) weights,
   matching TAN-DA→Hebbian transfer; exploration noise scaled inversely with
   confidence.

---

## References

Downloaded this session (PDF verified `%PDF`):
1. Bae H, Park S-Y, Kim SJ, Kim C-E (2022) Front Comput Neurosci 16:1062392.
   doi:10.3389/fncom.2022.1062392 — papers/cerebellar_kernel_bae2022.pdf
2. Levy WB, Calvert VG (2021) PNAS 118(18):e2008173118. doi:10.1073/pnas.2008173118
   — papers/levy_calvert2021_energy_audit_cortex.pdf (pre-existing)
3. Herculano-Houzel S (2011) PLoS ONE 6(3):e17514. doi:10.1371/journal.pone.0017514
   — papers/energy_per_neuron_herculano2011.pdf
4. Miyazaki K et al. (2018) Nat Commun. doi:10.1038/s41467-018-04496-y
   — papers/serotonin_patience_miyazaki2018.pdf
5. Bartol TM et al. (2015) eLife 4:e10778. doi:10.7554/eLife.10778
   — papers/bartol2015_synaptic_bits.pdf
6. Samavat M, Bartol TM et al. (2024) Neural Computation 36(5):781–802.
   — papers/synaptic_info_capacity_samavat2024.pdf
7. Simulated Dopamine Modulation of a Neurorobotic Model of the Basal Ganglia
   (2024) PMC10967936 — papers/bg_neurorobotic_da2024.pdf
8. Modirshanechi A, Becker S, Brea J, Gerstner W (2023) Curr Opin Neurobiol
   82:102758. doi:10.1016/j.conb.2023.102758
   — papers/modirshanechi2023_surprise_novelty_brain.pdf (pre-existing)

Markdown fallbacks saved (publisher blocked PDF):
9. Chen CS et al. (2024) J Neurosci 44(44):e1194232024.
   doi:10.1523/JNEUROSCI.1194-23.2024 — papers/explore_exploit_dane_chen2024.md
10. Zhu Q et al. (2024) Cogn Neurodyn 18:2127–2144. doi:10.1007/s11571-023-10046-0
    — papers/bg_tanda_zhu2024.md
11. Lanore F et al. (2021) Nat Neurosci 24:1142–1150. doi:10.1038/s41593-021-00873-x
    — papers/granule_axons_lanore2021.md

Pre-existing papers/ PDFs cited (agent A):
12. Sanger TD, Yamashita O, Kawato M (2020) J Physiol 598(5):913–928.
    doi:10.1113/JP278745 — papers/sanger2020_expansion_coding_cerebellum.pdf
13. Dalgleish HW et al. (2020) eLife. doi:10.7554/eLife.58889
    — papers/dalgleish2020_few_neurons_perception_sparse_coding.pdf
14. Schaefer et al. (2023) SNN quantization/pruning
    — papers/schaefer2023_snn_quantization_pruning.pdf
15. Schug et al. (2021) presynaptic stochasticity & consolidation
    — papers/schug2021_presynaptic_stochasticity_energy.pdf
16. Osaulenko et al. (2021) iWTA cell assemblies (preprint)
    — papers/osaulenko2021_iwta_cell_assemblies.pdf
17. Kim, Lim (2021) DG WTA sparse activation — papers/kim_lim2021_dg_wta_sparse_activation.pdf
18. Lehky et al. (2021) pseudosparse coding — papers/lehky2021_pseudosparse_coding.pdf

URL-only citations:
19. Aston-Jones G, Cohen JD (2005) Annu Rev Neurosci 28:403–450.
    doi:10.1146/annurev.neuro.28.061604.135709
20. Yu AJ, Dayan P (2005) Neural Comput 17(3):533–583. doi:10.1162/neco.2005.17.3.533
21. Gilmer JI, Person AL (2017) J Neurosci 37(50):12153.
    https://www.jneurosci.org/content/37/50/12153
22. Paying the brain's energy bill (2023) Curr Opin Behav Sci.
    doi:10.1016/j.conb.2022.102668
23. Cholinergic/noradrenergic modulation review (2022)
    https://pmc.ncbi.nlm.nih.gov/articles/PMC9320657/
