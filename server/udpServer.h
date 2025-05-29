#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QThread>
#include "UdpReceiver.h"
#include "UdpSender.h"
#include "types.hpp"

class UdpServer : public QObject {
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr);

    void init();
    void start();

public slots:
    void dispatcher(QNetworkDatagram incomingData);

signals:
    void sendMsg(QSharedPointer<QNetworkDatagram>);

private:
    struct ClientData {
        uint8_t version;
        quint16 port;
        double value;
        QHostAddress addr;
    };

    void sendError(const ClientData& clientData);
    void sendChunkByChunk(const ClientData& clientData, const uint8_t msgType, const uint32_t dataSizeBytes, const char* rawData);
    bool deserializeClientRequest(const QByteArray& val, ClientData& clientData) const;

    UdpReceiver mReceiver;
    UdpSender mSender;

    QThread mReadThread;
    QThread mWriteThread;
};

#endif // UDPSERVER_H
