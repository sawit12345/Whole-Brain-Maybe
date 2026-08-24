import OmegaDR.Basic

/-!
# ΩDR binding algebra (T4)

XOR/permutation binding on `BitVec w`: group laws, permutation distributivity,
and composition laws for cyclic rotations. Adapted from `OmegaDR/Draft.lean`
(wave 1 archive); `rotateRight_add` is now **proved** in core Lean 4.33.1 via
the pointwise `getLsbD` characterization (`BitVec.getLsbD_rotateRight`) and
`BitVec.eq_of_getLsbD_eq` - no Mathlib needed.
-/

namespace OmegaDR

/-- Role-filler binding: bitwise XOR of role and filler. -/
def bind {w : Nat} (r f : BitVec w) : BitVec w := r ^^^ f

/-- Permutation binding: apply a cyclic permutation (rotation) to the filler. -/
def permBind {w : Nat} (k : Nat) (f : BitVec w) : BitVec w := f.rotateRight k

section BindingAlgebra

variable {w : Nat}

/-- **Associativity.** -/
theorem bind_assoc (r₁ r₂ f : BitVec w) :
    bind r₁ (bind r₂ f) = bind (bind r₁ r₂) f := by
  simp [bind, BitVec.xor_assoc]

/-- **Role-commutativity**: roles and fillers enter symmetrically, so the same
operation serves as role-filler binding in either argument order. -/
theorem bind_comm (r f : BitVec w) : bind r f = bind f r := BitVec.xor_comm r f

/-- **Identity**: binding with the zero vector is the identity on fillers. -/
theorem bind_id_left (f : BitVec w) : bind 0#w f = f := by
  simp [bind]

theorem bind_id_right (r : BitVec w) : bind r 0#w = r := by
  simp [bind]

/-- **Inverse**: every element is its own inverse; unbinding recovers the filler. -/
theorem unbind_bind (r f : BitVec w) : bind r (bind r f) = f := by
  rw [bind, bind, ← BitVec.xor_assoc, BitVec.xor_self, BitVec.zero_xor]

/-- Unbinding with swapped argument order also recovers the filler. -/
theorem bind_unbind (r f : BitVec w) : bind r (bind f r) = f := by
  rw [bind, bind, BitVec.xor_comm f r, ← BitVec.xor_assoc, BitVec.xor_self, BitVec.zero_xor]

/-- Left cancellation (consequence of inverses). -/
theorem bind_cancel_left (c a b : BitVec w) : bind c a = bind c b ↔ a = b := by
  simp [bind]

/-- **T4 (charter).** `(BitVec w, xor, 0)` is an abelian group: the four group laws
as a single conjunction, each discharged by a core lemma. -/
theorem xor_group_laws (w : Nat) :
    (∀ x y z : BitVec w, (x ^^^ y) ^^^ z = x ^^^ (y ^^^ z)) ∧
    (∀ x : BitVec w, 0#w ^^^ x = x ∧ x ^^^ 0#w = x) ∧
    (∀ x : BitVec w, x ^^^ x = 0#w) ∧
    (∀ x y : BitVec w, x ^^^ y = y ^^^ x) :=
  ⟨fun x y z => BitVec.xor_assoc x y z,
   fun x => ⟨BitVec.zero_xor (x := x), BitVec.xor_zero (x := x)⟩,
   fun x => BitVec.xor_self (x := x),
   fun x y => BitVec.xor_comm x y⟩

/-- Cyclic permutations distribute over XOR: binding commutes with permutation.
This is the algebraic reason sparse-permutation bindings can be reordered freely. -/
theorem rotateRight_xor_distrib (x y : BitVec w) (k : Nat) :
    (x ^^^ y).rotateRight k = x.rotateRight k ^^^ y.rotateRight k := by
  ext i hi
  rw [BitVec.getElem_xor]
  rw [BitVec.getElem_rotateRight hi, BitVec.getElem_rotateRight hi,
    BitVec.getElem_rotateRight hi]
  split <;> simp_all [BitVec.getElem_xor]

/-- Permutation binding is a homomorphism of the XOR binding algebra. -/
theorem permBind_distrib (k : Nat) (r f : BitVec w) :
    permBind k (bind r f) = bind (permBind k r) (permBind k f) := by
  simp [bind, permBind, rotateRight_xor_distrib]

end BindingAlgebra

/-! ### Rotation composition (formerly the wave-1 `sorry`) -/

section RotationComposition

/-- Pointwise characterization of `rotateRight` for in-range bit indices. -/
private theorem getLsbD_rotateRight_eq {w : Nat} {x : BitVec w} {r i : Nat}
    (hi : i < w) :
    (x.rotateRight r).getLsbD i = x.getLsbD ((i + r) % w) := by
  rcases Nat.eq_zero_or_pos w with rfl | hw0
  · omega
  have hrw : r % w < w := Nat.mod_lt _ hw0
  rw [BitVec.getLsbD_rotateRight]
  by_cases h : i < w - r % w
  · rw [decide_eq_true h, cond_true]
    have hlt : i + r % w < w := by omega
    congr 1
    rw [Nat.add_comm (r % w) i, ← Nat.add_mod_mod]
    exact (Nat.mod_eq_of_lt hlt).symm
  · have hlt : i - (w - r % w) < w := by omega
    have hid : i + r % w = i - (w - r % w) + w := by omega
    rw [decide_eq_false h, cond_false, decide_eq_true hi, Bool.true_and]
    congr 1
    rw [← Nat.add_mod_mod, hid, Nat.add_mod_right, Nat.mod_eq_of_lt hlt]

/-- Composition of rotations adds shift amounts (mod the width).
Formerly `sorry` in `OmegaDR/Draft.lean`; proved core-only. -/
theorem rotateRight_add {w : Nat} (x : BitVec w) (r s : Nat) :
    (x.rotateRight r).rotateRight s = x.rotateRight (r + s) := by
  refine BitVec.eq_of_getLsbD_eq fun i hi => ?_
  have wp : 0 < w := Nat.pos_of_ne_zero (by omega)
  rw [getLsbD_rotateRight_eq hi, getLsbD_rotateRight_eq (Nat.mod_lt _ wp),
    getLsbD_rotateRight_eq hi]
  have heq : ((i + s) % w + r) % w = (i + (r + s)) % w := by
    rw [Nat.mod_add_mod]
    congr 1
    omega
  rw [heq]

/-- Rotating by the full width is the identity permutation. -/
theorem rotateRight_full_cycle {w : Nat} (x : BitVec w) : x.rotateRight w = x := by
  refine BitVec.eq_of_getLsbD_eq fun i hi => ?_
  have wp : 0 < w := Nat.pos_of_ne_zero (by omega)
  rw [getLsbD_rotateRight_eq hi, Nat.add_mod_right]
  exact congrArg x.getLsbD (Nat.mod_eq_of_lt hi)

end RotationComposition

end OmegaDR
