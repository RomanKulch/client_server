#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include "udpServerReader.h"
#include "udpServerWriter.h"

class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr);

signals:
};

#endif // UDPSERVER_H
