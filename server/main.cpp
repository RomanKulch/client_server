#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include "udpServer.h"

// #include "types.hpp"
// #include "jsonFileParser.hpp"

bool readFile(const QString &filePath, QByteArray& content) {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "File doesn't exist: " << filePath;
        return false;
    }

    content = file.readAll();
    file.close();

    return true;
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QStringList arguments = QCoreApplication::arguments();

    // if (arguments.empty()) { // fix it >= 1
    //     qDebug() << "No path to the protol file";
    //     return 1;
    // }

    // const QString filePath = arguments[1]; //
    // QByteArray content;
    // if (!readFile(filePath, content)) {
    //     qDebug() << "Reading file failed";
    //     return 1;
    // }

    UdpServer server{};
    server.init();
    server.start();

    // QObject::connect(&app, &QCoreApplication::aboutToQuit, &readThread, &QThread::quit);
    // QObject::connect(&app, &QCoreApplication::aboutToQuit, &writeThread, &QThread::quit);


    // TODO: terminate() -> QObject::connect(&app, &QCoreApplication::aboutToQuit, &readThread, &QThread::quit);?????

    return app.exec();
}
