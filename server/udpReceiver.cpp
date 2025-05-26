#include "UdpReceiver.h"
#include <QDebug>
// #include <QNetworkDatagram>
#include <thread> // rm this

UdpReceiver::UdpReceiver(QObject *parent) : QObject{parent} {
    mRxSocket = new QUdpSocket(this);
}

// void UdpReceiver::init(quint16 listenPort) {

// }

void UdpReceiver::start(quint16 listenPort) {
    qDebug() << "start id:" << std::hash<std::thread::id>{}(std::this_thread::get_id());
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
        qDebug() << "read id:" << std::hash<std::thread::id>{}(std::this_thread::get_id());
        emit handleMsg(datagram);
    }
}

