#include "tools.hpp"
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QCryptographicHash>
#include <constants.hpp>

bool Tools::getFileContent(QByteArray& content) {
    const QStringList arguments = QCoreApplication::arguments();

    if (arguments.size() < kMinNumArguments) {
        qDebug() << "No path to the file";
        return false;
    }

    const QString filePath = arguments[kFilePathIndex];
    // QByteArray content;
    if (!readFile(filePath, content)) {
        qDebug() << "Reading file failed";
        return false;
    }

    return true;
}

bool Tools::readFile(const QString &filePath, QByteArray& content) {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "File doesn't exist: " << filePath;
        return false;
    }

    content = file.readAll();
    file.close();

    return true;
}

bool Tools::getJsonObject(const QByteArray& content, QJsonObject& jsonObj) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(content);
    if (jsonDoc.isNull()) {
        qDebug() << "Invalid JSON";
        return false;
    }

    jsonObj = jsonDoc.object();
    return true;
}

bool Tools::isValidPort(quint16 port) {
    return port >= 1;
}

uint32_t Tools::getHashSum(QByteArrayView data) {
    QByteArray checksum = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    uint32_t hashSum = 0;
    if (static_cast<uint32_t>(checksum.size()) >= sizeof(hashSum)) {
        QDataStream stream(checksum.left(sizeof(hashSum)));
        stream.setByteOrder(kByteOrder);
        stream >> hashSum;
    }

    return hashSum;
}
