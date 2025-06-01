#include <QCoreApplication>
#include <QDebug>
#include "udpServer.hpp"
#include <tools.hpp>

bool parseServerConfig(const QByteArray& content, quint16& port) {
    const char* serverPort = "server_port";

    QJsonObject jsonObj;
    if (false == Tools::getJsonObject(content, jsonObj)) {
        return false;
    }

    if (jsonObj[serverPort].type() == QJsonValue::Null || jsonObj[serverPort].type() == QJsonValue::Undefined) {
        qDebug() << "Port attribute doesn't exist";
        return false;
    }

    if (false == jsonObj[serverPort].isDouble()) {
        qDebug() << "Port attribute has wrong type";
        return false;
    }

    port = jsonObj[serverPort].toInt();
    return true;
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QByteArray configServerContent;
    if (false == Tools::getFileContent(configServerContent)) {
        qDebug() << "Failed to read config file";
        return -1;
    }

    quint16 serverPort = 0;
    if (false == parseServerConfig(configServerContent, serverPort)) {
        qDebug() << "Failed to parse server config file";
        return -1;
    }

    if (false == Tools::isValidPort(serverPort)) {
        qDebug() << "Server port has wrong value";
        return -1;
    }

    UdpServer server{};
    server.init(serverPort);
    server.start();

    return app.exec();
}
