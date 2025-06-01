#include "udpSender.hpp"
#include <QDebug>

UdpSender::UdpSender(QObject *parent) : QObject{parent} {
    m_txSocket = new QUdpSocket(this);
}

void UdpSender::send(QSharedPointer<QNetworkDatagram> pDatagram) {
    qint64 bytesSent = m_txSocket->writeDatagram(*pDatagram);

    if (bytesSent == -1) {
        qWarning() << "Error send msg!";
    }
}
