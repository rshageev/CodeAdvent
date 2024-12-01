export module AoC_2020.Day25;

import std;
import utils;

export uint64 Solve_1(const std::filesystem::path& input);
export uint64 Solve_2(const std::filesystem::path& input);

module : private;

uint64 TransformStep(uint64 value, uint64 subject_number) {
    value *= subject_number;
    value = value % 20201227;
    return value;
}

uint64 TransformSN(uint64 subject_number, uint64 loop_size) {
    uint64 value = 1;
    for (uint64 i = 0; i < loop_size; ++i) {
        value = TransformStep(value, subject_number);
    }
    return value;
}

uint64 CalcPublicKey(uint64 loop_size) {
    return TransformSN(7, loop_size);
}

uint64 CalcEncryptionKey(uint64 key, uint64 loop) {
    return TransformSN(key, loop);
}

uint64 GuessLoopSize(uint64 public_key) {
    constexpr uint64 sn = 7;
    uint64 loop = 0;
    uint64 value = 1;
    while (value != public_key) {
        value = TransformStep(value, sn);
        ++loop;
    }
    return loop;
}

uint64 Solve_1(const std::filesystem::path& input)
{
    const uint64 card_pk = 0;
    const uint64 door_pk = 0;

    const auto card_loop = GuessLoopSize(card_pk);
    const auto door_loop = GuessLoopSize(door_pk);

    const auto enc_key = CalcEncryptionKey(card_pk, door_loop);
    return enc_key;
}

uint64 Solve_2(const std::filesystem::path& input)
{
    int result = 0;
    return result;
}