#include "UdpSender.h"
#include <QDebug>
// #include <thread> // rm this

UdpSender::UdpSender(QObject *parent) : QObject{parent} {
    mTxSocket = new QUdpSocket(this);
}

void UdpSender::send(QSharedPointer<QNetworkDatagram> pDatagram) {
    // qDebug() << "write:" << pDatagram->data();
    // qDebug() << "UdpSender::write id:" << std::hash<std::thread::id>{}(std::this_thread::get_id());
    qint64 bytesSent = mTxSocket->writeDatagram(*pDatagram);

    if (bytesSent != -1) {
        qDebug() << "Msg sent";
    } else {
        qDebug() << "Error send msg!";
    }
}
