#pragma once

#include <llvm/IR/IRBuilder.h>

#include "Ast.hpp"

namespace Truec
{
    class Translate
    {
    public:
        llvm::LLVMContext* LlvmContext;
        llvm::IRBuilder<>* IrBuilder;
        llvm::Value* StackPointerPointer;
    };

    void TranslateStatement(Translate const&, Statement const&);
}
