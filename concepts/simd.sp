simd {
    @feature == AVX512 {// 512 bits so 64 bytes
        for (i: i32 = 0; i < ele; i += 64) {
            *a *= i;
            // Assume "a" is a reference, the compiler takes the operators and compile down to intrin, or you can use the raw intrin yourself
            // but it would be a clearer naming scheme so instead of __m512i it will be i64x8, i32x16... and function names becoming avx512_i64x8_add();
        } // No need to mask since AVX512 already has great automatic masking
    }
    @feature == AVX2 {
        // Recreate the logic again but with your own masking
    }
    @feature == SSE4: inherits(AVX2) {
        // Compiler could take the masking hint from AVX2 and automatically infer, no need to rewrite logic
    }
    @feature == SVE {
       // I don't know how arm64 SIMD works but i assume we would take a runtime width, it would work wildly different from other simd branches
    }
}