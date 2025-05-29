#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

constexpr uint8_t kProtocolVersion = 1;

constexpr uint32_t kNumElements = 100;
constexpr uint16_t kChunkSize = 1400;

constexpr uint8_t kRequestSize = 10;
constexpr uint8_t kVersionIndex = 0;
constexpr uint8_t kValueIndex = 2;

#endif // CONSTANTS_H
