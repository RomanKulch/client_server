#include "tools.hpp"
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <constants.hpp>

bool Tools::getFileContent(QByteArray& content) {
    const QStringList arguments = QCoreApplication::arguments();

    if (arguments.size() < kMinNumArguments) {
        qDebug() << "No path to the file";
        return false;
    }

    const QString filePath = arguments[kFilePathIndex];
    // QByteArray content;
    if (!readFile(filePath, content)) {
        qDebug() << "Reading file failed";
        return false;
    }

    return true;
}

bool Tools::readFile(const QString &filePath, QByteArray& content) {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "File doesn't exist: " << filePath;
        return false;
    }

    content = file.readAll();
    file.close();

    return true;
}
