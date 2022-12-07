/** A subset of basic DSP functions for Logue, split from the main DSP library.
 */
#pragma once


// Import C99 standard integer types (int8_t etc).
#include <cstdint>
#include <cfloat>
#include <climits>



// Conditional compilation control macros:
//
//      DSP_CORTEX_M4           If true, the build target is an ARM Cortex M4
//      DSP_ARM_THUMB2          If true, the build target is using the Thumb2 instruction set.
//      DSP_ARM_AARCH32         If true, the build target is using the 32 bit (v7) ARM instruction set.
//      DSP_ARM_AARCH64         If true, the build target is using the 64 bit (v8/9( ARM instruction set.
//      DSP_INTEL_X64           If true, the build target is X86_64.
//      DSP_STDLIB              If true, code may use CSTDLIB functions.
//
#if !defined(DSP_CORTEX_M4)
#if (ARM_MATH_CM4)
#define DSP_CORTEX_M4           (1)
#else
#define DSP_CORTEX_M4           (0)
#endif
#endif

#if !defined(DSP_ARM_THUMB2)
#if (ARM_MATH_CM4)
#define DSP_ARM_THUMB2          (1)
#else
#define DSP_ARM_THUMB2          (0)
#endif
#endif

#if !defined(DSP_ARM_AARCH32)
#define DSP_ARM_AARCH32         (0)
#endif

#if !defined(DSP_ARM_AARCH64)
#define DSP_ARM_AARCH64         (0)
#endif

#if !defined(DSP_INTEL_X64)
#define DSP_INTEL_X64           (0)
#endif

#if !defined(DSP_CSTDLIB)
#define DSP_CSTDLIB             (0)
#endif


// Numeric format information.
//
//      DSP_IEEE_FLOAT32            true if the 32 bit float format is IEEE compliant.
//      DSP_IEEE_FLOAT64            true if the 64 bit float format is IEEE compliant.
//      DSP_LITTLE_ENDIAN           true if compiling for little endian integer system.
//      DSP_BIG_ENDIAN              true if compiling for a big-endian integer system.
//
#if !defined(DSP_IEEE_FLOAT32)
#define DSP_IEEE_FLOAT32        (1)
#endif

#if !defined(DSP_IEEE_FLOAT64)
#define DSP_IEEE_FLOAT64        (1)
#endif

#if !defined(DSP_LITTLE_ENDIAN)
#define DSP_LITTLE_ENDIAN       (1)
#endif

#if !defined(DSP_BIG_ENDIAN)
#define DSP_BIG_ENDIAN          (!(DSP_LITTLE_ENDIAN))
#endif



// Macros to support common compiler and/or build-target specific optimisations.
//
//      DSP_INLINE              Use in place of the inline keyword to indicate that inlining should be a priority.
//      DSP_NOALIAS             Prefix a function definition to indicate that no memory changes (other than arguments) are local only.
//      __restrict              Indicates that a pointer is not aliased.
//
#if defined(__clang__)
#define DSP_INLINE      inline __attribute__((always_inline))
#define DSP_NOALIAS     __attribute__ ((noalias))
#elif defined(__GNUC__)
#define DSP_INLINE      inline __attribute__((optimize("Ofast"),always_inline))
#define DSP_NOALIAS     __attribute__ ((pure))
#elif _MSC_VER
#define DSP_INLINE      inline
#define NOALIAS __declspec (noalias)
#else
#define DSP_INLINE      inline
#define DSP_NOALIAS
#endif



// Macros for support assert.
//
//      DSP_ASSERT              Maps to C assert() if DSP_USE_CASSERT() is true.
//
#if !defined(DSP_USE_CASSERT)
#define DSP_USE_CASSERT (0)
#endif

#if DSP_USE_CASSERT
#include <cassert>
#define DSP_ASSERT(_x)  do { assert(_x); } while (false)
#else
#define DSP_ASSERT(_x)  do { (void)(_x); } while (false)
#endif



// Macros to support branch hints.
//
//  if (DSP_EXPECT(condition))
//  {
//      // normal case code here
//  }
//  else
//  {
//      // exceptional case(s)
//  }
//
#if defined(__GNUC__) || defined(__clang__)
#define DSP_EXPECT(x)   __builtin_expect(!!(x), 1)
#else
#define DSP_EXPECT(x)   (!!(x))
#endif




