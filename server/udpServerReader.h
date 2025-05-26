#ifndef UDPSERVERREADER_H
#define UDPSERVERREADER_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

class UdpServerReader : public QObject
{
    Q_OBJECT
public:
    explicit UdpServerReader(QObject *parent = nullptr); // rm default value
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
    QUdpSocket* m_rxSocket;
    // QUdpSocket* m_txSocket;
};

#endif // UDPSERVERREADER_H
