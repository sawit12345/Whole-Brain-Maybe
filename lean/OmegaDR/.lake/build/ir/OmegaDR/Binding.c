// Lean compiler output
// Module: OmegaDR.Binding
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
lean_object* lean_nat_lxor(lean_object*, lean_object*);
lean_object* l_BitVec_rotateRight(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_bind___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_bind___redArg___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_bind(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_bind___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_permBind(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_permBind___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_bind___redArg(lean_object* v_r_1_, lean_object* v_f_2_){
_start:
{
lean_object* v___x_3_; 
v___x_3_ = lean_nat_lxor(v_r_1_, v_f_2_);
return v___x_3_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_bind___redArg___boxed(lean_object* v_r_4_, lean_object* v_f_5_){
_start:
{
lean_object* v_res_6_; 
v_res_6_ = lp_OmegaDR_OmegaDR_bind___redArg(v_r_4_, v_f_5_);
lean_dec(v_f_5_);
lean_dec(v_r_4_);
return v_res_6_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_bind(lean_object* v_w_7_, lean_object* v_r_8_, lean_object* v_f_9_){
_start:
{
lean_object* v___x_10_; 
v___x_10_ = lean_nat_lxor(v_r_8_, v_f_9_);
return v___x_10_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_bind___boxed(lean_object* v_w_11_, lean_object* v_r_12_, lean_object* v_f_13_){
_start:
{
lean_object* v_res_14_; 
v_res_14_ = lp_OmegaDR_OmegaDR_bind(v_w_11_, v_r_12_, v_f_13_);
lean_dec(v_f_13_);
lean_dec(v_r_12_);
lean_dec(v_w_11_);
return v_res_14_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_permBind(lean_object* v_w_15_, lean_object* v_k_16_, lean_object* v_f_17_){
_start:
{
lean_object* v___x_18_; 
v___x_18_ = l_BitVec_rotateRight(v_w_15_, v_f_17_, v_k_16_);
return v___x_18_;
}
}
LEAN_EXPORT lean_object* lp_OmegaDR_OmegaDR_permBind___boxed(lean_object* v_w_19_, lean_object* v_k_20_, lean_object* v_f_21_){
_start:
{
lean_object* v_res_22_; 
v_res_22_ = lp_OmegaDR_OmegaDR_permBind(v_w_19_, v_k_20_, v_f_21_);
lean_dec(v_f_21_);
lean_dec(v_k_20_);
lean_dec(v_w_19_);
return v_res_22_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_OmegaDR_OmegaDR_Basic(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_OmegaDR_OmegaDR_Binding(uint8_t builtin) {
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
