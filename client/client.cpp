#include "client.hpp"
#include <QDebug>

Client::Client(QObject *parent) : QObject{parent} {
    mRespHandler.moveToThread(&mHandlerThread);
    QObject::connect(this, &Client::msgHandler,
                     &mRespHandler, &ResponseHandler::responseHandler,
                     Qt::QueuedConnection);
    QObject::connect(&mHandlerThread, &QThread::finished,
                     &mRespHandler, &QObject::deleteLater);
    mHandlerThread.start();
}

Client::~Client() {
    if (mHandlerThread.isRunning()) {
        mHandlerThread.quit();

        constexpr uint16_t waitMs = 1000;
        if (!mHandlerThread.wait(waitMs)) {
            qWarning() << "Thread didn't finish gracefully, terminating";
            mHandlerThread.terminate();
            mHandlerThread.wait(waitMs);
        }
    }
}

void Client::start(QString addr, quint16 port, double num) {
    QHostAddress serverAddr(addr);
    if (serverAddr.isNull()) {
        qWarning() << "Invalid server address:" << addr;
        return;
    }

    m_pSocket = std::make_unique<QUdpSocket>();

    QObject::connect(m_pSocket.get(), &QUdpSocket::readyRead,
            this, &Client::onReadyRead);

    QByteArray data;
    QDataStream writedStream(&data, QIODevice::WriteOnly);
    writedStream.setByteOrder(kByteOrder);
    writedStream << kProtocolVersion << kReserve << num;

    qint64 bytesSent = m_pSocket->writeDatagram(data, serverAddr, port);

    if (bytesSent == -1) {
        qDebug() << "Error send msg!";
    } else {
        qDebug() << "Msg sent";
    }
}

void Client::onReadyRead() {
    while (m_pSocket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(int(m_pSocket->pendingDatagramSize()));

        qint64 bytesRead = m_pSocket->readDatagram(buffer.data(), buffer.size());
        if (bytesRead == -1) {
            qWarning() << "Error reading datagram:" << m_pSocket->errorString();
            continue;
        }

        buffer.resize(int(bytesRead));
        auto response = QSharedPointer<QByteArray>::create(std::move(buffer));
        emit msgHandler(response);
    }
}
