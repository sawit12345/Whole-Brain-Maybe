// Lean compiler output
// Module: OmegaDR.Capacity
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
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
lean_object* lean_nat_sub(lean_object*, lean_object*);
lean_object* lean_nat_add(lean_object*, lean_object*);
lean_object* lean_nat_mul(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_choose(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_choose___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_SigmaCard(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_SigmaCard___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Capacity_0__OmegaDR_choose_match__1_splitter___redArg(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Capacity_0__OmegaDR_choose_match__1_splitter___redArg___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Capacity_0__OmegaDR_choose_match__1_splitter(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Capacity_0__OmegaDR_choose_match__1_splitter___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_choose(lean_object* v_x_1_, lean_object* v_x_2_){
_start:
{
lean_object* v_zero_3_; uint8_t v_isZero_4_; 
v_zero_3_ = lean_unsigned_to_nat(0u);
v_isZero_4_ = lean_nat_dec_eq(v_x_2_, v_zero_3_);
if (v_isZero_4_ == 1)
{
lean_object* v___x_5_; 
v___x_5_ = lean_unsigned_to_nat(1u);
return v___x_5_;
}
else
{
uint8_t v_isZero_6_; 
v_isZero_6_ = lean_nat_dec_eq(v_x_1_, v_zero_3_);
if (v_isZero_6_ == 1)
{
return v_zero_3_;
}
else
{
lean_object* v_one_7_; lean_object* v_n_8_; lean_object* v_n_9_; lean_object* v___x_10_; lean_object* v___x_11_; lean_object* v___x_12_; lean_object* v___x_13_; 
v_one_7_ = lean_unsigned_to_nat(1u);
v_n_8_ = lean_nat_sub(v_x_2_, v_one_7_);
v_n_9_ = lean_nat_sub(v_x_1_, v_one_7_);
v___x_10_ = lp_OmegaDR_OmegaDR_choose(v_n_9_, v_n_8_);
v___x_11_ = lean_nat_add(v_n_8_, v_one_7_);
lean_dec(v_n_8_);
v___x_12_ = lp_OmegaDR_OmegaDR_choose(v_n_9_, v___x_11_);
lean_dec(v___x_11_);
lean_dec(v_n_9_);
v___x_13_ = lean_nat_add(v___x_10_, v___x_12_);
lean_dec(v___x_12_);
lean_dec(v___x_10_);
return v___x_13_;
}
}
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_choose___boxed(lean_object* v_x_14_, lean_object* v_x_15_){
_start:
{
lean_object* v_res_16_; 
v_res_16_ = lp_OmegaDR_OmegaDR_choose(v_x_14_, v_x_15_);
lean_dec(v_x_15_);
lean_dec(v_x_14_);
return v_res_16_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_SigmaCard(lean_object* v_n_17_, lean_object* v_k_18_){
_start:
{
lean_object* v___x_19_; lean_object* v___x_20_; lean_object* v___x_21_; lean_object* v___x_22_; 
v___x_19_ = lp_OmegaDR_OmegaDR_choose(v_n_17_, v_k_18_);
v___x_20_ = lean_nat_sub(v_n_17_, v_k_18_);
v___x_21_ = lp_OmegaDR_OmegaDR_choose(v___x_20_, v_k_18_);
lean_dec(v___x_20_);
v___x_22_ = lean_nat_mul(v___x_19_, v___x_21_);
lean_dec(v___x_21_);
lean_dec(v___x_19_);
return v___x_22_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_SigmaCard___boxed(lean_object* v_n_23_, lean_object* v_k_24_){
_start:
{
lean_object* v_res_25_; 
v_res_25_ = lp_OmegaDR_OmegaDR_SigmaCard(v_n_23_, v_k_24_);
lean_dec(v_k_24_);
lean_dec(v_n_23_);
return v_res_25_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Capacity_0__OmegaDR_choose_match__1_splitter___redArg(lean_object* v_x_26_, lean_object* v_x_27_, lean_object* v_h__1_28_, lean_object* v_h__2_29_, lean_object* v_h__3_30_){
_start:
{
lean_object* v_zero_31_; uint8_t v_isZero_32_; 
v_zero_31_ = lean_unsigned_to_nat(0u);
v_isZero_32_ = lean_nat_dec_eq(v_x_27_, v_zero_31_);
if (v_isZero_32_ == 1)
{
lean_object* v___x_33_; 
lean_dec(v_h__3_30_);
lean_dec(v_h__2_29_);
v___x_33_ = lean_apply_1(v_h__1_28_, v_x_26_);
return v___x_33_;
}
else
{
lean_object* v_one_34_; lean_object* v_n_35_; uint8_t v_isZero_36_; 
lean_dec(v_h__1_28_);
v_one_34_ = lean_unsigned_to_nat(1u);
v_n_35_ = lean_nat_sub(v_x_27_, v_one_34_);
v_isZero_36_ = lean_nat_dec_eq(v_x_26_, v_zero_31_);
if (v_isZero_36_ == 1)
{
lean_object* v___x_37_; 
lean_dec(v_h__3_30_);
lean_dec(v_x_26_);
v___x_37_ = lean_apply_1(v_h__2_29_, v_n_35_);
return v___x_37_;
}
else
{
lean_object* v_n_38_; lean_object* v___x_39_; 
lean_dec(v_h__2_29_);
v_n_38_ = lean_nat_sub(v_x_26_, v_one_34_);
lean_dec(v_x_26_);
v___x_39_ = lean_apply_2(v_h__3_30_, v_n_38_, v_n_35_);
return v___x_39_;
}
}
}
}
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Capacity_0__OmegaDR_choose_match__1_splitter___redArg___boxed(lean_object* v_x_40_, lean_object* v_x_41_, lean_object* v_h__1_42_, lean_object* v_h__2_43_, lean_object* v_h__3_44_){
_start:
{
lean_object* v_res_45_; 
v_res_45_ = lp_OmegaDR___private_OmegaDR_Capacity_0__OmegaDR_choose_match__1_splitter___redArg(v_x_40_, v_x_41_, v_h__1_42_, v_h__2_43_, v_h__3_44_);
lean_dec(v_x_41_);
return v_res_45_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Capacity_0__OmegaDR_choose_match__1_splitter(lean_object* v_motive_46_, lean_object* v_x_47_, lean_object* v_x_48_, lean_object* v_h__1_49_, lean_object* v_h__2_50_, lean_object* v_h__3_51_){
_start:
{
lean_object* v_zero_52_; uint8_t v_isZero_53_; 
v_zero_52_ = lean_unsigned_to_nat(0u);
v_isZero_53_ = lean_nat_dec_eq(v_x_48_, v_zero_52_);
if (v_isZero_53_ == 1)
{
lean_object* v___x_54_; 
lean_dec(v_h__3_51_);
lean_dec(v_h__2_50_);
v___x_54_ = lean_apply_1(v_h__1_49_, v_x_47_);
return v___x_54_;
}
else
{
lean_object* v_one_55_; lean_object* v_n_56_; uint8_t v_isZero_57_; 
lean_dec(v_h__1_49_);
v_one_55_ = lean_unsigned_to_nat(1u);
v_n_56_ = lean_nat_sub(v_x_48_, v_one_55_);
v_isZero_57_ = lean_nat_dec_eq(v_x_47_, v_zero_52_);
if (v_isZero_57_ == 1)
{
lean_object* v___x_58_; 
lean_dec(v_h__3_51_);
lean_dec(v_x_47_);
v___x_58_ = lean_apply_1(v_h__2_50_, v_n_56_);
return v___x_58_;
}
else
{
lean_object* v_n_59_; lean_object* v___x_60_; 
lean_dec(v_h__2_50_);
v_n_59_ = lean_nat_sub(v_x_47_, v_one_55_);
lean_dec(v_x_47_);
v___x_60_ = lean_apply_2(v_h__3_51_, v_n_59_, v_n_56_);
return v___x_60_;
}
}
}
}
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Capacity_0__OmegaDR_choose_match__1_splitter___boxed(lean_object* v_motive_61_, lean_object* v_x_62_, lean_object* v_x_63_, lean_object* v_h__1_64_, lean_object* v_h__2_65_, lean_object* v_h__3_66_){
_start:
{
lean_object* v_res_67_; 
v_res_67_ = lp_OmegaDR___private_OmegaDR_Capacity_0__OmegaDR_choose_match__1_splitter(v_motive_61_, v_x_62_, v_x_63_, v_h__1_64_, v_h__2_65_, v_h__3_66_);
lean_dec(v_x_63_);
return v_res_67_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_OmegaDR_OmegaDR_Basic(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_OmegaDR_OmegaDR_Capacity(uint8_t builtin) {
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
