#include "UdpSender.h"
#include <QDebug>
#include <thread> // rm this
// #include <iostream>
UdpSender::UdpSender(QObject *parent) : QObject{parent} {
    mTxSocket = new QUdpSocket(this);
}

void UdpSender::write(QNetworkDatagram datagram) {
    qDebug() << "write:" << datagram.data();
    qDebug() << "write id:" << std::hash<std::thread::id>{}(std::this_thread::get_id());
    // std::cout << "*****\n";
}
