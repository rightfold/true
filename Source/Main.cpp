#include "Translate.hpp"

#include <llvm/Support/raw_ostream.h>

int main()
{
    std::vector<Truec::Statement> ast{
        {Truec::StatementType::PushInteger, 1},
        {Truec::StatementType::PushInteger, 2},
        {Truec::StatementType::PushInteger, 3},
        {Truec::StatementType::PushInteger, 4},
        {Truec::StatementType::Rotate,      0},
        {Truec::StatementType::Add,         0},
        {Truec::StatementType::Multiply,    0},
        {Truec::StatementType::Divide,      0},
        {Truec::StatementType::Negate,      0},
    };

    llvm::LLVMContext llvmContext;
    llvm::Module llvmModule("", llvmContext);
    llvm::IRBuilder<> irBuilder(llvmContext);

    auto TrueMain = llvm::Function::Create(
        llvm::FunctionType::get(
            llvm::Type::getVoidTy(llvmContext),
            {
                llvm::Type::getInt32Ty(llvmContext)
                    ->getPointerTo()
                    ->getPointerTo(),
            },
            false
        ),
        llvm::GlobalValue::ExternalLinkage,
        "TrueMain",
        &llvmModule
    );
    auto stackPointerPointer = TrueMain->arg_begin();

    auto entry = llvm::BasicBlock::Create(llvmContext, "", TrueMain);
    irBuilder.SetInsertPoint(entry);

    Truec::Translate translate{
        &llvmContext,
        &irBuilder,
        stackPointerPointer,
    };

    for (auto&& statement : ast)
        Truec::TranslateStatement(translate, statement);

    irBuilder.CreateRetVoid();

    llvmModule.print(llvm::outs(), nullptr);

    return 0;
}
