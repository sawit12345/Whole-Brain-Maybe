import OmegaDR.Basic
import OmegaDR.Binding
import OmegaDR.ISA
import OmegaDR.Grid
import OmegaDR.Capacity
import OmegaDR.Noise

/-!
# ΩDR root module

Re-exports all ΩDR formalization modules and prints the axiom audit for every
public theorem. Sorries appear only in `OmegaDR.Noise.retrieval_success`
(NEEDS-MATHLIB, probability theory); every other declaration is axiom-free.
-/

namespace OmegaDR

-- Binding (T4)
#print axioms OmegaDR.bind_assoc
#print axioms OmegaDR.bind_comm
#print axioms OmegaDR.bind_id_left
#print axioms OmegaDR.bind_id_right
#print axioms OmegaDR.unbind_bind
#print axioms OmegaDR.bind_unbind
#print axioms OmegaDR.bind_cancel_left
#print axioms OmegaDR.xor_group_laws
#print axioms OmegaDR.rotateRight_xor_distrib
#print axioms OmegaDR.permBind_distrib
#print axioms OmegaDR.rotateRight_add
#print axioms OmegaDR.rotateRight_full_cycle

-- ISA (T5)
#print axioms OmegaDR.opcode_allowed
#print axioms OmegaDR.instr_allowed
#print axioms OmegaDR.prog_allowed
#print axioms OmegaDR.KernelProgram.allowed_of_mem'
#print axioms OmegaDR.KernelProgram.allowed_of_mem

-- Grid (T7)
#print axioms OmegaDR.joint_period_product
#print axioms OmegaDR.grid_periods_pairwise_coprime
#print axioms OmegaDR.grid_cycle_length
#print axioms OmegaDR.grid_clock_joint_period
#print axioms OmegaDR.grid_clock_full_rotation
#print axioms OmegaDR.grid_clock_rotation_composition

-- Capacity (T2)
#print axioms OmegaDR.choose_zero_right
#print axioms OmegaDR.choose_zero_succ
#print axioms OmegaDR.choose_succ_succ
#print axioms OmegaDR.choose_out_of_range
#print axioms OmegaDR.choose_pos
#print axioms OmegaDR.choose_self
#print axioms OmegaDR.mul_succ_choose_eq
#print axioms OmegaDR.choose_mono
#print axioms OmegaDR.sigma_card_pos
#print axioms OmegaDR.choose_bound_omega
#print axioms OmegaDR.choose_64_8
#print axioms OmegaDR.sigma_card_64_8
#print axioms OmegaDR.omega_capacity

-- Noise (T3)
#print axioms OmegaDR.winSum_split
#print axioms OmegaDR.successMass_antitone_theta
#print axioms OmegaDR.survive_mono_erasure
#print axioms OmegaDR.retrieval_success

end OmegaDR
