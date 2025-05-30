#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "client.hpp"
#include <tools.hpp>

struct ClientConfig {
    quint16 serverPort;
    double valueToSend;
    QString serverAddr;
};

bool parseClientConfig(const QByteArray& content, ClientConfig& config) {
    const char* serverAddress = "host_address";
    const char* serverPort = "server_port";
    const char* valueToSend = "value_to_send";

    bool result = true;
    auto attributeValid = [&result](const QJsonValue& val) {
        if (val.type() == QJsonValue::Null || val.type() == QJsonValue::Undefined) {
            qDebug() << "This attribute doesn't exist";
            result = false;
        }
    };

    QJsonDocument jsonDoc = QJsonDocument::fromJson(content);
    if (jsonDoc.isNull()) {
        qDebug() << "Invalid JSON";
        return false;
    }

    QJsonObject jsonObj = jsonDoc.object();

    attributeValid(jsonObj[serverPort]);
    attributeValid(jsonObj[valueToSend]);
    attributeValid(jsonObj[serverAddress]);

    result &= jsonObj[serverPort].isDouble()
              && jsonObj[valueToSend].isDouble()
              && jsonObj[serverAddress].isString();

    if (result) {
        config.serverPort = jsonObj[serverPort].toInt();
        config.valueToSend = jsonObj[valueToSend].toDouble();
        config.serverAddr = jsonObj[serverAddress].toString();
    }

    return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QByteArray content;
    if (false == Tools::getFileContent(content)) {
        qDebug() << "Failed to read config file";
        return -1;
    }

    ClientConfig config;
    if (false == parseClientConfig(content, config)) {
        qDebug() << "Failed to parse client config";
        return -1;
    }

    Client client;
    QThread clientThread;

    client.moveToThread(&clientThread);

    QObject::connect(&clientThread, &QThread::started, [&client, &config]() {
        QMetaObject::invokeMethod(&client, "start",
            Qt::QueuedConnection, Q_ARG(QString, config.serverAddr),
            Q_ARG(quint16, config.serverPort), Q_ARG(double, config.valueToSend));
    });

    QObject::connect(&clientThread, &QThread::finished, &client, &QObject::deleteLater);
    QObject::connect(&clientThread, &QThread::finished, &clientThread, &QThread::deleteLater);

    clientThread.start();

    return a.exec();
}
