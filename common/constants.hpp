#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>
#include <QDataStream>

struct RequestHeader {
    uint8_t version;
    uint8_t reserve;
};

constexpr decltype(RequestHeader::version) kProtocolVersion = 1;
constexpr decltype(RequestHeader::reserve) kReserve = 0;

struct ResponseHeader {
	uint8_t type;
	uint8_t version;
	uint16_t chunkNum;
	uint32_t totalByteSize;
    uint32_t hashMd5;
};

constexpr uint32_t kNumElements = 1000;
constexpr uint8_t kMinNumArguments = 2;
constexpr uint16_t kChunkSize = 1400;

constexpr uint8_t kRequestSize = 10;
constexpr uint8_t kVersionIndex = 0;
constexpr uint8_t kValueIndex = 2;
constexpr uint8_t kFilePathIndex = 1;

constexpr uint8_t kErrMsgType = 0;
constexpr uint8_t kArrDoubleMsgType = 1;

constexpr QDataStream::ByteOrder kByteOrder = QDataStream::LittleEndian;

#endif // CONSTANTS_H
