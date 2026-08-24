import OmegaDR.Binding

/-!
# ΩDR grid-clock periodicity (T7)

Nested coprime rotations with periods `{61, 127, 251}` model multi-scale entorhinal
grid-cell clocks. Pairwise coprimality makes the composite cycle the **product**:
`61 * 127 * 251 = 1944497` steps.

The clock is realized by iterated `BitVec.rotateRight`; composition of rotations
adds shift amounts (`OmegaDR.rotateRight_add`, proved core-only in `Binding.lean`),
so the composite period acts as the identity rotation.
-/

namespace OmegaDR

section GridClock

/-- Common multiples of pairwise-coprime `p q r` are exactly the multiples of `p*q*r`.
This is the precise sense in which the composite grid clock has period `p*q*r`. -/
theorem joint_period_product {p q r : Nat}
    (hpq : Nat.Coprime p q) (hqr : Nat.Coprime q r) (hpr : Nat.Coprime p r) :
    ∀ t : Nat, (t % p = 0 ∧ t % q = 0 ∧ t % r = 0) ↔ t % (p * q * r) = 0 := by
  intro t
  have hcop : Nat.Coprime (p * q) r := (Nat.coprime_mul_iff_left ..).mpr ⟨hpr, hqr⟩
  have key : p * q * r ∣ t ↔ (p ∣ t ∧ q ∣ t ∧ r ∣ t) := by
    constructor
    · intro h
      refine ⟨Nat.dvd_trans (Nat.dvd_mul_right p q)
          (Nat.dvd_trans (Nat.dvd_mul_right (p * q) r) h),
        Nat.dvd_trans (Nat.dvd_mul_left q p)
          (Nat.dvd_trans (Nat.dvd_mul_right (p * q) r) h),
        Nat.dvd_trans (Nat.dvd_mul_left r (p * q)) h⟩
    · rintro ⟨hp, hq, hr⟩
      exact Nat.Coprime.mul_dvd_of_dvd_of_dvd hcop
        (Nat.Coprime.mul_dvd_of_dvd_of_dvd hpq hp hq) hr
  constructor
  · intro h
    exact Nat.dvd_iff_mod_eq_zero.mp (key.mpr
      ⟨Nat.dvd_iff_mod_eq_zero.mpr h.1, Nat.dvd_iff_mod_eq_zero.mpr h.2.1,
       Nat.dvd_iff_mod_eq_zero.mpr h.2.2⟩)
  · intro h
    obtain ⟨hp, hq, hr⟩ := key.mp (Nat.dvd_iff_mod_eq_zero.mpr h)
    exact ⟨Nat.dvd_iff_mod_eq_zero.mp hp, Nat.dvd_iff_mod_eq_zero.mp hq,
      Nat.dvd_iff_mod_eq_zero.mp hr⟩

/-- The ΩDR grid periods are pairwise coprime (computed by `decide`). -/
theorem grid_periods_pairwise_coprime :
    Nat.Coprime 61 127 ∧ Nat.Coprime 127 251 ∧ Nat.Coprime 61 251 :=
  ⟨by decide, by decide, by decide⟩

/-- Composite cycle length: `61 * 127 * 251 = 1944497`. -/
theorem grid_cycle_length : 61 * 127 * 251 = 1944497 := by decide

/-- The ΩDR grid-clock composite period is the product `1944497`. -/
theorem grid_clock_joint_period :
    ∀ t : Nat, (t % 61 = 0 ∧ t % 127 = 0 ∧ t % 251 = 0) ↔ t % 1944497 = 0 := by
  rw [show (1944497 : Nat) = 61 * 127 * 251 from grid_cycle_length.symm]
  exact joint_period_product (by decide) (by decide) (by decide)

/-- Realization: the composite grid clock acting on a `1944497`-lane phase vector is
the identity rotation - one full cycle returns every lane to its origin. -/
theorem grid_clock_full_rotation (x : BitVec 1944497) :
    x.rotateRight 1944497 = x :=
  rotateRight_full_cycle x

/-- Realization: nested rotations compose by adding shift amounts, so the three
grid modules' combined phase displacement is the sum of individual displacements. -/
theorem grid_clock_rotation_composition (x : BitVec 1944497) (r s : Nat) :
    (x.rotateRight r).rotateRight s = x.rotateRight (r + s) :=
  rotateRight_add x r s

end GridClock

end OmegaDR
