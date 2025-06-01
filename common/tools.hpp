#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <QString>
#include <QByteArray>
#include <QJsonObject>

class Tools {
public:
    static bool getFileContent(QByteArray& content);
    static bool readFile(const QString &filePath, QByteArray& content);
    static bool getJsonObject(const QByteArray& content, QJsonObject& jsonObj);
    static bool isValidPort(quint16 port);
    static uint32_t getHashSum(QByteArrayView data);
};

#endif // TOOLS_HPP
