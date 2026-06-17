unsafe {
    compiler { // This is a basic pseudocode example of compiler modifications,
        __compiler.lexer { // Block functions can also be user made, I can discuss how it would work in func.cpp
            add_keyword(class);
            add_keyword(public);
            add_keyword(private);
            add_keyword(friend);
            add_keyword(virtual);
        }
        __compiler.ast{
        func some_compiler_block_local_function();
        };
        __compiler.semantics.add_type_rule(/*...*/);
        __compiler.ir.add_intrinsic(/*...*/);
        }
        // Sample clang for testing parser
        clang {
            #include <stdio.h>
            int main() {
                return 0;
            }
        }
        // Raw LLVM IR, can be defined outside if there's some hyper optimization of the core language that is needed otherwise it will be mostly used within compiler
        llvm {
            %object_header_t = type {
                ptr; *vtable
            }
            %vtable_entry_t = type {
                ptr; fn ptr
            }
            define void @vtable_dispatch(ptr %obj, i32 %slot, ptr %arg) {
                %vtable = load ptr, ptr %obj
                %entry = getelementptr %vtable_entry_t, ptr %vtable, i32 %slot
                %fn = load ptr, ptr %entry
                call void %fn(ptr %obj, ptr %arg)
                ret void
            }
        }
        asm { // You can use raw assembly if needed, though you may need to specifically target architecture
            @arch = x86_64 {
            // intel x86 no OS
                test:
                    mov s1,msg
                    cld
                    lodsb
                chrlp:
                    call outchr
                    lodsb
                    or al, al
                    bne chrlp
                    ret
                msg: db 'Hello x86!', 0xa, 0x0
            }
            @arch = arm64 {
            // arm64 using kernel syscalls
                .global _start
                .section .data
                msg: .ascii "Hello, ARM64!\n"
                len = . - msg

                .section .text
                _start:
                    mov x0, #1
                    ldr x1, =msg
                    mov x2, #len
                    mov x8, #64
                    svc #0
                    mov x0, #0
                    mov x8, #93
                    svc #0
            }
        }
    }
}

// There is an interesting quirk, compiler modifications are technically safe because once they are injected they still get the same safety passes as normal code, though I can't prove if it would break anything or not
// You can still make unsafe libraries if you explicitly opt in and the user can only call those by wrapping unsafe{}