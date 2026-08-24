import OmegaDR.Basic

/-!
# ΩDR multiply-free operational semantics (T5)

The kernel ISA `{NOT, AND, OR, XOR, SHL, SHR, POPCOUNT, CMP, ADD}` with a
static whitelist gate. Zero multiplication, zero division, zero floating
point in all kernel paths. Adapted verbatim from `OmegaDR/Draft.lean`.
-/

namespace OmegaDR

section OperationalSemantics

/-- The kernel opcode set. Exactly nine constructors; anything outside the set is
unrepresentable, which is the primary multiply-free firewall. -/
inductive OpKind where
  | and | or | xor | not | shl | shr | popcnt | cmp | add

/-- Multiply-free whitelist predicate. Every `OpKind` constructor is allowed; if the
enum were ever extended (e.g. with `mul`), this predicate would have to be extended
too and the gate `KernelProgram.cons` would reject the new opcode until audited. -/
def Allowed : OpKind → Prop
  | .and => True
  | .or => True
  | .xor => True
  | .not => True
  | .shl => True
  | .shr => True
  | .popcnt => True
  | .cmp => True
  | .add => True

/-- Every opcode of the fixed ISA lies in the whitelist. -/
theorem opcode_allowed (k : OpKind) : Allowed k := by
  cases k <;> exact trivial

/-- A kernel instruction: opcode, destination register, source registers, immediate. -/
structure Instr (w : Nat) where
  kind : OpKind
  rd : Nat
  rs1 : Nat
  rs2 : Nat
  imm : Nat

/-- Register file: register number maps to a `w`-bit value. -/
abbrev RegFile (w : Nat) := Nat → BitVec w

/-- Single-step operational semantics. Defined **exclusively** via core primitives
`&&&`, `|||`, `^^^`, `~~~`, `<<<`, `>>>`, `BitVec.cpop` (popcount), `BitVec.ult`
(compare) and `BitVec.add`. No `*`, no `/`, no float anywhere in the definition. -/
def step {w : Nat} (ρ : RegFile w) (i : Instr w) : BitVec w :=
  match i.kind with
  | .and    => ρ i.rs1 &&& ρ i.rs2
  | .or     => ρ i.rs1 ||| ρ i.rs2
  | .xor    => ρ i.rs1 ^^^ ρ i.rs2
  | .not    => ~~~(ρ i.rs1)
  | .shl    => ρ i.rs1 <<< i.imm
  | .shr    => ρ i.rs1 >>> i.imm
  | .popcnt => BitVec.cpop (ρ i.rs1)
  | .cmp    => cond (BitVec.ult (ρ i.rs1) (ρ i.rs2)) 1#w 0#w
  | .add    => ρ i.rs1 + ρ i.rs2

/-- Every instruction carries an allowed opcode. -/
theorem instr_allowed (i : Instr w) : Allowed i.kind :=
  opcode_allowed i.kind

/-- Unrestricted programs. -/
abbrev Prog (w : Nat) := List (Instr w)

/-- Every instruction appearing in any program list is allowed. -/
theorem prog_allowed : ∀ (p : Prog w) (i : Instr w), i ∈ p → Allowed i.kind
  | [], _, h => absurd h (by simp)
  | j :: p', i, h => by
      rcases List.mem_cons.mp h with rfl | hm
      · exact instr_allowed i
      · exact prog_allowed p' i hm

/-- Well-formed kernel program: instructions may only be consed on **with a proof**
that their opcode is allowed. This is the static gate of the multiply-free discipline. -/
inductive KernelProgram (w : Nat) where
  | nil : KernelProgram w
  | cons : (i : Instr w) → Allowed i.kind → KernelProgram w → KernelProgram w

namespace KernelProgram

variable {w : Nat}

def Mem (i : Instr w) : KernelProgram w → Prop
  | .nil => False
  | .cons j _ p => i = j ∨ Mem i p

@[reducible]
instance : Membership (Instr w) (KernelProgram w) := ⟨fun p i => Mem i p⟩

/-- **T5 (charter).** Every instruction of a well-formed kernel program carries an
allowed (multiply-free) opcode. -/
theorem allowed_of_mem' :
    ∀ (p : KernelProgram w) (i : Instr w), Mem i p → Allowed i.kind
  | .cons _ hp _, _, Or.inl rfl => hp
  | .cons _ _ p, i, Or.inr h => allowed_of_mem' p i h

theorem allowed_of_mem (p : KernelProgram w) (i : Instr w) (h : i ∈ p) :
    Allowed i.kind :=
  allowed_of_mem' p i h

end KernelProgram

end OperationalSemantics

end OmegaDR
