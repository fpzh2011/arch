#include <asmjit/x86.h>
#include <stdio.h>
#include <gtest/gtest.h>

class Asm {
public:
    Asm() {
        code.init(rt.environment());
    }

protected:
    asmjit::JitRuntime rt;
    asmjit::CodeHolder code;
};

TEST(PARSETest, Testxxxx) {
    using namespace asmjit;
    typedef int (*Func)(void);
    JitRuntime rt;  // Runtime specialized for JIT code execution.

    CodeHolder code;              // Holds code and relocation information.
    code.init(rt.environment());  // Initialize code to match the JIT environment.

    x86::Assembler a(&code);  // Create and attach x86::Assembler to code.
    a.mov(x86::eax, 1);       // Move one to eax register.
    a.ret();                  // Return from function.
    // ===== x86::Assembler is no longer needed from here and can be destroyed =====

    Func fn;                         // Holds address to the generated function.
    Error err = rt.add(&fn, &code);  // Add the generated code to the runtime.
    if (err)
        return;  // Handle a possible error returned by AsmJit.
    // ===== CodeHolder is no longer needed from here and can be destroyed =====

    int result = fn();       // Execute the generated code.
    printf("%d\n", result);  // Print the resulting "1".

    // All classes use RAII, all resources will be released before `main()` returns,
    // the generated function can be, however, released explicitly if you intend to
    // reuse or keep the runtime alive, which you should in a production-ready code.
    rt.release(fn);
}
