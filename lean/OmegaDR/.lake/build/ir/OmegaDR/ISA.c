// Lean compiler output
// Module: OmegaDR.ISA
// Imports: public import Init public meta import Init public import OmegaDR.Basic
#include <lean/lean.h>
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-label"
#elif defined(__GNUC__) && !defined(__CLANG__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-label"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
#ifdef __cplusplus
extern "C" {
#endif
lean_object* lean_nat_land(lean_object*, lean_object*);
lean_object* lean_nat_lor(lean_object*, lean_object*);
lean_object* lean_nat_lxor(lean_object*, lean_object*);
lean_object* l_BitVec_not(lean_object*, lean_object*);
lean_object* l_BitVec_shiftLeft(lean_object*, lean_object*, lean_object*);
lean_object* lean_nat_shiftr(lean_object*, lean_object*);
lean_object* l_BitVec_cpop(lean_object*, lean_object*);
uint8_t lean_nat_dec_lt(lean_object*, lean_object*);
lean_object* l_BitVec_ofNat(lean_object*, lean_object*);
lean_object* l_BitVec_add(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorIdx(uint8_t);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorIdx___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_toCtorIdx(uint8_t);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_toCtorIdx___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorElim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorElim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorElim(lean_object*, lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorElim___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_and_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_and_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_and_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_and_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_or_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_or_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_or_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_or_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_xor_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_xor_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_xor_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_xor_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_not_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_not_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_not_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_not_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shl_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shl_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shl_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shl_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shr_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shr_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shr_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shr_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_popcnt_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_popcnt_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_popcnt_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_popcnt_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_cmp_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_cmp_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_cmp_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_cmp_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_add_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_add_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_add_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_add_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_step(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorIdx___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorIdx___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorIdx(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorIdx___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorElim___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorElim(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorElim___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_nil_elim___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_nil_elim(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_nil_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_cons_elim___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_cons_elim(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_cons_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_instMembershipInstr(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_instMembershipInstr___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorIdx(uint8_t v_x_1_){
_start:
{
switch(v_x_1_)
{
case 0:
{
lean_object* v___x_2_; 
v___x_2_ = lean_unsigned_to_nat(0u);
return v___x_2_;
}
case 1:
{
lean_object* v___x_3_; 
v___x_3_ = lean_unsigned_to_nat(1u);
return v___x_3_;
}
case 2:
{
lean_object* v___x_4_; 
v___x_4_ = lean_unsigned_to_nat(2u);
return v___x_4_;
}
case 3:
{
lean_object* v___x_5_; 
v___x_5_ = lean_unsigned_to_nat(3u);
return v___x_5_;
}
case 4:
{
lean_object* v___x_6_; 
v___x_6_ = lean_unsigned_to_nat(4u);
return v___x_6_;
}
case 5:
{
lean_object* v___x_7_; 
v___x_7_ = lean_unsigned_to_nat(5u);
return v___x_7_;
}
case 6:
{
lean_object* v___x_8_; 
v___x_8_ = lean_unsigned_to_nat(6u);
return v___x_8_;
}
case 7:
{
lean_object* v___x_9_; 
v___x_9_ = lean_unsigned_to_nat(7u);
return v___x_9_;
}
default: 
{
lean_object* v___x_10_; 
v___x_10_ = lean_unsigned_to_nat(8u);
return v___x_10_;
}
}
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorIdx___boxed(lean_object* v_x_11_){
_start:
{
uint8_t v_x_boxed_12_; lean_object* v_res_13_; 
v_x_boxed_12_ = lean_unbox(v_x_11_);
v_res_13_ = lp_OmegaDR_OmegaDR_OpKind_ctorIdx(v_x_boxed_12_);
return v_res_13_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_toCtorIdx(uint8_t v_x_14_){
_start:
{
lean_object* v___x_15_; 
v___x_15_ = lp_OmegaDR_OmegaDR_OpKind_ctorIdx(v_x_14_);
return v___x_15_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_toCtorIdx___boxed(lean_object* v_x_16_){
_start:
{
uint8_t v_x_4__boxed_17_; lean_object* v_res_18_; 
v_x_4__boxed_17_ = lean_unbox(v_x_16_);
v_res_18_ = lp_OmegaDR_OmegaDR_OpKind_toCtorIdx(v_x_4__boxed_17_);
return v_res_18_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorElim___redArg(lean_object* v_k_19_){
_start:
{
lean_inc(v_k_19_);
return v_k_19_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorElim___redArg___boxed(lean_object* v_k_20_){
_start:
{
lean_object* v_res_21_; 
v_res_21_ = lp_OmegaDR_OmegaDR_OpKind_ctorElim___redArg(v_k_20_);
lean_dec(v_k_20_);
return v_res_21_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorElim(lean_object* v_motive_22_, lean_object* v_ctorIdx_23_, uint8_t v_t_24_, lean_object* v_h_25_, lean_object* v_k_26_){
_start:
{
lean_inc(v_k_26_);
return v_k_26_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_ctorElim___boxed(lean_object* v_motive_27_, lean_object* v_ctorIdx_28_, lean_object* v_t_29_, lean_object* v_h_30_, lean_object* v_k_31_){
_start:
{
uint8_t v_t_boxed_32_; lean_object* v_res_33_; 
v_t_boxed_32_ = lean_unbox(v_t_29_);
v_res_33_ = lp_OmegaDR_OmegaDR_OpKind_ctorElim(v_motive_27_, v_ctorIdx_28_, v_t_boxed_32_, v_h_30_, v_k_31_);
lean_dec(v_k_31_);
lean_dec(v_ctorIdx_28_);
return v_res_33_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_and_elim___redArg(lean_object* v_and_34_){
_start:
{
lean_inc(v_and_34_);
return v_and_34_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_and_elim___redArg___boxed(lean_object* v_and_35_){
_start:
{
lean_object* v_res_36_; 
v_res_36_ = lp_OmegaDR_OmegaDR_OpKind_and_elim___redArg(v_and_35_);
lean_dec(v_and_35_);
return v_res_36_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_and_elim(lean_object* v_motive_37_, uint8_t v_t_38_, lean_object* v_h_39_, lean_object* v_and_40_){
_start:
{
lean_inc(v_and_40_);
return v_and_40_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_and_elim___boxed(lean_object* v_motive_41_, lean_object* v_t_42_, lean_object* v_h_43_, lean_object* v_and_44_){
_start:
{
uint8_t v_t_boxed_45_; lean_object* v_res_46_; 
v_t_boxed_45_ = lean_unbox(v_t_42_);
v_res_46_ = lp_OmegaDR_OmegaDR_OpKind_and_elim(v_motive_41_, v_t_boxed_45_, v_h_43_, v_and_44_);
lean_dec(v_and_44_);
return v_res_46_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_or_elim___redArg(lean_object* v_or_47_){
_start:
{
lean_inc(v_or_47_);
return v_or_47_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_or_elim___redArg___boxed(lean_object* v_or_48_){
_start:
{
lean_object* v_res_49_; 
v_res_49_ = lp_OmegaDR_OmegaDR_OpKind_or_elim___redArg(v_or_48_);
lean_dec(v_or_48_);
return v_res_49_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_or_elim(lean_object* v_motive_50_, uint8_t v_t_51_, lean_object* v_h_52_, lean_object* v_or_53_){
_start:
{
lean_inc(v_or_53_);
return v_or_53_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_or_elim___boxed(lean_object* v_motive_54_, lean_object* v_t_55_, lean_object* v_h_56_, lean_object* v_or_57_){
_start:
{
uint8_t v_t_boxed_58_; lean_object* v_res_59_; 
v_t_boxed_58_ = lean_unbox(v_t_55_);
v_res_59_ = lp_OmegaDR_OmegaDR_OpKind_or_elim(v_motive_54_, v_t_boxed_58_, v_h_56_, v_or_57_);
lean_dec(v_or_57_);
return v_res_59_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_xor_elim___redArg(lean_object* v_xor_60_){
_start:
{
lean_inc(v_xor_60_);
return v_xor_60_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_xor_elim___redArg___boxed(lean_object* v_xor_61_){
_start:
{
lean_object* v_res_62_; 
v_res_62_ = lp_OmegaDR_OmegaDR_OpKind_xor_elim___redArg(v_xor_61_);
lean_dec(v_xor_61_);
return v_res_62_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_xor_elim(lean_object* v_motive_63_, uint8_t v_t_64_, lean_object* v_h_65_, lean_object* v_xor_66_){
_start:
{
lean_inc(v_xor_66_);
return v_xor_66_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_xor_elim___boxed(lean_object* v_motive_67_, lean_object* v_t_68_, lean_object* v_h_69_, lean_object* v_xor_70_){
_start:
{
uint8_t v_t_boxed_71_; lean_object* v_res_72_; 
v_t_boxed_71_ = lean_unbox(v_t_68_);
v_res_72_ = lp_OmegaDR_OmegaDR_OpKind_xor_elim(v_motive_67_, v_t_boxed_71_, v_h_69_, v_xor_70_);
lean_dec(v_xor_70_);
return v_res_72_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_not_elim___redArg(lean_object* v_not_73_){
_start:
{
lean_inc(v_not_73_);
return v_not_73_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_not_elim___redArg___boxed(lean_object* v_not_74_){
_start:
{
lean_object* v_res_75_; 
v_res_75_ = lp_OmegaDR_OmegaDR_OpKind_not_elim___redArg(v_not_74_);
lean_dec(v_not_74_);
return v_res_75_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_not_elim(lean_object* v_motive_76_, uint8_t v_t_77_, lean_object* v_h_78_, lean_object* v_not_79_){
_start:
{
lean_inc(v_not_79_);
return v_not_79_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_not_elim___boxed(lean_object* v_motive_80_, lean_object* v_t_81_, lean_object* v_h_82_, lean_object* v_not_83_){
_start:
{
uint8_t v_t_boxed_84_; lean_object* v_res_85_; 
v_t_boxed_84_ = lean_unbox(v_t_81_);
v_res_85_ = lp_OmegaDR_OmegaDR_OpKind_not_elim(v_motive_80_, v_t_boxed_84_, v_h_82_, v_not_83_);
lean_dec(v_not_83_);
return v_res_85_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shl_elim___redArg(lean_object* v_shl_86_){
_start:
{
lean_inc(v_shl_86_);
return v_shl_86_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shl_elim___redArg___boxed(lean_object* v_shl_87_){
_start:
{
lean_object* v_res_88_; 
v_res_88_ = lp_OmegaDR_OmegaDR_OpKind_shl_elim___redArg(v_shl_87_);
lean_dec(v_shl_87_);
return v_res_88_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shl_elim(lean_object* v_motive_89_, uint8_t v_t_90_, lean_object* v_h_91_, lean_object* v_shl_92_){
_start:
{
lean_inc(v_shl_92_);
return v_shl_92_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shl_elim___boxed(lean_object* v_motive_93_, lean_object* v_t_94_, lean_object* v_h_95_, lean_object* v_shl_96_){
_start:
{
uint8_t v_t_boxed_97_; lean_object* v_res_98_; 
v_t_boxed_97_ = lean_unbox(v_t_94_);
v_res_98_ = lp_OmegaDR_OmegaDR_OpKind_shl_elim(v_motive_93_, v_t_boxed_97_, v_h_95_, v_shl_96_);
lean_dec(v_shl_96_);
return v_res_98_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shr_elim___redArg(lean_object* v_shr_99_){
_start:
{
lean_inc(v_shr_99_);
return v_shr_99_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shr_elim___redArg___boxed(lean_object* v_shr_100_){
_start:
{
lean_object* v_res_101_; 
v_res_101_ = lp_OmegaDR_OmegaDR_OpKind_shr_elim___redArg(v_shr_100_);
lean_dec(v_shr_100_);
return v_res_101_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shr_elim(lean_object* v_motive_102_, uint8_t v_t_103_, lean_object* v_h_104_, lean_object* v_shr_105_){
_start:
{
lean_inc(v_shr_105_);
return v_shr_105_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_shr_elim___boxed(lean_object* v_motive_106_, lean_object* v_t_107_, lean_object* v_h_108_, lean_object* v_shr_109_){
_start:
{
uint8_t v_t_boxed_110_; lean_object* v_res_111_; 
v_t_boxed_110_ = lean_unbox(v_t_107_);
v_res_111_ = lp_OmegaDR_OmegaDR_OpKind_shr_elim(v_motive_106_, v_t_boxed_110_, v_h_108_, v_shr_109_);
lean_dec(v_shr_109_);
return v_res_111_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_popcnt_elim___redArg(lean_object* v_popcnt_112_){
_start:
{
lean_inc(v_popcnt_112_);
return v_popcnt_112_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_popcnt_elim___redArg___boxed(lean_object* v_popcnt_113_){
_start:
{
lean_object* v_res_114_; 
v_res_114_ = lp_OmegaDR_OmegaDR_OpKind_popcnt_elim___redArg(v_popcnt_113_);
lean_dec(v_popcnt_113_);
return v_res_114_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_popcnt_elim(lean_object* v_motive_115_, uint8_t v_t_116_, lean_object* v_h_117_, lean_object* v_popcnt_118_){
_start:
{
lean_inc(v_popcnt_118_);
return v_popcnt_118_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_popcnt_elim___boxed(lean_object* v_motive_119_, lean_object* v_t_120_, lean_object* v_h_121_, lean_object* v_popcnt_122_){
_start:
{
uint8_t v_t_boxed_123_; lean_object* v_res_124_; 
v_t_boxed_123_ = lean_unbox(v_t_120_);
v_res_124_ = lp_OmegaDR_OmegaDR_OpKind_popcnt_elim(v_motive_119_, v_t_boxed_123_, v_h_121_, v_popcnt_122_);
lean_dec(v_popcnt_122_);
return v_res_124_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_cmp_elim___redArg(lean_object* v_cmp_125_){
_start:
{
lean_inc(v_cmp_125_);
return v_cmp_125_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_cmp_elim___redArg___boxed(lean_object* v_cmp_126_){
_start:
{
lean_object* v_res_127_; 
v_res_127_ = lp_OmegaDR_OmegaDR_OpKind_cmp_elim___redArg(v_cmp_126_);
lean_dec(v_cmp_126_);
return v_res_127_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_cmp_elim(lean_object* v_motive_128_, uint8_t v_t_129_, lean_object* v_h_130_, lean_object* v_cmp_131_){
_start:
{
lean_inc(v_cmp_131_);
return v_cmp_131_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_cmp_elim___boxed(lean_object* v_motive_132_, lean_object* v_t_133_, lean_object* v_h_134_, lean_object* v_cmp_135_){
_start:
{
uint8_t v_t_boxed_136_; lean_object* v_res_137_; 
v_t_boxed_136_ = lean_unbox(v_t_133_);
v_res_137_ = lp_OmegaDR_OmegaDR_OpKind_cmp_elim(v_motive_132_, v_t_boxed_136_, v_h_134_, v_cmp_135_);
lean_dec(v_cmp_135_);
return v_res_137_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_add_elim___redArg(lean_object* v_add_138_){
_start:
{
lean_inc(v_add_138_);
return v_add_138_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_add_elim___redArg___boxed(lean_object* v_add_139_){
_start:
{
lean_object* v_res_140_; 
v_res_140_ = lp_OmegaDR_OmegaDR_OpKind_add_elim___redArg(v_add_139_);
lean_dec(v_add_139_);
return v_res_140_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_add_elim(lean_object* v_motive_141_, uint8_t v_t_142_, lean_object* v_h_143_, lean_object* v_add_144_){
_start:
{
lean_inc(v_add_144_);
return v_add_144_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_OpKind_add_elim___boxed(lean_object* v_motive_145_, lean_object* v_t_146_, lean_object* v_h_147_, lean_object* v_add_148_){
_start:
{
uint8_t v_t_boxed_149_; lean_object* v_res_150_; 
v_t_boxed_149_ = lean_unbox(v_t_146_);
v_res_150_ = lp_OmegaDR_OmegaDR_OpKind_add_elim(v_motive_145_, v_t_boxed_149_, v_h_147_, v_add_148_);
lean_dec(v_add_148_);
return v_res_150_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_step(lean_object* v_w_151_, lean_object* v_00_u03c1_152_, lean_object* v_i_153_){
_start:
{
uint8_t v_kind_154_; 
v_kind_154_ = lean_ctor_get_uint8(v_i_153_, sizeof(void*)*4);
switch(v_kind_154_)
{
case 0:
{
lean_object* v_rs1_155_; lean_object* v_rs2_156_; lean_object* v___x_157_; lean_object* v___x_158_; lean_object* v___x_159_; 
lean_dec(v_w_151_);
v_rs1_155_ = lean_ctor_get(v_i_153_, 1);
lean_inc(v_rs1_155_);
v_rs2_156_ = lean_ctor_get(v_i_153_, 2);
lean_inc(v_rs2_156_);
lean_dec_ref(v_i_153_);
lean_inc_ref(v_00_u03c1_152_);
v___x_157_ = lean_apply_1(v_00_u03c1_152_, v_rs1_155_);
v___x_158_ = lean_apply_1(v_00_u03c1_152_, v_rs2_156_);
v___x_159_ = lean_nat_land(v___x_157_, v___x_158_);
lean_dec(v___x_158_);
lean_dec(v___x_157_);
return v___x_159_;
}
case 1:
{
lean_object* v_rs1_160_; lean_object* v_rs2_161_; lean_object* v___x_162_; lean_object* v___x_163_; lean_object* v___x_164_; 
lean_dec(v_w_151_);
v_rs1_160_ = lean_ctor_get(v_i_153_, 1);
lean_inc(v_rs1_160_);
v_rs2_161_ = lean_ctor_get(v_i_153_, 2);
lean_inc(v_rs2_161_);
lean_dec_ref(v_i_153_);
lean_inc_ref(v_00_u03c1_152_);
v___x_162_ = lean_apply_1(v_00_u03c1_152_, v_rs1_160_);
v___x_163_ = lean_apply_1(v_00_u03c1_152_, v_rs2_161_);
v___x_164_ = lean_nat_lor(v___x_162_, v___x_163_);
lean_dec(v___x_163_);
lean_dec(v___x_162_);
return v___x_164_;
}
case 2:
{
lean_object* v_rs1_165_; lean_object* v_rs2_166_; lean_object* v___x_167_; lean_object* v___x_168_; lean_object* v___x_169_; 
lean_dec(v_w_151_);
v_rs1_165_ = lean_ctor_get(v_i_153_, 1);
lean_inc(v_rs1_165_);
v_rs2_166_ = lean_ctor_get(v_i_153_, 2);
lean_inc(v_rs2_166_);
lean_dec_ref(v_i_153_);
lean_inc_ref(v_00_u03c1_152_);
v___x_167_ = lean_apply_1(v_00_u03c1_152_, v_rs1_165_);
v___x_168_ = lean_apply_1(v_00_u03c1_152_, v_rs2_166_);
v___x_169_ = lean_nat_lxor(v___x_167_, v___x_168_);
lean_dec(v___x_168_);
lean_dec(v___x_167_);
return v___x_169_;
}
case 3:
{
lean_object* v_rs1_170_; lean_object* v___x_171_; lean_object* v___x_172_; 
v_rs1_170_ = lean_ctor_get(v_i_153_, 1);
lean_inc(v_rs1_170_);
lean_dec_ref(v_i_153_);
v___x_171_ = lean_apply_1(v_00_u03c1_152_, v_rs1_170_);
v___x_172_ = l_BitVec_not(v_w_151_, v___x_171_);
lean_dec(v___x_171_);
lean_dec(v_w_151_);
return v___x_172_;
}
case 4:
{
lean_object* v_rs1_173_; lean_object* v_imm_174_; lean_object* v___x_175_; lean_object* v___x_176_; 
v_rs1_173_ = lean_ctor_get(v_i_153_, 1);
lean_inc(v_rs1_173_);
v_imm_174_ = lean_ctor_get(v_i_153_, 3);
lean_inc(v_imm_174_);
lean_dec_ref(v_i_153_);
v___x_175_ = lean_apply_1(v_00_u03c1_152_, v_rs1_173_);
v___x_176_ = l_BitVec_shiftLeft(v_w_151_, v___x_175_, v_imm_174_);
lean_dec(v_imm_174_);
lean_dec(v___x_175_);
lean_dec(v_w_151_);
return v___x_176_;
}
case 5:
{
lean_object* v_rs1_177_; lean_object* v_imm_178_; lean_object* v___x_179_; lean_object* v___x_180_; 
lean_dec(v_w_151_);
v_rs1_177_ = lean_ctor_get(v_i_153_, 1);
lean_inc(v_rs1_177_);
v_imm_178_ = lean_ctor_get(v_i_153_, 3);
lean_inc(v_imm_178_);
lean_dec_ref(v_i_153_);
v___x_179_ = lean_apply_1(v_00_u03c1_152_, v_rs1_177_);
v___x_180_ = lean_nat_shiftr(v___x_179_, v_imm_178_);
lean_dec(v_imm_178_);
lean_dec(v___x_179_);
return v___x_180_;
}
case 6:
{
lean_object* v_rs1_181_; lean_object* v___x_182_; lean_object* v___x_183_; 
v_rs1_181_ = lean_ctor_get(v_i_153_, 1);
lean_inc(v_rs1_181_);
lean_dec_ref(v_i_153_);
v___x_182_ = lean_apply_1(v_00_u03c1_152_, v_rs1_181_);
v___x_183_ = l_BitVec_cpop(v_w_151_, v___x_182_);
lean_dec(v___x_182_);
return v___x_183_;
}
case 7:
{
lean_object* v_rs1_184_; lean_object* v_rs2_185_; lean_object* v___x_186_; lean_object* v___x_187_; uint8_t v___x_188_; 
v_rs1_184_ = lean_ctor_get(v_i_153_, 1);
lean_inc(v_rs1_184_);
v_rs2_185_ = lean_ctor_get(v_i_153_, 2);
lean_inc(v_rs2_185_);
lean_dec_ref(v_i_153_);
lean_inc_ref(v_00_u03c1_152_);
v___x_186_ = lean_apply_1(v_00_u03c1_152_, v_rs1_184_);
v___x_187_ = lean_apply_1(v_00_u03c1_152_, v_rs2_185_);
v___x_188_ = lean_nat_dec_lt(v___x_186_, v___x_187_);
lean_dec(v___x_187_);
lean_dec(v___x_186_);
if (v___x_188_ == 0)
{
lean_object* v___x_189_; lean_object* v___x_190_; 
v___x_189_ = lean_unsigned_to_nat(0u);
v___x_190_ = l_BitVec_ofNat(v_w_151_, v___x_189_);
lean_dec(v_w_151_);
return v___x_190_;
}
else
{
lean_object* v___x_191_; lean_object* v___x_192_; 
v___x_191_ = lean_unsigned_to_nat(1u);
v___x_192_ = l_BitVec_ofNat(v_w_151_, v___x_191_);
lean_dec(v_w_151_);
return v___x_192_;
}
}
default: 
{
lean_object* v_rs1_193_; lean_object* v_rs2_194_; lean_object* v___x_195_; lean_object* v___x_196_; lean_object* v___x_197_; 
v_rs1_193_ = lean_ctor_get(v_i_153_, 1);
lean_inc(v_rs1_193_);
v_rs2_194_ = lean_ctor_get(v_i_153_, 2);
lean_inc(v_rs2_194_);
lean_dec_ref(v_i_153_);
lean_inc_ref(v_00_u03c1_152_);
v___x_195_ = lean_apply_1(v_00_u03c1_152_, v_rs1_193_);
v___x_196_ = lean_apply_1(v_00_u03c1_152_, v_rs2_194_);
v___x_197_ = l_BitVec_add(v_w_151_, v___x_195_, v___x_196_);
lean_dec(v___x_196_);
lean_dec(v___x_195_);
lean_dec(v_w_151_);
return v___x_197_;
}
}
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorIdx___redArg(lean_object* v_x_198_){
_start:
{
if (lean_obj_tag(v_x_198_) == 0)
{
lean_object* v___x_199_; 
v___x_199_ = lean_unsigned_to_nat(0u);
return v___x_199_;
}
else
{
lean_object* v___x_200_; 
v___x_200_ = lean_unsigned_to_nat(1u);
return v___x_200_;
}
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorIdx___redArg___boxed(lean_object* v_x_201_){
_start:
{
lean_object* v_res_202_; 
v_res_202_ = lp_OmegaDR_OmegaDR_KernelProgram_ctorIdx___redArg(v_x_201_);
lean_dec(v_x_201_);
return v_res_202_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorIdx(lean_object* v_w_203_, lean_object* v_x_204_){
_start:
{
lean_object* v___x_205_; 
v___x_205_ = lp_OmegaDR_OmegaDR_KernelProgram_ctorIdx___redArg(v_x_204_);
return v___x_205_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorIdx___boxed(lean_object* v_w_206_, lean_object* v_x_207_){
_start:
{
lean_object* v_res_208_; 
v_res_208_ = lp_OmegaDR_OmegaDR_KernelProgram_ctorIdx(v_w_206_, v_x_207_);
lean_dec(v_x_207_);
lean_dec(v_w_206_);
return v_res_208_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorElim___redArg(lean_object* v_t_209_, lean_object* v_k_210_){
_start:
{
if (lean_obj_tag(v_t_209_) == 0)
{
return v_k_210_;
}
else
{
lean_object* v_i_211_; lean_object* v_a_212_; lean_object* v___x_213_; 
v_i_211_ = lean_ctor_get(v_t_209_, 0);
lean_inc_ref(v_i_211_);
v_a_212_ = lean_ctor_get(v_t_209_, 1);
lean_inc(v_a_212_);
lean_dec_ref_known(v_t_209_, 2);
v___x_213_ = lean_apply_3(v_k_210_, v_i_211_, lean_box(0), v_a_212_);
return v___x_213_;
}
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorElim(lean_object* v_w_214_, lean_object* v_motive_215_, lean_object* v_ctorIdx_216_, lean_object* v_t_217_, lean_object* v_h_218_, lean_object* v_k_219_){
_start:
{
lean_object* v___x_220_; 
v___x_220_ = lp_OmegaDR_OmegaDR_KernelProgram_ctorElim___redArg(v_t_217_, v_k_219_);
return v___x_220_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_ctorElim___boxed(lean_object* v_w_221_, lean_object* v_motive_222_, lean_object* v_ctorIdx_223_, lean_object* v_t_224_, lean_object* v_h_225_, lean_object* v_k_226_){
_start:
{
lean_object* v_res_227_; 
v_res_227_ = lp_OmegaDR_OmegaDR_KernelProgram_ctorElim(v_w_221_, v_motive_222_, v_ctorIdx_223_, v_t_224_, v_h_225_, v_k_226_);
lean_dec(v_ctorIdx_223_);
lean_dec(v_w_221_);
return v_res_227_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_nil_elim___redArg(lean_object* v_t_228_, lean_object* v_nil_229_){
_start:
{
lean_object* v___x_230_; 
v___x_230_ = lp_OmegaDR_OmegaDR_KernelProgram_ctorElim___redArg(v_t_228_, v_nil_229_);
return v___x_230_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_nil_elim(lean_object* v_w_231_, lean_object* v_motive_232_, lean_object* v_t_233_, lean_object* v_h_234_, lean_object* v_nil_235_){
_start:
{
lean_object* v___x_236_; 
v___x_236_ = lp_OmegaDR_OmegaDR_KernelProgram_ctorElim___redArg(v_t_233_, v_nil_235_);
return v___x_236_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_nil_elim___boxed(lean_object* v_w_237_, lean_object* v_motive_238_, lean_object* v_t_239_, lean_object* v_h_240_, lean_object* v_nil_241_){
_start:
{
lean_object* v_res_242_; 
v_res_242_ = lp_OmegaDR_OmegaDR_KernelProgram_nil_elim(v_w_237_, v_motive_238_, v_t_239_, v_h_240_, v_nil_241_);
lean_dec(v_w_237_);
return v_res_242_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_cons_elim___redArg(lean_object* v_t_243_, lean_object* v_cons_244_){
_start:
{
lean_object* v___x_245_; 
v___x_245_ = lp_OmegaDR_OmegaDR_KernelProgram_ctorElim___redArg(v_t_243_, v_cons_244_);
return v___x_245_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_cons_elim(lean_object* v_w_246_, lean_object* v_motive_247_, lean_object* v_t_248_, lean_object* v_h_249_, lean_object* v_cons_250_){
_start:
{
lean_object* v___x_251_; 
v___x_251_ = lp_OmegaDR_OmegaDR_KernelProgram_ctorElim___redArg(v_t_248_, v_cons_250_);
return v___x_251_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_cons_elim___boxed(lean_object* v_w_252_, lean_object* v_motive_253_, lean_object* v_t_254_, lean_object* v_h_255_, lean_object* v_cons_256_){
_start:
{
lean_object* v_res_257_; 
v_res_257_ = lp_OmegaDR_OmegaDR_KernelProgram_cons_elim(v_w_252_, v_motive_253_, v_t_254_, v_h_255_, v_cons_256_);
lean_dec(v_w_252_);
return v_res_257_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_instMembershipInstr(lean_object* v_w_258_){
_start:
{
lean_object* v___x_259_; 
v___x_259_ = lean_box(0);
return v___x_259_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_KernelProgram_instMembershipInstr___boxed(lean_object* v_w_260_){
_start:
{
lean_object* v_res_261_; 
v_res_261_ = lp_OmegaDR_OmegaDR_KernelProgram_instMembershipInstr(v_w_260_);
lean_dec(v_w_260_);
return v_res_261_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_OmegaDR_OmegaDR_Basic(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_OmegaDR_OmegaDR_ISA(uint8_t builtin) {
lean_object * res;
if (_G_initialized) return lean_io_result_mk_ok(lean_box(0));
_G_initialized = true;
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_OmegaDR_OmegaDR_Basic(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
