#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QObject>
#include <QUdpSocket>
#include <QThread>
#include <memory>
#include <responseHandler.hpp>
// #include <QHostAddress>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

public slots:
    void start(QString addr, quint16 port, double num);
    void onReadyRead();
signals:

    void msgHandler(QSharedPointer<QByteArray> msg);
private:
    std::unique_ptr<QUdpSocket> m_pSocket;
    QThread mHandlerThread;
    ResponseHandler mRespHandler;
    // QUdpSocket mUdpSocket;
};

#endif // CLIENT_HPP
