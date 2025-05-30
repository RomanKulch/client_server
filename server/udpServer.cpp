#include "udpServer.h"
#include <QDebug>
#include <QDataStream>
#include <QIODevice>
#include "Constants.hpp"
#include "randomUniqueArray.hpp"

UdpServer::UdpServer(QObject *parent) : QObject{parent} {}

void UdpServer::init() {
    mReceiver.moveToThread(&mReadThread);
    mSender.moveToThread(&mWriteThread);

    QObject::connect(&mReadThread, &QThread::started, [this]() {
        constexpr quint16 port = 12347;
        QMetaObject::invokeMethod(&mReceiver, "start", Q_ARG(quint16, port));
    });

    QObject::connect(&mReadThread, &QThread::finished, &mReceiver, &QObject::deleteLater);
    QObject::connect(&mWriteThread, &QThread::finished, &mSender, &QObject::deleteLater);

    QObject::connect(&mReceiver, &UdpReceiver::handleMsg,
                     this, &UdpServer::dispatcher,
                     Qt::QueuedConnection);

    QObject::connect(this, &UdpServer::sendMsg,
                     &mSender, &UdpSender::send,
                     Qt::QueuedConnection);
}

void UdpServer::start() {
    mReadThread.start();
    mWriteThread.start();

}

void UdpServer::dispatcher(QNetworkDatagram request) {
    qDebug() << "UdpServer::dispatcher id:" << std::hash<std::thread::id>{}(std::this_thread::get_id());
    qDebug() << "dispatcher:" << request.data();

    // make a func
    ClientData clientData{};
    if (false == deserializeClientRequest(request.data(), clientData)) {
        qDebug() << "request type failed failed";
        return;
    }
    clientData.addr = request.senderAddress();
    clientData.port = request.senderPort();

    if (clientData.version < kProtocolVersion) {
        sendError(clientData);
    } else {
        constexpr uint8_t kArrDoubleMsgType = 1;
        QVector<double> response = RandomUniqueArray::generateUniqueValues(clientData.value, kNumElements);
        qDebug() << "response size" << response.size();
        const uint32_t totalDataSize = static_cast<uint32_t>(response.size() * sizeof(double));
        sendChunkByChunk(clientData, kArrDoubleMsgType, totalDataSize
                         , reinterpret_cast<const char*>(response.constData()));
    }
}

bool UdpServer::deserializeClientRequest(const QByteArray& val, ClientData& clientData) const {
    if (val.size() < kRequestSize) {
        qDebug() << "wrong value size";
        return false;
    }

    clientData.version = val[kVersionIndex];
    clientData.value = val[kValueIndex];
    qDebug() << "version: " << clientData.version << ", val: " << clientData.value;
    return true;
}

void UdpServer::sendError(const ClientData& clientData) {
    constexpr uint8_t kErrMsgType = 0;
    QString logMessage;
    QTextStream txtStream(&logMessage);
    txtStream << "The client version: " << clientData.version
            << " is lower than the server version: " << kProtocolVersion;
    qDebug() << logMessage;

    sendChunkByChunk(clientData, kErrMsgType, logMessage.size(), logMessage.toUtf8().constData());
}

void UdpServer::sendChunkByChunk(const ClientData& clientData, const uint8_t msgType, const uint32_t dataSizeBytes, const char* rawData) {
    struct Header {
        uint8_t type;
        uint8_t version;
        uint16_t chunkNum;
        uint32_t totalSize;
    };

    Header header{msgType, kProtocolVersion, 0, dataSizeBytes};
    const uint16_t totalChunks = (dataSizeBytes + kChunkSize - 1) / kChunkSize;
// add byte order
    for (uint16_t i = 0; i < totalChunks; ++i) {
        header.chunkNum = i;

        const uint32_t startByte = static_cast<uint32_t>(i) * kChunkSize;
        const uint32_t remainingBytes = dataSizeBytes - startByte;
        const uint16_t currentChunkSize = static_cast<uint16_t>(qMin(static_cast<uint32_t>(kChunkSize), remainingBytes));

        QByteArray chunk;
        chunk.reserve(sizeof(Header) + currentChunkSize);

        chunk.append(reinterpret_cast<const char*>(&header), sizeof(Header));
        chunk.append(rawData + startByte, currentChunkSize);

        auto pOutputData = QSharedPointer<QNetworkDatagram>::create(std::move(chunk), clientData.addr, clientData.port);
        emit sendMsg(pOutputData);
        qDebug() << "chunk.size: " << chunk.size();

        QThread::usleep(100); // 0.1ms
    }

    qDebug() << "Sent" << totalChunks << "chunks, payload size:" << dataSizeBytes << "bytes";
}
