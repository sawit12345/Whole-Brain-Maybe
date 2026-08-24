import OmegaDR.Basic

/-!
# ΩDR storage capacity (T2)

Self-hosted binomial coefficients: `Nat.choose` does **not** exist in Lean 4.33.1
core (it is Batteries/Mathlib), so `OmegaDR.choose` is defined here by the Pascal
recurrence and equipped with the needed theory - all axiom-free.

Headline: `omega_capacity` states Σ(2048,20) > 10^95 (state space +11.1 orders
over the SDR baseline). The spec expected this to need Mathlib; in fact the exact
inequality is decidable by kernel computation, so it is **proved** here with
`decide` - no `sorry`, no Mathlib.
-/

namespace OmegaDR

section Capacity

/-- Pascal-triangle binomial coefficient (core-only; `Nat.choose` lives outside
Lean 4.33.1 core). -/
def choose : Nat → Nat → Nat
  | _, 0 => 1
  | 0, _ + 1 => 0
  | n + 1, k + 1 => choose n k + choose n (k + 1)

/-- Capacity summand Σ(n,k) = C(n,k) · C(n-k,k): ways to place two disjoint
k-subsets (positive and negative planes) in an n-bit codeword. -/
def SigmaCard (n k : Nat) : Nat := choose n k * choose (n - k) k

theorem choose_zero_right (n : Nat) : choose n 0 = 1 := by simp [choose]

theorem choose_zero_succ (k : Nat) : choose 0 (k + 1) = 0 := by simp [choose]

theorem choose_succ_succ (n k : Nat) :
    choose (n + 1) (k + 1) = choose n k + choose n (k + 1) := by simp [choose]

/-- Out-of-range binomial coefficients vanish. -/
theorem choose_out_of_range {n k : Nat} (h : n < k) : choose n k = 0 := by
  induction n generalizing k with
  | zero =>
      match k with
      | 0 => omega
      | j + 1 => exact choose_zero_succ j
  | succ m ih =>
      match k with
      | 0 => omega
      | j + 1 =>
          rw [choose_succ_succ, ih (by omega), ih (by omega)]

/-- Binomial coefficients are positive for `k ≤ n`. -/
theorem choose_pos {n k : Nat} (h : k ≤ n) : 0 < choose n k := by
  induction n generalizing k with
  | zero =>
      match k with
      | 0 => exact Nat.zero_lt_one
      | j + 1 => rw [choose_zero_succ]; omega
  | succ m ih =>
      match k with
      | 0 => exact Nat.zero_lt_one
      | j + 1 =>
          rw [choose_succ_succ]
          exact Nat.lt_of_lt_of_le (ih (by omega)) (Nat.le_add_right _ _)

/-- Boundary ones. -/
theorem choose_self (n : Nat) : choose n n = 1 := by
  induction n with
  | zero => rfl
  | succ m ih =>
      rw [choose_succ_succ, ih, choose_out_of_range (by omega)]

