#include "udpServerReader.h"
#include <QDebug>
// #include <QNetworkDatagram>
// #include <thread> // rm this

UdpServerReader::UdpServerReader(QObject *parent) : QObject{parent} {
    m_rxSocket = new QUdpSocket(this);
    // m_txSocket = new QUdpSocket(this);
    // quint16 listenPort = 12345;
    // if (!m_rxSocket->bind(QHostAddress::AnyIPv4, listenPort)) {
    //     qCritical() << "Failed to bind receive socket";
    //     return;
    // }
    // connect(m_rxSocket, &QUdpSocket::readyRead, this, &UdpServerReader::read);
    // qDebug() << "Listening on port" << listenPort;

}

// void UdpServerReader::init(quint16 listenPort) {

// }

void UdpServerReader::start(quint16 listenPort) {
    qDebug() << "Listening on port" << listenPort;
    if (!m_rxSocket->bind(QHostAddress::AnyIPv4, listenPort)) {
        qCritical() << "Failed to bind receive socket";
        return;
    }
    connect(m_rxSocket, &QUdpSocket::readyRead, this, &UdpServerReader::read);
}


void UdpServerReader::read() {
    while (m_rxSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_rxSocket->receiveDatagram();
        qDebug() << "Get:" << datagram.data();
        emit handleMsg(datagram);
    }

    // while (true) {
    //     qDebug() << "read";
    //     // std::this_thread::sleep_for(std::chrono::seconds(1));
    // }
}

// void UdpServerReader::write() {
//     while (true) {
//         qDebug() << "write";
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
// }
