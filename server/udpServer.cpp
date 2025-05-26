#include "udpServer.h"

UdpServer::UdpServer(QObject *parent)
    : QObject{parent}
{}

void UdpServer::init() {
    mReceiver.moveToThread(&mReadThread);
    mSender.moveToThread(&mWriteThread);

    QObject::connect(&mReadThread, &QThread::started, [this]() {
        constexpr quint16 port = 12347;
        QMetaObject::invokeMethod(&mReceiver, "start", Q_ARG(quint16, port));
    });

    QObject::connect(&mReadThread, &QThread::finished, &mReceiver, &QObject::deleteLater);
    QObject::connect(&mWriteThread, &QThread::finished, &mSender, &QObject::deleteLater);

    QObject::connect(&mReceiver, &UdpReceiver::handleMsg,
                     &mSender, &UdpSender::write,
                     Qt::QueuedConnection);
}

void UdpServer::start() {
    mReadThread.start();
    mWriteThread.start();

}
