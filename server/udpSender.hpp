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
    void send(QSharedPointer<QNetworkDatagram> pDatagram);

private:
    QUdpSocket* m_txSocket;
};

#endif // UdpSender_H
