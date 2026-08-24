import OmegaDR.Basic

/-!
# ΩDR noise & erasure model (T3 skeleton)

Abstract corruption model for bundled differential decoding (BDD): each item is
stored as m=3 independently-seeded sub-codewords sharing a logical tag; retrieval
sums signed overlaps by integer adds and applies a threshold.

The threshold-monotonicity and erasure-monotonicity lemmas are **proved**
core-only. The probabilistic retrieval-success statement is written in valid
syntax but its proof needs measure theory: NEEDS-MATHLIB (honest `sorry`).
-/

namespace OmegaDR

section Noise

/-- Abstract finitely-supported overlap model for a `w`-lane codeword:
`mass i` is the unnormalized weight of signed overlap `i`, `total` normalizes it.
Probability of an event E is `(∑_{i∈E} mass i) / total`. -/
structure OverlapModel (w : Nat) where
  mass : Nat → Nat
  total : Nat
  total_pos : 0 < total

/-- Windowed lane weight `∑_{j<len} g (lo+j)`. -/
def winSum (g : Nat → Nat) : Nat → Nat → Nat
  | _, 0 => 0
  | lo, len + 1 => g lo + winSum g (lo + 1) len

theorem winSum_zero (g : Nat → Nat) (lo : Nat) : winSum g lo 0 = 0 := by
  simp [winSum]

theorem winSum_succ (g : Nat → Nat) (lo len : Nat) :
    winSum g lo (len + 1) = g lo + winSum g (lo + 1) len := by
  simp [winSum]

/-- Splitting a window at offset `d`: `[a, a+(d+len)) = [a, a+d) ∪ [a+d, a+d+len)`. -/
theorem winSum_split (g : Nat → Nat) (a d len : Nat) :
    winSum g a (d + len) = winSum g a d + winSum g (a + d) len := by
  induction d generalizing a with
  | zero => simp [winSum]
  | succ d ih =>
      have ha : a + 1 + d = a + (d + 1) := by omega
      calc winSum g a ((d + 1) + len)
          = g a + winSum g (a + 1) (d + len) := winSum_succ g a (d + len)
        _ = g a + (winSum g (a + 1) d + winSum g (a + 1 + d) len) := by rw [ih (a + 1)]
        _ = g a + winSum g (a + 1) d + winSum g (a + (d + 1)) len := by
              rw [ha]
              exact (Nat.add_assoc (g a) _ _).symm

/-- Success region: unnormalized probability mass of overlaps clearing θ. -/
def successMass (g : Nat → Nat) (w θ : Nat) : Nat := winSum g θ (w - θ)

/-- **Threshold monotonicity** (the task's core-only piece): raising the retrieval
threshold θ shrinks (never grows) the success region, for ANY overlap distribution. -/
theorem successMass_antitone_theta {g : Nat → Nat} {w a b : Nat} (hab : a ≤ b) :
    successMass g w b ≤ successMass g w a := by
  by_cases hbw : b ≤ w
  · have hsplit : winSum g a (w - a)
        = winSum g a (b - a) + winSum g (a + (b - a)) (w - b) := by
      have hw : w - a = (b - a) + (w - b) := by omega
      rw [hw, winSum_split]
    calc successMass g w b = winSum g b (w - b) := rfl
      _ = winSum g (a + (b - a)) (w - b) := by
            rw [show a + (b - a) = b from by omega]
      _ ≤ winSum g a (b - a) + winSum g (a + (b - a)) (w - b) := Nat.le_add_left _ _
      _ = winSum g a (w - a) := hsplit.symm
      _ = successMass g w a := rfl
  · have hb : w - b = 0 := by omega
    show winSum g b (w - b) ≤ winSum g a (w - a)
    rw [hb, winSum_zero]
    exact Nat.zero_le _

/-- Deterministic worst-case survival: a K-hot codeword hit by `e` erased lanes
still clears threshold θ iff `θ ≤ K - min e K`. -/
def survive (K θ e : Nat) : Prop := θ ≤ K - min e K

/-- Erasure monotonicity: less erasure never breaks survival. -/
theorem survive_mono_erasure {K θ e₁ e₂ : Nat} (h : e₁ ≤ e₂) (hs : survive K θ e₂) :
    survive K θ e₁ := by
  refine Nat.le_trans hs ?_
  have hmin : min e₁ K ≤ min e₂ K := by omega
  omega

/-! ### Pending probabilistic statements -/

/-- **T3 statement (pending formalization).** If the deterministic survival
condition holds under an erasure of `e` lanes and the failure budget ε does not
exceed the normalization total, then retrieval succeeds with probability at
least `1 - ε`.

NEEDS-MATHLIB: requires hypergeometric/Chernoff-type measure-theoretic bounds;
wave 2F plans a core-only Cantelli-style attempt. Until then this is an honest
`sorry`. -/
theorem retrieval_success {w : Nat} (m : OverlapModel w) (K θ e ε : Nat)
    (hK : K ≤ w) (hθ : θ ≤ K) (_hε : ε ≤ m.total)
    (hcond : survive K θ e) :
    m.total - ε ≤ successMass m.mass w θ := sorry

end Noise

end OmegaDR
