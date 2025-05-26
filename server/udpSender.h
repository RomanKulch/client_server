#ifndef UdpSender_H
#define UdpSender_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

class UdpSender : public QObject
{
    Q_OBJECT
public:
    explicit UdpSender(QObject *parent = nullptr);

signals:

public slots:
    // void read();
    void write(QNetworkDatagram datagram); // rename send

private:
    // QUdpSocket* m_rxSocket;
    QUdpSocket* mTxSocket;
};

#endif // UdpSender_H