// Macros defining all the permissible FPU immediate constants on a Cortex M4.
// These can be used for CM4 specific optimisations, avoiding PC relative load for constant values.
#define CM4_FLOAT_31_0          (31.0000000f)
#define CM4_FLOAT_30_0          (30.0000000f)
#define CM4_FLOAT_29_0          (29.0000000f)
#define CM4_FLOAT_28_0          (28.0000000f)
#define CM4_FLOAT_27_0          (27.0000000f)
#define CM4_FLOAT_26_0          (26.0000000f)
#define CM4_FLOAT_25_0          (25.0000000f)
#define CM4_FLOAT_24_0          (24.0000000f)
#define CM4_FLOAT_23_0          (23.0000000f)
#define CM4_FLOAT_22_0          (22.0000000f)
#define CM4_FLOAT_21_0          (21.0000000f)
#define CM4_FLOAT_20_0          (20.0000000f)
#define CM4_FLOAT_19_0          (19.0000000f)
#define CM4_FLOAT_18_0          (18.0000000f)
#define CM4_FLOAT_17_0          (17.0000000f)
#define CM4_FLOAT_16_0          (16.0000000f)
#define CM4_FLOAT_15_5          (15.5000000f)
#define CM4_FLOAT_15_0          (15.0000000f)
#define CM4_FLOAT_14_5          (14.5000000f)
#define CM4_FLOAT_14_0          (14.0000000f)
#define CM4_FLOAT_13_5          (13.5000000f)
#define CM4_FLOAT_13_0          (13.0000000f)
#define CM4_FLOAT_12_5          (12.5000000f)
#define CM4_FLOAT_12_0          (12.0000000f)
#define CM4_FLOAT_11_5          (11.5000000f)
#define CM4_FLOAT_11_0          (11.0000000f)
#define CM4_FLOAT_10_5          (10.5000000f)
#define CM4_FLOAT_10_0          (10.0000000f)
#define CM4_FLOAT_9_5            (9.5000000f)
#define CM4_FLOAT_9_0            (9.0000000f)
#define CM4_FLOAT_8_5            (8.5000000f)
#define CM4_FLOAT_8_0            (8.0000000f)
#define CM4_FLOAT_7_75           (7.7500000f)
#define CM4_FLOAT_7_5            (7.5000000f)
#define CM4_FLOAT_7_25           (7.2500000f)
#define CM4_FLOAT_7_0            (7.0000000f)
#define CM4_FLOAT_6_75           (6.7500000f)
#define CM4_FLOAT_6_5            (6.5000000f)
#define CM4_FLOAT_6_25           (6.2500000f)
#define CM4_FLOAT_6_0            (6.0000000f)
#define CM4_FLOAT_5_75           (5.7500000f)
#define CM4_FLOAT_5_5            (5.5000000f)
#define CM4_FLOAT_5_25           (5.2500000f)
#define CM4_FLOAT_5_0            (5.0000000f)
#define CM4_FLOAT_4_75           (4.7500000f)
#define CM4_FLOAT_4_5            (4.5000000f)
#define CM4_FLOAT_4_25           (4.2500000f)
#define CM4_FLOAT_4_0            (4.0000000f)
#define CM4_FLOAT_3_875          (3.8750000f)
#define CM4_FLOAT_3_75           (3.7500000f)
#define CM4_FLOAT_3_625          (3.6250000f)
#define CM4_FLOAT_3_5            (3.5000000f)
#define CM4_FLOAT_3_375          (3.3750000f)
#define CM4_FLOAT_3_25           (3.2500000f)
#define CM4_FLOAT_3_125          (3.1250000f)
#define CM4_FLOAT_3_0            (3.0000000f)
#define CM4_FLOAT_2_875          (2.8750000f)
#define CM4_FLOAT_2_75           (2.7500000f)
#define CM4_FLOAT_2_625          (2.6250000f)
#define CM4_FLOAT_2_5            (2.5000000f)
#define CM4_FLOAT_2_375          (2.3750000f)
#define CM4_FLOAT_2_25           (2.2500000f)
#define CM4_FLOAT_2_125          (2.1250000f)
#define CM4_FLOAT_2_0            (2.0000000f)
#define CM4_FLOAT_1_9375         (1.9375000f)
#define CM4_FLOAT_1_875          (1.8750000f)
#define CM4_FLOAT_1_8125         (1.8125000f)
#define CM4_FLOAT_1_75           (1.7500000f)
#define CM4_FLOAT_1_6875         (1.6875000f)
#define CM4_FLOAT_1_625          (1.6250000f)
#define CM4_FLOAT_1_5625         (1.5625000f)
#define CM4_FLOAT_1_5            (1.5000000f)
#define CM4_FLOAT_1_4375         (1.4375000f)
#define CM4_FLOAT_1_375          (1.3750000f)
#define CM4_FLOAT_1_3125         (1.3125000f)
#define CM4_FLOAT_1_25           (1.2500000f)
#define CM4_FLOAT_1_1875         (1.1875000f)
#define CM4_FLOAT_1_125          (1.1250000f)
#define CM4_FLOAT_1_0625         (1.0625000f)
#define CM4_FLOAT_1_0            (1.0000000f)
#define CM4_FLOAT_0_96875        (0.9687500f)
#define CM4_FLOAT_0_9375         (0.9375000f)
#define CM4_FLOAT_0_90625        (0.9062500f)
#define CM4_FLOAT_0_875          (0.8750000f)
#define CM4_FLOAT_0_84375        (0.8437500f)
#define CM4_FLOAT_0_8125         (0.8125000f)
#define CM4_FLOAT_0_78125        (0.7812500f)
#define CM4_FLOAT_0_75           (0.7500000f)
#define CM4_FLOAT_0_71875        (0.7187500f)
#define CM4_FLOAT_0_6875         (0.6875000f)
#define CM4_FLOAT_0_65625        (0.6562500f)
#define CM4_FLOAT_0_625          (0.6250000f)
#define CM4_FLOAT_0_59375        (0.5937500f)
#define CM4_FLOAT_0_5625         (0.5625000f)
#define CM4_FLOAT_0_53125        (0.5312500f)
#define CM4_FLOAT_0_5            (0.5000000f)
#define CM4_FLOAT_0_484375       (0.4843750f)
#define CM4_FLOAT_0_46875        (0.4687500f)
#define CM4_FLOAT_0_453125       (0.4531250f)
#define CM4_FLOAT_0_4375         (0.4375000f)
#define CM4_FLOAT_0_421875       (0.4218750f)
#define CM4_FLOAT_0_40625        (0.4062500f)
#define CM4_FLOAT_0_390625       (0.3906250f)
#define CM4_FLOAT_0_375          (0.3750000f)
#define CM4_FLOAT_0_359375       (0.3593750f)
#define CM4_FLOAT_0_34375        (0.3437500f)
#define CM4_FLOAT_0_328125       (0.3281250f)
#define CM4_FLOAT_0_3125         (0.3125000f)
#define CM4_FLOAT_0_296875       (0.2968750f)
#define CM4_FLOAT_0_28125        (0.2812500f)
#define CM4_FLOAT_0_265625       (0.2656250f)
#define CM4_FLOAT_0_25           (0.2500000f)
#define CM4_FLOAT_0_2421875      (0.2421875f)
#define CM4_FLOAT_0_234375       (0.2343750f)
#define CM4_FLOAT_0_2265625      (0.2265625f)
#define CM4_FLOAT_0_21875        (0.2187500f)
#define CM4_FLOAT_0_2109375      (0.2109375f)
#define CM4_FLOAT_0_203125       (0.2031250f)
#define CM4_FLOAT_0_1953125      (0.1953125f)
#define CM4_FLOAT_0_1875         (0.1875000f)
#define CM4_FLOAT_0_1796875      (0.1796875f)
#define CM4_FLOAT_0_171875       (0.1718750f)
#define CM4_FLOAT_0_1640625      (0.1640625f)
#define CM4_FLOAT_0_15625        (0.1562500f)
#define CM4_FLOAT_0_1484375      (0.1484375f)
#define CM4_FLOAT_0_140625       (0.1406250f)
#define CM4_FLOAT_0_1328125      (0.1328125f)
#define CM4_FLOAT_0_125          (0.1250000f)



