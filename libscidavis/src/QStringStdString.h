#ifndef QSTRINGSTDSTRING_H
#define QSTRINGSTDSTRING_H
/// Deal with conversion between QString and std::string/std::wstring in a generic way

template<class S>
S toString(const QString &x);
template<>
inline std::string toString<std::string>(const QString &x)
{
    return x.toStdString();
}
template<>
inline std::wstring toString<std::wstring>(const QString &x)
{
    return x.toStdWString();
}
template<>
inline std::u16string toString<std::u16string>(const QString &x)
{
    return x.toStdU16String();
}
template<>
inline std::u32string toString<std::u32string>(const QString &x)
{
    return x.toStdU32String();
}

inline QString QStringFromString(const std::string &x)
{
    return QString::fromStdString(x);
}
inline QString QStringFromString(const std::wstring &x)
{
    return QString::fromStdWString(x);
}
inline QString QStringFromString(const std::u16string &x)
{
    return QString::fromStdU16String(x);
}
inline QString QStringFromString(const std::u32string &x)
{
    return QString::fromStdU32String(x);
}
inline QString QStringFromString(const char *x)
{
    return QString::fromStdString(x);
}
inline QString QStringFromString(const wchar_t *x)
{
    return QString::fromStdWString(x);
}

#endif
