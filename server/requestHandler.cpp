#include "requestHandler.hpp"
#include <QDebug>
#include <QThread>
#include "constants.hpp"
#include "tools.hpp"
#include "randomUniqueArray.hpp"

RequestHandler::RequestHandler(QNetworkDatagram&& request)
    : QObject{nullptr}
    , m_clientData()
    , m_isValidRequest(true) {
    deserializeClientRequest(request.data());
    m_clientData.addr = request.senderAddress();
    m_clientData.port = request.senderPort();

    setAutoDelete(false);
}

void RequestHandler::run() {
    if (m_isValidRequest) {
        if (m_clientData.version < kProtocolVersion) {
            sendError();
        } else {
            sendResponse();
        }
    }

    emit finished();
}

void RequestHandler::deserializeClientRequest(const QByteArray& val) {
    if (val.size() < kRequestSize) {
        qDebug() << "wrong value size";
        m_isValidRequest = false;
        return;
    }

    m_clientData.version = val[kVersionIndex];
    m_clientData.value = val[kValueIndex];
    qDebug() << "version: " << m_clientData.version << ", val: " << m_clientData.value;
}

void RequestHandler::sendResponse() {
    QVector<double> response = RandomUniqueArray::generateUniqueValues(m_clientData.value, kNumElements);
    qDebug() << "response size" << response.size();
    const uint32_t totalDataSize = static_cast<uint32_t>(response.size() * sizeof(double));
    sendChunkByChunk(kArrDoubleMsgType, totalDataSize
                     , reinterpret_cast<const char*>(response.constData()));
}

void RequestHandler::sendError() {
    QString logMessage;
    QTextStream txtStream(&logMessage);
    txtStream << "The client version: " << m_clientData.version
              << " is lower than the server version: " << kProtocolVersion;
    qDebug() << logMessage;

    sendChunkByChunk(kErrMsgType, logMessage.size(), logMessage.toUtf8().constData());
}

void RequestHandler::sendChunkByChunk(const uint8_t msgType, const uint32_t dataSizeBytes, const char* rawData) {
    ResponseHeader header{msgType, kProtocolVersion, 0, dataSizeBytes, 0};
    const uint16_t totalChunks = (dataSizeBytes + kChunkSize - 1) / kChunkSize;

    for (uint16_t i = 0; i < totalChunks; ++i) {
        const uint32_t startByte = static_cast<uint32_t>(i) * kChunkSize;
        const uint32_t remainingBytes = dataSizeBytes - startByte;
        const uint16_t currentChunkSize = static_cast<uint16_t>(qMin(static_cast<uint32_t>(kChunkSize), remainingBytes));

        QByteArray chunk;
        QByteArrayView dataOnly(rawData + startByte, currentChunkSize);
        header.hashMd5 = Tools::getHashSum(dataOnly);
        header.chunkNum = i;

        chunk.reserve(sizeof(ResponseHeader) + currentChunkSize);
        chunk.append(reinterpret_cast<const char*>(&header), sizeof(ResponseHeader));
        chunk.append(dataOnly);

        auto pOutputData = QSharedPointer<QNetworkDatagram>::create(std::move(chunk), m_clientData.addr, m_clientData.port);
        emit sendMsg(pOutputData);
        qDebug() << "chunk# " << i << " from: " << totalChunks << "sent";

        // fot test purpose
        // QThread::usleep(100);
    }

    qDebug() << "Sent" << totalChunks << "chunks, payload size:" << dataSizeBytes << "bytes";
}

