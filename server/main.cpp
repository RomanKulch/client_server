#include <QCoreApplication>
#include <QThread>
#include "udpServerReader.h"
#include "udpServerWriter.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QThread readThread;
    QThread writeThread;

    UdpServerReader reader;
    UdpServerWriter writer;

    reader.moveToThread(&readThread);
    writer.moveToThread(&writeThread);

    // QObject::connect(&readThread, &QThread::started, &reader, &Worker::doWork);
    QObject::connect(&readThread, &QThread::started, [&reader]() {
        constexpr quint16 port = 12347;
        QMetaObject::invokeMethod(&reader, "start", Q_ARG(quint16, port));
    });


    QObject::connect(&readThread, &QThread::finished, &reader, &QObject::deleteLater);
    QObject::connect(&writeThread, &QThread::finished, &writer, &QObject::deleteLater);

    // QObject::connect(&readThread, &QThread::started, [server]() {
    //     QMetaObject::invokeMethod(server, "read", Qt::QueuedConnection);
    // });

    // QObject::connect(&writeThread, &QThread::started, [server]() {
    //     QMetaObject::invokeMethod(server, "write", Qt::QueuedConnection);
    // });

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &readThread, &QThread::quit);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &writeThread, &QThread::quit);
    // QObject::connect(&app, &QCoreApplication::aboutToQuit, &writeThread, &QThread::quit);


    QObject::connect(&reader, &UdpServerReader::handleMsg,
                     &writer, &UdpServerWriter::write,
                     Qt::QueuedConnection);



    readThread.start();
    writeThread.start();

    return app.exec();
}
