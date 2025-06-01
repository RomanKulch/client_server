#ifndef UdpReceiver_H
#define UdpReceiver_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

class UdpReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UdpReceiver(QObject *parent = nullptr);

public slots:
    void start(quint16 listenPort);
    void read();

signals:
    void handleMsg(QNetworkDatagram datagram);

private:
    QUdpSocket* m_rxSocket;
};

#endif // UdpReceiver_H
