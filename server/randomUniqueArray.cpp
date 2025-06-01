#include "randomUniqueArray.hpp"
#include <QDebug>
#include <QRandomGenerator>
#include <QSet>

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
