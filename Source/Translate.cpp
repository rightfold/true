#include "Translate.hpp"

namespace
{
    void TranslatePush(Truec::Translate const& t, llvm::Value* x)
    {
        // Increment the stack pointer.
        auto stackPointer = t.IrBuilder->CreateLoad(t.StackPointerPointer);
        auto newStackPointer =
            t.IrBuilder->CreateConstInBoundsGEP1_32(nullptr, stackPointer, 1);
        t.IrBuilder->CreateStore(newStackPointer, t.StackPointerPointer);

        // Store the value at the new stack pointer.
        t.IrBuilder->CreateStore(x, newStackPointer);
    }

    llvm::Value* TranslatePop(Truec::Translate const& t)
    {
        // Decrement the stack pointer.
        auto stackPointer = t.IrBuilder->CreateLoad(t.StackPointerPointer);
        auto newStackPointer =
            t.IrBuilder->CreateConstInBoundsGEP1_32(nullptr, stackPointer, -1);
        t.IrBuilder->CreateStore(newStackPointer, t.StackPointerPointer);

        // Load the value at the previous stack pointer.
        return t.IrBuilder->CreateLoad(stackPointer);
    }
}

void Truec::TranslateStatement(Translate const& t, Statement const& s)
{
    switch (s.Type)
    {
        case StatementType::PushInteger: {
            auto x = std::get<std::int32_t>(s.Value);
            auto llt = llvm::Type::getInt32Ty(*t.LlvmContext);
            auto llx = llvm::ConstantInt::getSigned(llt, x);
            TranslatePush(t, llx);
            break;
        }

        case StatementType::Duplicate: {
            auto x = TranslatePop(t);
            TranslatePush(t, x);
            TranslatePush(t, x);
            break;
        }

        case StatementType::Drop: {
            TranslatePop(t);
            break;
        }

        case StatementType::Swap: {
            auto x = TranslatePop(t);
            auto y = TranslatePop(t);
            TranslatePush(t, x);
            TranslatePush(t, y);
            break;
        }

        case StatementType::Rotate: {
            auto x = TranslatePop(t);
            auto y = TranslatePop(t);
            auto z = TranslatePop(t);
            TranslatePush(t, y);
            TranslatePush(t, x);
            TranslatePush(t, z);
            break;
        }

        case StatementType::Add: {
            auto x = TranslatePop(t);
            auto y = TranslatePop(t);
            auto r = t.IrBuilder->CreateAdd(x, y);
            TranslatePush(t, r);
            break;
        }

        case StatementType::Subtract: {
            auto x = TranslatePop(t);
            auto y = TranslatePop(t);
            auto r = t.IrBuilder->CreateSub(y, x);
            TranslatePush(t, r);
            break;
        }

        case StatementType::Multiply: {
            auto x = TranslatePop(t);
            auto y = TranslatePop(t);
            auto r = t.IrBuilder->CreateMul(x, y);
            TranslatePush(t, r);
            break;
        }

        case StatementType::Divide: {
            // TODO: Handle division by zero safely.
            auto x = TranslatePop(t);
            auto y = TranslatePop(t);
            auto r = t.IrBuilder->CreateSDiv(y, x);
            TranslatePush(t, r);
            break;
        }

        case StatementType::Negate: {
            auto zt = llvm::Type::getInt32Ty(*t.LlvmContext);
            auto z  = llvm::ConstantInt::getSigned(zt, 0);
            auto x = TranslatePop(t);
            auto r = t.IrBuilder->CreateSub(z, x);
            TranslatePush(t, r);
            break;
        }
    }
}
