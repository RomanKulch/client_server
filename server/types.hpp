#ifndef TYPES_HPP
#define TYPES_HPP

#include <QtTypes>

// struct Header {
//     quint8 total;
//     quint8 majorVersionSize;
//     quint8 minorVersionSize;
// };

using Version_t = quint8;

struct UdpProtocol {
    Version_t majorVersion;
    Version_t minorVersion;
    Version_t version;
    quint32 numElements;
    // Header header;
};

#endif // TYPES_HPP
