#ifndef RANDOMUNIQUEARRAY_HPP
#define RANDOMUNIQUEARRAY_HPP

#include <QObject>

class RandomUniqueArray : public QObject
{
    Q_OBJECT
public:
    explicit RandomUniqueArray(QObject *parent = nullptr);
    static QVector<double> generateUniqueValues(const double range, const quint32 count);
signals:
};

#endif // RANDOMUNIQUEARRAY_HPP
