#ifndef SWEEP_TYPES_H
#define SWEEP_TYPES_H
#include <stdint.h>

// StandardDatatype, SIMDDataType, GPUDataType? maybe we can implement native GPU accel
typedef enum: uint16_t {
    SWEEP_TYPE_NULL = 0x0000,
    SWEEP_TYPE_BOOL = 0x0001,
    SWEEP_TYPE_CHAR = 0x0002,
    SWEEP_TYPE_STR = 0x0003,

    SWEEP_TYPE_INT8 = 0x0011,               // i8
    SWEEP_TYPE_INT16 = 0x0012,              // i16
    SWEEP_TYPE_INT32 = 0x0013,              // i32
    SWEEP_TYPE_INT64 = 0x0014,              // i64
    SWEEP_TYPE_INT = SWEEP_TYPE_INT64,      // int

    SWEEP_TYPE_UINT8 = 0x0021,              // u8
    SWEEP_TYPE_UINT16 = 0x0022,             // u16
    SWEEP_TYPE_UINT32 = 0x0023,             // u32
    SWEEP_TYPE_UINT64 = 0x0024,             // u64
    SWEEP_TYPE_UINT = SWEEP_TYPE_UINT64,    // uint

    SWEEP_TYPE_FLOAT16 = 0x0031,            // f16
    SWEEP_TYPE_FLOAT32 = 0x0032,            // f32
    SWEEP_TYPE_FLOAT64 = 0x0033,            // f64
    SWEEP_TYPE_FLOAT = SWEEP_TYPE_FLOAT64,  // float

    SWEEP_TYPE_STRUCT = 0x0041,
    SWEEP_TYPE_UNION = 0x0042,
    SWEEP_TYPE_ENUM = 0x0043,
} SweepStandardDataType;

typedef enum: uint16_t {
    // SSE1
    SWEEP_SIMD_FLOAT32x4 = 0x010A,          // f32x4
    // SSE2/SSE4/AVX128
    SWEEP_SIMD_INT8x16 = 0x0121,            // i8x16
    SWEEP_SIMD_INT16x8 = 0x0122,            // i16x8
    SWEEP_SIMD_INT32x4 = 0x0123,            // i32x4
    SWEEP_SIMD_INT64x2 = 0x0124,            // i64x2
    // CFLOAT is conversion type since fp16 only natively was supported in AVX512 and these only existed for storing, they still convert to fp32
    SWEEP_SIMD_CFLOAT16x8 = 0x0125,         // cf16x8
    SWEEP_SIMD_FLOAT64x2 = 0x0126,          // f64x2
    SWEEP_SIMD_UINT8x16 = 0x0127,           // u8x16
    SWEEP_SIMD_UINT16x8 = 0x0128,           // u16x8
    SWEEP_SIMD_UINT32x4 = 0x0129,           // u32x4
    SWEEP_SIMD_UINT64x2 = 0x012A,           // u64x2
    // AVX2
    SWEEP_SIMD_INT8x32 = 0x0131,            // i8x32
    SWEEP_SIMD_INT16x16 = 0x0132,           // i16x16
    SWEEP_SIMD_INT32x8 = 0x0133,            // i32x8
    SWEEP_SIMD_INT64x4 = 0x0134,            // i64x4
    SWEEP_SIMD_UINT8x32 = 0x0135,           // u8x32
    SWEEP_SIMD_UINT16x16 = 0x0136,          // u16x16
    SWEEP_SIMD_UINT32x8 = 0x0137,           // u32x8
    SWEEP_SIMD_UINT64x4 = 0x0138,           // u64x4
    SWEEP_SIMD_CFLOAT16x16 = 0x0139,        // cf16x16
    SWEEP_SIMD_FLOAT32x8 = 0x013A,          // f32x8
    SWEEP_SIMD_FLOAT64x4 = 0x013B,          // f64x4
    // AVX512 masked
    SWEEP_SIMD_INT8x64 = 0x0141,            // i8x64
    SWEEP_SIMD_INT16x32 = 0x0142,           // i16x32
    SWEEP_SIMD_INT32x16 = 0x0143,           // i32x16
    SWEEP_SIMD_INT64x8 = 0x0144,            // i64x8
    SWEEP_SIMD_UINT8x64 = 0x0145,           // u8x64
    SWEEP_SIMD_UINT16x32 = 0x0146,          // u16x32
    SWEEP_SIMD_UINT32x16 = 0x0147,          // u32x16
    SWEEP_SIMD_UINT64x8 = 0x0148,           // u64x8
    SWEEP_SIMD_FLOAT16x32 = 0x0149,         // f16x32
    SWEEP_SIMD_FLOAT32x16 = 0x014A,         // f32x16
    SWEEP_SIMD_FLOAT64x8 = 0x014B,          // f64x8
    // From what I understood, bfloat is float with higher exponent range giving float32 precision but trades off with less decimal digits
    SWEEP_SIMD_BFLOAT16x32 = 0x014C,        // bf16x32
    // NEON
    SWEEP_SIMD_NEON_INT8x16 = 0x0151,       // neon_i8x16
    SWEEP_SIMD_NEON_INT16x8 = 0x0152,       // neon_i16x8
    SWEEP_SIMD_NEON_INT32x4 = 0x0153,       // neon_i32x4
    SWEEP_SIMD_NEON_INT64x2 = 0x0154,       // neon_i64x2
    SWEEP_SIMD_NEON_UINT8x16 = 0x0155,      // neon_u8x16
    SWEEP_SIMD_NEON_UINT16x8 = 0x0156,      // neon_u16x8
    SWEEP_SIMD_NEON_UINT32x4 = 0x0157,      // neon_u32x4
    SWEEP_SIMD_NEON_UINT64x2 = 0x0158,      // neon_u64x2
    SWEEP_SIMD_NEON_FLOAT16x8 = 0x0159,     // neon_f16x8
    SWEEP_SIMD_NEON_FLOAT32x4 = 0x015A,     // neon_f32x4
    SWEEP_SIMD_NEON_FLOAT64x2 = 0x015B,     // neon_f64x2
    SWEEP_SIMD_NEON_BFLOAT16x8 = 0x015C,    // neon_bf16x8
    // SVE
    SWEEP_SIMD_SVE_FLOAT16 = 0x0161,        // sve_f16
    SWEEP_SIMD_SVE_FLOAT32 = 0x0162,        // sve_f32
    SWEEP_SIMD_SVE_FLOAT64 = 0x0163,        // sve_f64
    SWEEP_SIMD_SVE_INT8 = 0x0164,           // sve_i8
    SWEEP_SIMD_SVE_INT16 = 0x0165,          // sve_i16
    SWEEP_SIMD_SVE_INT32 = 0x0166,          // sve_i32
    SWEEP_SIMD_SVE_INT64 = 0x0167,          // sve_i64
    SWEEP_SIMD_SVE_UINT8 = 0x0168,          // sve_u8
    SWEEP_SIMD_SVE_UINT16 = 0x0169,         // sve_u16
    SWEEP_SIMD_SVE_UINT32 = 0x016A,         // sve_u32
    SWEEP_SIMD_SVE_UINT64 = 0x016B,         // sve_u64
    SWEEP_SIMD_SVE_BFLOAT16 = 0x016C,       // sve_bf16
} SweepSIMDDataType;

// 0x0200 reserved for gpu datatypes in future

#endif