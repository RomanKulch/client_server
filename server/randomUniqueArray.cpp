#include "randomUniqueArray.hpp"
#include <QDebug>
#include <QDataStream>
#include <QIODevice>
#include <QRandomGenerator>
#include <QSet>

RandomUniqueArray::RandomUniqueArray(QObject *parent)
    : QObject{parent}
{}

QVector<double> RandomUniqueArray::generateUniqueValues(const double range, const quint32 count) {
    QSet<double> seen;
    QVector<double> result;
    result.reserve(count);

    while (result.size() < count) {
        double num = QRandomGenerator::global()->generateDouble() * (2 * range) - range;
        if (!seen.contains(num)) {
            seen.insert(num);
            result.append(num);
        }
    }

    return result;
}
