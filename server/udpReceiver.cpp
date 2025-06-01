#include "udpReceiver.hpp"
#include <QDebug>

UdpReceiver::UdpReceiver(QObject *parent) : QObject{parent} {
    m_rxSocket = new QUdpSocket(this);
}

void UdpReceiver::start(quint16 listenPort) {
    qDebug() << "Listening on port" << listenPort;
    if (!m_rxSocket->bind(QHostAddress::AnyIPv4, listenPort)) {
        qCritical() << "Failed to bind receive socket";
        return;
    }
    connect(m_rxSocket, &QUdpSocket::readyRead, this, &UdpReceiver::read);
}

void UdpReceiver::read() {
    while (m_rxSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_rxSocket->receiveDatagram();
        emit handleMsg(std::move(datagram));
    }
}

