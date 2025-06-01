#include "responseHandler.hpp"
#include <QDebug>
#include <QFile>
#include <QCryptographicHash>
#include "tools.hpp"

ResponseHandler::ResponseHandler(QObject *parent)
    : QObject{parent}
{}

void ResponseHandler::responseHandler(QSharedPointer<QByteArray> msg) {
    if (nullptr == msg) {
        return;
    }

    ResponseHeader header;
    if (false == deserializeHeader(*msg, header)) {
        return;
    }

    if (false == validateHashSum(*msg, header.hashMd5)) {
        qWarning() << "Wrong data hash sum";
        return;
    }

    if (header.type == kErrMsgType) {
        processError(*msg, header);
    } else if (header.type == kArrDoubleMsgType) {
        processValue(*msg, header);
    } else {
        qDebug() << "wrong message type: " << header.type;
        return;
    }
}

bool ResponseHandler::deserializeHeader(const QByteArray& data, ResponseHeader& header) const {
    if (data.size() < static_cast<qsizetype>(sizeof(header))) {
        qDebug() << "wrong data size: " << data.size() << ", header size: " << static_cast<qsizetype>(sizeof(header));
        return false;
    }

    QDataStream stream(data);
    stream.setByteOrder(kByteOrder);
    stream >> header.type;
    stream >> header.version;
    stream >> header.chunkNum;
    stream >> header.totalByteSize;
    stream >> header.hashMd5;
    return true;
}

void ResponseHandler::processError(const QByteArray& data, ResponseHeader& header) const {
    QString errorMsg = QString::fromUtf8(getPayloadStart(data));
    qDebug() << errorMsg;
    qDebug() << "Server version is: " << header.version;
}

void ResponseHandler::processValue(const QByteArray& data, ResponseHeader& header) {
    qDebug() << "deserializeValue, chunk num: " << header.chunkNum;

    if (getPayloadSize(data) % sizeof(double) != 0) {
        qDebug() << "Invalid payload size for double array";
        return;
    }

    const qsizetype expectedSize = header.totalByteSize / sizeof(double);
    if (false == m_isReserveMemory) {
        m_value.reserve(expectedSize);
        m_isReserveMemory = true;
    }

    const double* doubleData = reinterpret_cast<const double*>(getPayloadStart(data));
    unsigned doubleCount = getPayloadSize(data) / sizeof(double);

    QVector<double> package(doubleData, doubleData + doubleCount);
    m_value.append(std::move(package));

    if (m_value.size() == expectedSize) {
        std::sort(m_value.begin(), m_value.end());
        const QString fileName = "doubleArr";
        saveToBinaryFile(fileName);
    }
}

void ResponseHandler::saveToBinaryFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open file for writing:" << filename;
        return;
    }

    if (!m_value.isEmpty()) {
        qint64 bytesWritten = file.write(
            reinterpret_cast<const char*>(m_value.constData()),
            m_value.size() * sizeof(double)
            );

        const qint64 valueSize = m_value.size() * sizeof(double);
        if (bytesWritten != valueSize) {
            qWarning() << "Failed to write all data";
        }
    }

    file.close();

    qDebug() << "File saved";
}

size_t ResponseHandler::getPayloadSize(const QByteArray& data) const {
    return data.size() - sizeof(ResponseHeader);
}

const char* ResponseHandler::getPayloadStart(const QByteArray& data) const {
    return (data.constData() + sizeof(ResponseHeader));
}

bool ResponseHandler::validateHashSum(const QByteArray& data, uint32_t expectedHash) const {
    QByteArrayView dataOnly(getPayloadStart(data), getPayloadSize(data));
    return Tools::getHashSum(dataOnly) == expectedHash;
}
