#include "responseHandler.hpp"
#include <QDebug>
#include <QFile>

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
    if (header.chunkNum == 0) {
        mValue.reserve(expectedSize);
    }

    const double* doubleData = reinterpret_cast<const double*>(getPayloadStart(data));
    unsigned doubleCount = getPayloadSize(data) / sizeof(double);

    QVector<double> package(doubleData, doubleData + doubleCount);
    mValue.append(std::move(package));

    if (mValue.size() == expectedSize) {
        std::sort(mValue.begin(), mValue.end());
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

    if (!mValue.isEmpty()) {
        qint64 bytesWritten = file.write(
            reinterpret_cast<const char*>(mValue.constData()),
            mValue.size() * sizeof(double)
            );

        const qint64 valueSize = mValue.size() * sizeof(double);
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
