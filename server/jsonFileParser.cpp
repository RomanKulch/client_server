#include "jsonFileParser.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QDebug>

// common
namespace {
// const char* version = "protocol_version";
const char* version = "version";
const char* majorVersion = "major_version";
const char* minorVersion = "minor_version";

const char* payload = "payload";
const char* numElements = "num_elements";

// const char* header = "header";
// const char* sizeBytes = "size_bytes";
} // namespace

JsonFileParser::JsonFileParser(QObject *parent) : QObject{parent} {}

bool JsonFileParser::parseProtocol(const QByteArray& content, UdpProtocol& outValue) {
    bool result = true;
    auto attributeValid = [&result](const QJsonValue& val) {
        if (val.type() == QJsonValue::Null || val.type() == QJsonValue::Undefined) {
            qDebug() << "This attribute doesn't exist";
            result = false;
        }
    };

    QJsonDocument jsonDoc = QJsonDocument::fromJson(content);
    QJsonObject jsonObj = jsonDoc.object();

    // attributeValid(jsonObj[version].toObject()[majorVersion]);
    // outValue.majorVersion = jsonObj[version].toObject()[majorVersion].toInt();

    // attributeValid(jsonObj[version].toObject()[minorVersion]);
    // outValue.minorVersion = jsonObj[version].toObject()[minorVersion].toInt();

    attributeValid(jsonObj[version]);
    outValue.version = jsonObj[version].toInt();

    attributeValid(jsonObj[payload].toObject()[numElements]);
    outValue.numElements = jsonObj[payload].toObject()[numElements].toInt();

    // attributeValid(jsonObj[header].toObject()[sizeBytes]);
    // outValue.header.total = jsonObj[header].toObject()[sizeBytes].toInt();

    // attributeValid(jsonObj[header].toObject()[majorVersion].toObject()[sizeBytes]);
    // outValue.header.majorVersionSize = jsonObj[header].toObject()[majorVersion].toObject()[sizeBytes].toInt();

    // attributeValid(jsonObj[header].toObject()[minorVersion].toObject()[sizeBytes]);
    // outValue.header.minorVersionSize = jsonObj[header].toObject()[minorVersion].toObject()[sizeBytes].toInt();

    // qDebug() << "majorVersion: " << outValue.majorVersion;
    // qDebug() << "minorVersion: " << outValue.minorVersion;
    // qDebug() << "numElements: " << outValue.numElements;
    // qDebug() << "total: " << outValue.header.total;
    // qDebug() << "majorVersionSize: " << outValue.header.majorVersionSize;
    // qDebug() << "minorVersionSize: " << outValue.header.minorVersionSize;

    // qDebug() << "vers: " << jsonObj[version].toString();
    // qDebug() << "filePath: " << filePath;

    return result;
}

// bool protocolValidation(QJsonObject jsonObj) {
//     bool result = false;

//     if (jsonObj.isEmpty()) {
//         qDebug() << "json content is empty";
//         return result;
//     }



//     // auto isAttributeValid = [&](const QString& attribute, const QJsonValue::Type type) {
//     //     if (!jsonObj.contains(attribute)) {
//     //         qDebug() << attribute << " attribute is missing";
//     //         return false;
//     //     }

//     //     if (jsonObj[attribute].type() != type) {
//     //         qDebug() << "Type is " << jsonObj[attribute].type() << ", instead of " << type;
//     //         return false;
//     //     }

//     //     return true;
//     // };

//     // result = isAttributeValid(version, );

//     // if (!jsonObj.contains(version)) {
//     //     qDebug() << version << " attribute is missing";
//     //     return result;
//     // }



//     return result;
// }
