#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QThread>
#include "UdpReceiver.h"
#include "UdpSender.h"

class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr);

    void init();
    void start();

signals:

private:
    UdpReceiver mReceiver; // UdpReceiver
    UdpSender mSender; // UdpSender

    QThread mReadThread;
    QThread mWriteThread;
};

#endif // UDPSERVER_H