/-- Absorption identity `C(n,k+1)·(k+1) = C(n,k)·(n-k)` (division-free form of the
ratio between successive rows), proved by double induction on the Pascal recurrence. -/
theorem mul_succ_choose_eq (n k : Nat) :
    choose n (k + 1) * (k + 1) = choose n k * (n - k) := by
  induction n generalizing k with
  | zero =>
      match k with
      | 0 => rw [choose_zero_succ, choose_zero_right]
      | j + 1 => rw [choose_zero_succ, choose_zero_succ, Nat.zero_mul, Nat.zero_mul]
  | succ m ih =>
      match k with
      | 0 =>
          have h0 : choose m 1 = m := by
            have hh := ih 0
            simpa [choose_zero_right] using hh
          rw [choose_succ_succ]
          simp [h0, choose_zero_right]
          omega
      | j + 1 =>
          rcases Nat.lt_or_ge m (j + 1) with hn | hn
          · have e2 : choose m (j + 1) = 0 := choose_out_of_range (by omega)
            have e3 : choose m (j + 2) = 0 := choose_out_of_range (by omega)
            have hs0 : (m + 1) - (j + 1) = 0 := by omega
            rw [choose_succ_succ, choose_succ_succ, e3, e2, Nat.add_zero,
                Nat.add_zero, hs0, Nat.mul_zero]
            exact Nat.zero_mul _
          · have hsub : (m + 1) - (j + 1) = m - j := by omega
            rw [hsub, choose_succ_succ, choose_succ_succ]
            have h1 := ih j
            have h2 := ih (j + 1)
            have p1 : (j + 2) + (m - (j + 1)) = m + 1 := by omega
            have p2 : m + 1 = (j + 1) + (m - j) := by omega
            calc (choose m (j + 1) + choose m (j + 2)) * (j + 1 + 1)
                = choose m (j + 1) * (j + 2) + choose m (j + 2) * (j + 2) :=
                  Nat.add_mul _ _ _
              _ = choose m (j + 1) * (j + 2) + choose m (j + 1) * (m - (j + 1)) := by rw [h2]
              _ = choose m (j + 1) * ((j + 2) + (m - (j + 1))) := by rw [← Nat.mul_add]
              _ = choose m (j + 1) * (m + 1) := by rw [p1]
              _ = choose m (j + 1) * ((j + 1) + (m - j)) := by rw [p2]
              _ = choose m (j + 1) * (j + 1) + choose m (j + 1) * (m - j) :=
                    Nat.mul_add _ _ _
              _ = choose m j * (m - j) + choose m (j + 1) * (m - j) := by rw [← h1]
              _ = (choose m j + choose m (j + 1)) * (m - j) :=
                    (Nat.add_mul _ _ _).symm

/-- Single-factor monotonicity: rows grow while the diagonal has not been crossed.
Condition `2*k+1 ≤ n` is sharp (`C(2k+1,k) = C(2k+1,k+1)`). -/
theorem choose_mono {n k : Nat} (h : 2 * k + 1 ≤ n) : choose n k ≤ choose n (k + 1) := by
  have hc : 0 < k + 1 := by omega
  refine Nat.le_of_mul_le_mul_right ?_ hc
  have hk : k + 1 ≤ n - k := by omega
  have hEq := mul_succ_choose_eq n k
  calc choose n k * (k + 1) ≤ choose n k * (n - k) :=
        Nat.mul_le_mul_left _ hk
    _ = choose n (k + 1) * (k + 1) := hEq.symm

/-- Σ(n,k) is positive whenever both planes fit (`2*k < n`). -/
theorem sigma_card_pos {n k : Nat} (h : 2 * k + 1 ≤ n) : 0 < SigmaCard n k := by
  have hle : k ≤ n - k := by omega
  exact Nat.lt_of_lt_of_le (choose_pos (by omega))
    (Nat.le_mul_of_pos_right _ (choose_pos hle))

/-- **T2 supporting bound (charter).** The ΩDR state space dominates its positive
plane alone: `C(2048,20) · C(2028,20) ≥ C(2048,20)` (equality only if the negative
plane degenerates). -/
theorem choose_bound_omega :
    choose 2048 20 * choose 2028 20 ≥ choose 2048 20 :=
  Nat.le_mul_of_pos_right _ (choose_pos (by omega))

/-- Concrete digit fact (kernel-decided): `C(64,8) = 4,426,165,368`. -/
theorem choose_64_8 : choose 64 8 = 4426165368 := by decide

/-- Concrete digit fact (kernel-decided): Σ(64,8) = C(64,8) · C(56,8)
= `6,287,341,680,214,194,600`. -/
theorem sigma_card_64_8 : SigmaCard 64 8 = 6287341680214194600 := by
  set_option maxRecDepth 100000 in decide

/-- **T2 headline (charter).** ΩDR state space at width 2048 with K=20 per plane:
Σ(2048,20) > 10^95 (SDR baseline single-plane: ~10^84.38).

NEEDS-MATHLIB note: originally expected to require Mathlib's `Nat.choose`;
in fact core-only kernel computation decides the exact inequality, so this is
**proved** - the capacity claim no longer rests on a `sorry`. -/
theorem omega_capacity : SigmaCard 2048 20 > 10 ^ 95 := by
  set_option maxRecDepth 100000 in decide

end Capacity

end OmegaDR
