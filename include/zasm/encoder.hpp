#pragma once

#include "errors.hpp"
#include "expected.hpp"
#include "instruction.hpp"

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>

namespace zasm
{
    // Encoder context used for serialization by the Program.
    struct EncoderContext;

    // A small buffer which holds the bytes of a single encoded instruction and the length.
    struct EncoderBuffer
    {
        std::array<uint8_t, 15> data{};
        uint8_t length{};
    };
    static_assert(sizeof(EncoderBuffer) == 16);

    // This function might change some operands internally in order to encode without a context.
    // The purpose of this function is to generate instructions for the assembler before
    // full serialization. This allows to query instruction meta data such as operand access
    // and CPU flags, this can be also used to estimate the size.
    Error encodeEstimated(
        EncoderBuffer& buf, ZydisMachineMode mode, Instruction::Prefix prefixes, ZydisMnemonic id,
        const Instruction::Operands& operands);

    // Encodes with full context. This function still allows labels to be unbound and will not error
    // instead a temporary value be usually encoded. It is expected for the serialization to handle this
    // with multiple passes.
    Error encodeFull(
        EncoderBuffer& buf, EncoderContext& ctx, ZydisMachineMode mode, Instruction::Prefix prefixes, ZydisMnemonic mnemonic,
        const Instruction::Operands& operands);

    // Helper function that unpacks the instruction and calls the explicit encodeFull variant.
    // Only explicit operands will be considered for the encoder request.
    Error encodeFull(EncoderBuffer& buf, EncoderContext& ctx, ZydisMachineMode mode, const Instruction& instr);

} // namespace zasm