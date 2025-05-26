#ifndef UDPSERVERWRITER_H
#define UDPSERVERWRITER_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

class UdpServerWriter : public QObject
{
    Q_OBJECT
public:
    explicit UdpServerWriter(QObject *parent = nullptr);

signals:

public slots:
    // void read();
    void write(QNetworkDatagram datagram); // rename send

private:
    // QUdpSocket* m_rxSocket;
    QUdpSocket* m_txSocket;
};

#endif // UDPSERVERWRITER_H
