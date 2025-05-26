#include "udpServerWriter.h"
#include <QDebug>
#include <iostream>
UdpServerWriter::UdpServerWriter(QObject *parent) : QObject{parent} {
    m_txSocket = new QUdpSocket(this);
}

void UdpServerWriter::write(QNetworkDatagram datagram) {
    qDebug() << "write:" << datagram.data();
    std::cout << "*****\n";
}