namespace logue
{
    /** @fn             template <unsigned fbits> static int32_t floatToS32(float n)
     *  @brief          Cast a float to an int32 with specified number of fractional bits, rounding towards zero.
     *  @tparam fbits   The number of fractional integer bits in the output.
     *  @param  n       The float value.
     *  @return         The corresponding int32 value. Undefined if f exceeds the output range.
     *
     *  Examples:
     *
     *      int32_t a = floatToS32<0>(-10.50);      // a = -10
     *      int32_t b = floatToS32<0>(10.50);       // b = 10
     *      int32_t c = floatToS32<1>(12.50);       // c = 25
     *      int32_t d = floatToS32<31>(-2.0);       // d = undefined
     *      int32_t e = floatToS32<31>(2.0);        // e = undefined
     *      int32_t y = floatToS32<31>(x);          // x:[-1, +1) => y:[0x80000000, 0x7fffffff], undefined elsewhere.
     */


    /** @fn             template <unsigned fbits> static uint32_t floatToU32<>(float n)
     *  @brief          Cast a float to an uint32 with specified number of fractional bits, rounding towards zero.
     *  @tparam fbits   The number of fractional integer bits in the output.
     *  @param  n       The float value.
     *  @return         The corresponding uint32 value. Undefined if f exceeds the output range.
     *
     *  Examples:
     *
     *      uint32_t a = floatToU32<0>(-10.50);     // a = undefined
     *      uint32_t b = floatToU32<0>(10.50);      // a = 10
     *      uint32_t c = floatToU32<1>(12.50);      // a = 25
     *      uint32_t y = floatToU32<32>(x);         // x:[0, +1) => y:[0, 0xffffffff], undefined elsewhere.
     */


