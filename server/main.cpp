#include <QCoreApplication>
#include "udpServer.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    UdpServer server;
    server.init();
    server.start();

    // QObject::connect(&app, &QCoreApplication::aboutToQuit, &readThread, &QThread::quit);
    // QObject::connect(&app, &QCoreApplication::aboutToQuit, &writeThread, &QThread::quit);


    // TODO: terminate() -> QObject::connect(&app, &QCoreApplication::aboutToQuit, &readThread, &QThread::quit);?????

    return app.exec();
}
