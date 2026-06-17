null bool char str i8 i16 i32 i64 int u8 u16 u32 u64 uint f16 f32 f64 float struct union enum
f32x4 i8x16 i16x8 i32x4 i64x2 u8x16 u16x8 u32x4 u64x2 cf16x8 f64x2
f32x8 i8x32 i16x16 i32x8 i64x4 u8x32 u16x16 u32x8 u64x4 cf16x16 f64x4 i8x64 i16x32 i32x16 i64x8 u8x64 u16x32 u32x16 u64x8 bf16x32 f16x32 f32x16 f64x8
neon_i8x16 neon_i16x8 neon_i32x4 neon_i64x2 neon_u8x16 neon_u16x8 neon_u32x4 neon_u64x2 neon_bf16x8 neon_f16x8 neon_f32x4 neon_f64x2
sve_i8 sve_i16 sve_i32 sve_i64 sve_u8 sve_u16 sve_u32 sve_u64 sve_f16 sve_bf16 sve_f32 sve_f64
const volatile ptr inline scoped shared atomic mutex rw once func scope destructor err switch case default if else for while continue break __pause __continue from import simd inherits unsafe compiler llvm clang asm spawn merge ret main

unsafe {
    compiler {
        llvm {
            %object_header_t = type {
                ptr; *vtable
            }
        }
        clang {
            #include <stdio.h>
            int main() {
                return 0;
            }
        }
        asm {
            @arch = x86_64 {
            }
        }
    }
}
