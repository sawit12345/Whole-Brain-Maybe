// Lean compiler output
// Module: OmegaDR.Noise
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
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_winSum(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_winSum___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Noise_0__OmegaDR_winSum_match__1_splitter___redArg(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Noise_0__OmegaDR_winSum_match__1_splitter___redArg___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Noise_0__OmegaDR_winSum_match__1_splitter(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Noise_0__OmegaDR_winSum_match__1_splitter___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_successMass(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_successMass___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_winSum(lean_object* v_g_1_, lean_object* v_x_2_, lean_object* v_x_3_){
_start:
{
lean_object* v_zero_4_; uint8_t v_isZero_5_; 
v_zero_4_ = lean_unsigned_to_nat(0u);
v_isZero_5_ = lean_nat_dec_eq(v_x_3_, v_zero_4_);
if (v_isZero_5_ == 1)
{
lean_dec(v_x_2_);
lean_dec_ref(v_g_1_);
return v_zero_4_;
}
else
{
lean_object* v_one_6_; lean_object* v_n_7_; lean_object* v___x_8_; lean_object* v___x_9_; lean_object* v___x_10_; lean_object* v___x_11_; 
v_one_6_ = lean_unsigned_to_nat(1u);
v_n_7_ = lean_nat_sub(v_x_3_, v_one_6_);
lean_inc_ref(v_g_1_);
lean_inc(v_x_2_);
v___x_8_ = lean_apply_1(v_g_1_, v_x_2_);
v___x_9_ = lean_nat_add(v_x_2_, v_one_6_);
lean_dec(v_x_2_);
v___x_10_ = lp_OmegaDR_OmegaDR_winSum(v_g_1_, v___x_9_, v_n_7_);
lean_dec(v_n_7_);
v___x_11_ = lean_nat_add(v___x_8_, v___x_10_);
lean_dec(v___x_10_);
lean_dec(v___x_8_);
return v___x_11_;
}
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_winSum___boxed(lean_object* v_g_12_, lean_object* v_x_13_, lean_object* v_x_14_){
_start:
{
lean_object* v_res_15_; 
v_res_15_ = lp_OmegaDR_OmegaDR_winSum(v_g_12_, v_x_13_, v_x_14_);
lean_dec(v_x_14_);
return v_res_15_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Noise_0__OmegaDR_winSum_match__1_splitter___redArg(lean_object* v_x_16_, lean_object* v_x_17_, lean_object* v_h__1_18_, lean_object* v_h__2_19_){
_start:
{
lean_object* v_zero_20_; uint8_t v_isZero_21_; 
v_zero_20_ = lean_unsigned_to_nat(0u);
v_isZero_21_ = lean_nat_dec_eq(v_x_17_, v_zero_20_);
if (v_isZero_21_ == 1)
{
lean_object* v___x_22_; 
lean_dec(v_h__2_19_);
v___x_22_ = lean_apply_1(v_h__1_18_, v_x_16_);
return v___x_22_;
}
else
{
lean_object* v_one_23_; lean_object* v_n_24_; lean_object* v___x_25_; 
lean_dec(v_h__1_18_);
v_one_23_ = lean_unsigned_to_nat(1u);
v_n_24_ = lean_nat_sub(v_x_17_, v_one_23_);
v___x_25_ = lean_apply_2(v_h__2_19_, v_x_16_, v_n_24_);
return v___x_25_;
}
}
}
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Noise_0__OmegaDR_winSum_match__1_splitter___redArg___boxed(lean_object* v_x_26_, lean_object* v_x_27_, lean_object* v_h__1_28_, lean_object* v_h__2_29_){
_start:
{
lean_object* v_res_30_; 
v_res_30_ = lp_OmegaDR___private_OmegaDR_Noise_0__OmegaDR_winSum_match__1_splitter___redArg(v_x_26_, v_x_27_, v_h__1_28_, v_h__2_29_);
lean_dec(v_x_27_);
return v_res_30_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Noise_0__OmegaDR_winSum_match__1_splitter(lean_object* v_motive_31_, lean_object* v_x_32_, lean_object* v_x_33_, lean_object* v_h__1_34_, lean_object* v_h__2_35_){
_start:
{
lean_object* v_zero_36_; uint8_t v_isZero_37_; 
v_zero_36_ = lean_unsigned_to_nat(0u);
v_isZero_37_ = lean_nat_dec_eq(v_x_33_, v_zero_36_);
if (v_isZero_37_ == 1)
{
lean_object* v___x_38_; 
lean_dec(v_h__2_35_);
v___x_38_ = lean_apply_1(v_h__1_34_, v_x_32_);
return v___x_38_;
}
else
{
lean_object* v_one_39_; lean_object* v_n_40_; lean_object* v___x_41_; 
lean_dec(v_h__1_34_);
v_one_39_ = lean_unsigned_to_nat(1u);
v_n_40_ = lean_nat_sub(v_x_33_, v_one_39_);
v___x_41_ = lean_apply_2(v_h__2_35_, v_x_32_, v_n_40_);
return v___x_41_;
}
}
}
LEAN_EXPORT lean_object* lp_OmegaDR___private_OmegaDR_Noise_0__OmegaDR_winSum_match__1_splitter___boxed(lean_object* v_motive_42_, lean_object* v_x_43_, lean_object* v_x_44_, lean_object* v_h__1_45_, lean_object* v_h__2_46_){
_start:
{
lean_object* v_res_47_; 
v_res_47_ = lp_OmegaDR___private_OmegaDR_Noise_0__OmegaDR_winSum_match__1_splitter(v_motive_42_, v_x_43_, v_x_44_, v_h__1_45_, v_h__2_46_);
lean_dec(v_x_44_);
return v_res_47_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_successMass(lean_object* v_g_48_, lean_object* v_w_49_, lean_object* v_00_u03b8_50_){
_start:
{
lean_object* v___x_51_; lean_object* v___x_52_; 
v___x_51_ = lean_nat_sub(v_w_49_, v_00_u03b8_50_);
v___x_52_ = lp_OmegaDR_OmegaDR_winSum(v_g_48_, v_00_u03b8_50_, v___x_51_);
lean_dec(v___x_51_);
return v___x_52_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_successMass___boxed(lean_object* v_g_53_, lean_object* v_w_54_, lean_object* v_00_u03b8_55_){
_start:
{
lean_object* v_res_56_; 
v_res_56_ = lp_OmegaDR_OmegaDR_successMass(v_g_53_, v_w_54_, v_00_u03b8_55_);
lean_dec(v_w_54_);
return v_res_56_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_OmegaDR_OmegaDR_Basic(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_OmegaDR_OmegaDR_Noise(uint8_t builtin) {
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
