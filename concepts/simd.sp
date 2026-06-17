simd {
    @feature = AVX512 {// 512 bits so 64 bytes
        for (i32 i = 0; i < ele; i += 64) { // Incrementation of 64 so SIMD gets a hint to vectorize it
            *a *= i; // Assume "a" is a reference
        } // No need to mask since AVX512 already has great automatic masking
    }
    @feature = AVX2 {
        // Recreate the logic again but with your own masking
    }
    @feature = SSE4: inherits(AVX2) {
        // Compiler could take the masking hint from AVX2 and automatically infer, no need to rewrite logic
    }
    @feature = SVE {
       // I don't know how arm64 SIMD works
    }
}