    /** @fn             template <unsigned fbits> static int32_t floatToS32_saturating(float n)
     *  @brief          Cast a float to an int32 with specified number of fractional bits, rounding towards zero, with saturation.
     *  @tparam fbits   The number of fractional integer bits in the output.
     *  @param  n       The float value.
     *  @return         The corresponding saturated int32 value.
     *
     *  Examples:
     *
     *      int32_t a = floatToS32_saturating<0>(-10.50);       // a = -10
     *      int32_t b = floatToS32_saturating<0>(10.50);        // b = 10
     *      int32_t c = floatToS32_saturating<1>(12.50);        // c = 25
     *      int32_t d = floatToS32_saturating<31>(-2.0);        // d = INT32_MIN
     *      int32_t e = floatToS32_saturating<31>(2.0);         // e = INT32_MAX
     *      int32_t y = floatToS32_saturating<31>(x);           // y:[0x80000000, 0x7fffffff]
     */


    /** @fn             template <unsigned fbits> static uint32_t saturatingFloatToU32(float n)
     *  @brief          Cast a float to an int32 with specified number of fractional bits, rounding towards zero, with saturation.
     *  @tparam fbits   The number of fractional integer bits in the output.
     *  @param  n       The float value.
     *  @return         The corresponding saturated uint32 value.
     *
     *  Examples:
     *
     *      int32_t a = floatToU32_saturating<0>(-10.50);       // a = -10
     *      int32_t b = floatToU32_saturating<0>(10.50);        // b = 10
     *      int32_t c = floatToU32_saturating<1>(12.50);        // c = 25
     *      int32_t d = floatToU32_saturating<31>(-2.0);        // d = 0
     *      int32_t e = floatToU32_saturating<31>(2.0);         // e = UINT32_MAX
     *      int32_t y = floatToU32_saturating<31>(x);           // y:[0, 0xffffffff]
     */


#if DSP_CORTEX_M4
    // Template specialisations for Cortex M4.
    // Cortex M4's standard float conversion primitives are saturating. Unfortunately, gcc will not
    // generate these opcodes, so inline assembler is the only way to benefit.
    // NB: the _0 macros are because the assembler is too dumb to accept #0 for the fractional bit count.
#define DSP_CM4_FLOAT_TO_INTEGER(output, input, fbits, type32)  asm (                   \
            "vcvt." type32 ".f32 %[s_input], %[s_input], %[k_fbits]\n"                  \
            "vmov                %[r_output], %[s_input]\n"                             \
            : [r_output] "=r" (output), [s_input] "+t" (input)                          \
            : [k_fbits] "n" (fbits))

#define DSP_CM4_FLOAT_TO_INTEGER_0(output, input, type32)  asm (                        \
            "vcvt." type32 ".f32 %[s_input], %[s_input]\n"                              \
            "vmov                %[r_output], %[s_input]\n"                             \
            : [r_output] "=r" (output), [s_input] "+t" (input))

