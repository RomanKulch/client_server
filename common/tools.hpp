#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <QString>
#include <QByteArray>

class Tools {
public:
    static bool getFileContent(QByteArray& content);
    static bool readFile(const QString &filePath, QByteArray& content);
};

#endif // TOOLS_HPP
