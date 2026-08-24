# ΩDR — Binding Algebra & Operational Semantics (S5, S6, S7)

**Agent**: math subagent (wave 1) · **Charter**: `docs/spec/00_CHARTER.md` (claims T4, T5)
**Lean artifact**: `lean/OmegaDR/OmegaDR/Draft.lean` (262 lines)
**Toolchain**: Lean 4.33.1 (`leanprover--lean4---v4.33.1`), **core only — no Mathlib**
**Reproduce**:

```sh
export PATH="$HOME/.elan/bin:$PATH"
cd lean/OmegaDR
lake env lean OmegaDR/Draft.lean   # → exit 0; single warning: line 257 uses 'sorry'
lake build                         # → Build completed successfully
```

Full compile output at time of writing:

```
OmegaDR/Draft.lean:257:8: warning: declaration uses `sorry`
exit=0
```

Axiom audit (`#print axioms`, run on an out-of-repo copy): every theorem below depends only on
the standard axioms `[propext, Quot.sound]` or none — **except** the one declared `sorry`
(`rotateRight_add`), which reports `[propext, sorryAx, Quot.sound]`. No other `sorryAx` users.

---

## S5. Binding algebra for XOR/permutation binding on `BitVec n` (charter T4)

Model: role-filler binding is XOR, `bind r f = r ^^^ f`; permutation binding composes a cyclic
permutation, modeled as `permBind k f = f.rotateRight k`.

| Theorem (`OmegaDR.*`) | Statement (abridged) | Status |
|---|---|---|
| `bind_assoc` | `bind r₁ (bind r₂ f) = bind (bind r₁ r₂) f` | ✅ compiles, **no sorry** |
| `bind_comm` | `bind r f = bind f r` (**role-commutativity**) | ✅ compiles, no sorry |
| `bind_id_left` / `bind_id_right` | zero vector is identity on both sides | ✅ compiles, no sorry |
| `unbind_bind` | `bind r (bind r f) = f` (**inverse / unbinding**) | ✅ compiles, no sorry |
| `bind_unbind` | `bind r (bind f r) = f` | ✅ compiles, no sorry |
| `bind_cancel_left` | `bind c a = bind c b ↔ a = b` | ✅ compiles, no sorry |
| `xor_group_laws` | conjunction: associativity ∧ two-sided identity ∧ self-inverse ∧ commutativity | ✅ compiles, no sorry |
| `rotateRight_xor_distrib` | `(x ^^^ y).rotateRight k = x.rotateRight k ^^^ y.rotateRight k` | ✅ compiles, no sorry |
| `permBind_distrib` | permutation binding is a homomorphism of the binding algebra | ✅ compiles, no sorry |

All proofs discharge against core lemmas (`BitVec.xor_assoc/xor_comm/xor_self/xor_zero/
zero_xor/xor_right_inj`, and pointwise `getElem_rotateRight`/`getElem_xor` for distributivity).
`xor_group_laws` is the formal counterpart of charter T4: `(BitVec w, xor, 0)` is an abelian group.
Role-commutativity means roles and fillers are interchangeable operands; combined with
`permBind_distrib`, sparse-permutation bindings can be reordered freely inside bundles.

## S6. Multiply-free operational semantics (charter T5)

Definitions in `Draft.lean` (§OperationalSemantics):

- `inductive OpKind` with exactly nine constructors:
  `and | or | xor | not | shl | shr | popcnt | cmp | add` — anything outside the ISA is
  unrepresentable (primary firewall).
- `def Allowed : OpKind → Prop` — whitelist by cases (all constructors map to `True` today;
  any future extension such as `mul` would be rejected by the gate until audited).
- `structure Instr w` — `kind, rd, rs1, rs2, imm`.
- `def step : RegFile w → Instr w → BitVec w` — operational semantics defined **exclusively**
  via core primitives `&&& ||| ^^^ ~~~ <<< >>>`, `BitVec.cpop` (popcount), `BitVec.ult` (cmp),
  `BitVec.add`. No `*`, `/`, or float appears anywhere in the definition.
