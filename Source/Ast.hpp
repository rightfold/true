#pragma once

#include <cstdint>
#include <variant>
#include <vector>

namespace Truec
{
    enum class StatementType {
        PushInteger,
        // TODO: PushLambda,

        // TODO: Call,
        // TODO: If,
        // TODO: While,

        Duplicate,
        Drop,
        Swap,
        Rotate,
        // TODO: Pick,

        Add,
        Subtract,
        Multiply,
        Divide,
        Negate,
    };

    class Statement
    {
    public:
        StatementType Type;
        std::variant<
            std::int32_t,
            std::vector<Statement>
        > Value;
    };
}
