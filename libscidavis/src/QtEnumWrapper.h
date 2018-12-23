#ifndef QTENUMWRAPPER_H
#define QTENUMWRAPPER_H
/// wrapper class allowing conversion Qt:: enum to QtEnum, and expose to Python
// in own header file to prevent classdesc fromprocessing it

template <class E, class QE>
struct QtEnumWrapper
{
  E value;
  QtEnumWrapper() {}
  QtEnumWrapper(QE v): value(E(v)) {}
  operator QE() {return QE(value);}
};
#endif
