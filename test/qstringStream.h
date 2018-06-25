#ifndef QSTRINGSTREAM_H
#define QSTRINGSTREAM_H
#include <iostream>
#include <QString>

inline std::ostream& operator<<(std::ostream& o, const QString& s)
{return o<<s.toStdString();}

extern unsigned numInfos, numWarnings;

#endif