- `inductive KernelProgram w` — `cons` carries a **proof argument** `Allowed i.kind`
  (static gate), plus `abbrev Prog w := List (Instr w)` for ungated lists.

| Theorem | Statement | Status |
|---|---|---|
| `opcode_allowed` | `∀ k : OpKind, Allowed k` | ✅ compiles, axiom-free |
| `instr_allowed` | every instruction's opcode is allowed | ✅ compiles, axiom-free |
| `prog_allowed` | every instruction in any program list is allowed | ✅ compiles, no sorry |
| `KernelProgram.allowed_of_mem` | **T5**: every instruction of a gated kernel program carries an allowed opcode | ✅ compiles, axiom-free |

Honest caveat: "multiply-free" is enforced here *by construction* — the deep-embedded `step`
is defined only over the whitelisted opcodes calling only core bitwise primitives, and the gate
theorem is machine-checked. A fully syntactic no-multiplication proof (interpreting `step` in a
deep embedding of arithmetic and proving no `HMul` path exists) is deferred to wave 2.

## S7. Grid-clock periodicity {61, 127, 251} → composite cycle 1944497

Modeling note: the composite-cycle claim is about **clock phases** (each grid clock ticks with
its own modulus). The precise statement proved is: common multiples of pairwise-coprime periods
are exactly the multiples of the product — i.e. the joint phase `(t mod p, t mod q, t mod r)`
returns to origin iff `p·q·r ∣ t`.

| Theorem | Statement | Status |
|---|---|---|
| `joint_period_product` | pairwise coprime `p q r` → `(t % p = 0 ∧ t % q = 0 ∧ t % r = 0) ↔ t % (p*q*r) = 0` | ✅ compiles, **no sorry** (core `Nat.Coprime`, `Nat.coprime_mul_iff_left`, `Nat.Coprime.mul_dvd_of_dvd_of_dvd`, `Nat.dvd_iff_mod_eq_zero`) |
| `grid_periods_pairwise_coprime` | `Coprime 61 127 ∧ Coprime 127 251 ∧ Coprime 61 251` | ✅ compiles, axiom-free (`decide`) |
| `grid_cycle_length` | `61 * 127 * 251 = 1944497` | ✅ compiles, axiom-free (`decide`) |
| `grid_clock_joint_period` | `(t % 61 = 0 ∧ t % 127 = 0 ∧ t % 251 = 0) ↔ t % 1944497 = 0` | ✅ compiles, no sorry |
| `rotateRight_add` | `(x.rotateRight r).rotateRight s = x.rotateRight (r + s)` | ⚠️ statement typechecks; proof is `sorry` |

Why the one `sorry`: Lean 4.33 core ships only pointwise characterizations of `rotateRight`
(`getLsbD_rotateRight`, `getElem_rotateRight`) — there is **no composition lemma in core**, and
re-deriving it requires mod-width index case analysis that is exactly what Mathlib already has
(cf. Mathlib's `BitVec.rotateLeft_rotateLeft` family). Per protocol this is flagged now and
scheduled for wave 2 when the Mathlib dependency lands (see `lean/OmegaDR/SETUP.md`). This also
unblocks the derived facts "rotation order divides w" and "`rotateRight` is a bijection", both
stated nowhere here to avoid unproved claims.

## Summary

- **Compile cleanly (no `sorry`, core-only)**: all 9 S5 theorems, all 4 S6 theorems,
  all 4 Nat-level S7 theorems — 17/18 declarations.
- **Needs Mathlib / currently `sorry`**: 1/18 — `rotateRight_add` (statement verified to
  typecheck; proof deferred).
- Files touched (contract-compliant): `docs/math/algebra_semantics.md` (this file),
  `lean/OmegaDR/OmegaDR/Draft.lean`. No other files modified; `lake build` green.
