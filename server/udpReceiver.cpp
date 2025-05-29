#include "UdpReceiver.h"
#include <QDebug>
#include <thread> // rm this

UdpReceiver::UdpReceiver(QObject *parent) : QObject{parent} {
    mRxSocket = new QUdpSocket(this);
}

void UdpReceiver::start(quint16 listenPort) {
    qDebug() << "UdpReceiver::start id:" << std::hash<std::thread::id>{}(std::this_thread::get_id());
    qDebug() << "Listening on port" << listenPort;
    if (!mRxSocket->bind(QHostAddress::AnyIPv4, listenPort)) {
        qCritical() << "Failed to bind receive socket";
        return;
    }
    connect(mRxSocket, &QUdpSocket::readyRead, this, &UdpReceiver::read);
}

void UdpReceiver::read() {
    while (mRxSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = mRxSocket->receiveDatagram();
        qDebug() << "Get:" << datagram.data();
        qDebug() << "UdpReceiver::read id:" << std::hash<std::thread::id>{}(std::this_thread::get_id());
        emit handleMsg(std::move(datagram));
    }
}

