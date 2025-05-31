#include "udpServer.hpp"
#include <QDebug>
#include <QDataStream>
#include <QIODevice>
#include "constants.hpp"
#include "randomUniqueArray.hpp"
#include "requestHandler.hpp"

UdpServer::UdpServer(QObject *parent)
    : QObject{parent}
    , m_pThreadPool(new QThreadPool(this)){} // make ptrs for others

void UdpServer::init() {
    m_receiver.moveToThread(&m_readThread);
    m_sender.moveToThread(&m_writeThread);
    m_pThreadPool->setMaxThreadCount(QThread::idealThreadCount());

    QObject::connect(&m_readThread, &QThread::started, [this]() {
        constexpr quint16 port = 12347;
        QMetaObject::invokeMethod(&m_receiver, "start", Q_ARG(quint16, port));
    });

    QObject::connect(&m_readThread, &QThread::finished, &m_receiver, &QObject::deleteLater);
    QObject::connect(&m_writeThread, &QThread::finished, &m_sender, &QObject::deleteLater);

    QObject::connect(&m_receiver, &UdpReceiver::handleMsg,
                     this, &UdpServer::dispatcher,
                     Qt::QueuedConnection);
}

void UdpServer::start() {
    m_readThread.start();
    m_writeThread.start();

}

void UdpServer::dispatcher(QNetworkDatagram request) {
    RequestHandler *handler = new RequestHandler(std::move(request));

    QObject::connect(handler, &RequestHandler::sendMsg,
                     &m_sender, &UdpSender::send,
                     Qt::QueuedConnection);

    QObject::connect(handler, &RequestHandler::finished,
                     handler, &RequestHandler::deleteLater,
                     Qt::QueuedConnection);

    m_pThreadPool->start(handler);
}