#define DSP_CM4_INTEGER_TO_FLOAT(output, input, fbits, type32)  asm (                   \
            "vmov                %[s_output], %[r_input]\n"                             \
            "vcvt.f32." type32 " %[s_output], %[s_output], %[k_fbits]\n"                \
            : [s_output] "=t" (output)                                                  \
            : [r_input] "r" (input), [k_fbits] "n" (fbits))

#define DSP_CM4_INTEGER_TO_FLOAT_0(output, input, type32)  asm (                        \
            "vmov                %[s_output], %[r_input]\n"                             \
            "vcvt.f32." type32 " %[s_output], %[s_output]\n"                            \
            : [s_output] "=t" (output) : [r_input] "r" (input))

    template <unsigned fbits> DSP_INLINE static float    s32ToFloat(int32_t n) { float result; DSP_CM4_INTEGER_TO_FLOAT(result, n, fbits, "s32"); return result; }
    template <unsigned fbits> DSP_INLINE static float    u32ToFloat(uint32_t n) { float result; DSP_CM4_INTEGER_TO_FLOAT(result, n, fbits, "u32"); return result; }
    template <unsigned fbits> DSP_INLINE static int32_t  floatToS32(float n) { int32_t result; DSP_CM4_FLOAT_TO_INTEGER(result, n, fbits, "s32"); return result; }
    template <unsigned fbits> DSP_INLINE static uint32_t floatToU32(float n) { uint32_t result; DSP_CM4_FLOAT_TO_INTEGER(result, n, fbits, "u32"); return result; }
    template <unsigned fbits> DSP_INLINE static int32_t  floatToS32_saturating(float n) { return floatToS32<fbits>(n); }
    template <unsigned fbits> DSP_INLINE static uint32_t floatToU32_saturating(float n) { return floatToU32<fbits>(n); }

    template <> float    DSP_INLINE s32ToFloat<0>(int32_t n) { float result; DSP_CM4_INTEGER_TO_FLOAT_0(result, n, "s32"); return result; }
    template <> float    DSP_INLINE u32ToFloat<0>(uint32_t n) { float result; DSP_CM4_INTEGER_TO_FLOAT_0(result, n, "u32"); return result; }
    template <> int32_t  DSP_INLINE floatToS32<0>(float n) { int32_t result; DSP_CM4_FLOAT_TO_INTEGER_0(result, n, "s32"); return result; }
    template <> uint32_t DSP_INLINE floatToU32<0>(float n) { uint32_t result; DSP_CM4_FLOAT_TO_INTEGER_0(result, n, "u32"); return result; }

#else   // Platform independent float<->int conversions.

    template <unsigned fbits> DSP_INLINE static float    s32ToFloat(int32_t n) { return float(n) * (1.0f / float(1ull << fbits)); }
    template <unsigned fbits> DSP_INLINE static float    u32ToFloat(uint32_t n) { return float(n) * (1.0f / float(1ull << fbits)); }
    template <unsigned fbits> DSP_INLINE static int32_t  floatToS32(float n) { return int32_t(n * float(1ull << fbits)); }
    template <unsigned fbits> DSP_INLINE static uint32_t floatToU32(float n) { return uint32_t(n * float(1ull << fbits)); }
    template <unsigned fbits> DSP_INLINE static int32_t  floatToS32_saturating(float n)
    {
        if (n >= float(1ull << (31-fbits))) return INT32_MAX;
        else if (n <= -float(1ull << (31-fbits))) return INT32_MIN;
        else return floatToS32<fbits>(n);
    }
    template <unsigned fbits> DSP_INLINE static uint32_t  floatToU32_saturating(float n)
    {
        if (n >= float(1ull << (32-fbits))) return UINT32_MAX;
        else if (n <= 0.0f) return 0;
        else return floatToU32<fbits>(n);
    }

#endif



    /** Fast multiplication of a float by a integral power of two.
     *
     *  @param x    Initial value.
     *  @param y    The power of two to scale by.
     *  @return     x * 2^y
     *  @warning    If overflow or underflow occurs, the result will be junk. In particular, underflow will
     *              result in a garbage result rather than something more helpful, like zero. Use this method
     *              only where the numeric ranges are well known.
     */
    DSP_INLINE static float floatScaleByPow2(float x, int y)
    {
        union { float f; uint32_t i; } xu;
        xu.f = x;
        xu.i += uint32_t(y << 23);      // Assumes IEEE float-32 format.
        return xu.f;
    }


}   // namespace

