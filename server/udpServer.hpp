#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QThread>
#include <QThreadPool>
#include "udpReceiver.hpp"
#include "udpSender.hpp"

class UdpServer : public QObject {
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr);

    void init();
    void start();

public slots:
    void dispatcher(QNetworkDatagram incomingData);

private:
    UdpReceiver m_receiver;
    UdpSender m_sender;

    QThread m_readThread;
    QThread m_writeThread;
    QThreadPool* m_pThreadPool;
};

#endif // UDPSERVER_H
