#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QRunnable>
#include <QNetworkDatagram>

class RequestHandler : public QObject, public QRunnable {
    Q_OBJECT
public:
    explicit RequestHandler(QNetworkDatagram&& request);

    void run() override;

signals:
    void sendMsg(QSharedPointer<QNetworkDatagram>);
    void finished();

private:
    struct ClientData {
        uint8_t version;
        quint16 port;
        double value;
        QHostAddress addr;
    };

    void deserializeClientRequest(const QByteArray& val);
    void sendResponse();
    void sendError();
    void sendChunkByChunk(const uint8_t msgType, const uint32_t dataSizeBytes, const char* rawData);

    ClientData m_clientData{};
    bool m_isValidRequest;
};

#endif // REQUESTHANDLER_H
