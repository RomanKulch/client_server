#ifndef RESPONSEHANDLER_H
#define RESPONSEHANDLER_H

#include <QObject>
#include <QSharedPointer>
#include <QByteArray>
#include "constants.hpp"

class ResponseHandler : public QObject
{
    Q_OBJECT
public:
    explicit ResponseHandler(QObject *parent = nullptr);

public slots:
    void responseHandler(QSharedPointer<QByteArray> msg);

private:
    bool deserializeHeader(const QByteArray& data, ResponseHeader& header) const;
    void processError(const QByteArray& data, ResponseHeader& header) const;
    void processValue(const QByteArray& data, ResponseHeader& header);
    void saveToBinaryFile(const QString& filename); // const??
    size_t getPayloadSize(const QByteArray& data) const;
    const char* getPayloadStart(const QByteArray& data) const;
    bool validateHashSum(const QByteArray& data, uint32_t expectedHash) const;

    QVector<double> m_value{};
    bool m_isReserveMemory{false};
};

#endif // RESPONSEHANDLER_H
