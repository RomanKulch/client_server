#ifndef UdpReceiver_H
#define UdpReceiver_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

class UdpReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UdpReceiver(QObject *parent = nullptr); // rm default value
    // void init(quint16 listenPort);
    // void start(quint16 listenPort = 12345);

public slots:
    void start(quint16 listenPort);
    // private slots
    void read();
    // void write();

signals:
    void handleMsg(QNetworkDatagram datagram);

private:
    QUdpSocket* mRxSocket;
};

#endif // UdpReceiver_H
