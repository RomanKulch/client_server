#ifndef JSONFILEPARSER_HPP
#define JSONFILEPARSER_HPP

#include <QObject>
#include <QJsonObject>
#include "types.hpp"

class JsonFileParser : public QObject
{
    Q_OBJECT
public:
    explicit JsonFileParser(QObject *parent = nullptr);
    bool parseProtocol(const QByteArray& content, UdpProtocol& outValue);

// private:
//     bool protocolValidation(QJsonObject jsonObj);
};

#endif // JSONFILEPARSER_HPP
