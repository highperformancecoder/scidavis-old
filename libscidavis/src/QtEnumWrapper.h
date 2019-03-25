#ifndef QTENUMWRAPPER_H
#define QTENUMWRAPPER_H
/// wrapper class allowing conversion Qt:: enum to QtEnum, and expose to Python
// in own header file to prevent classdesc fromprocessing it

template <class E, class QE>
struct QtEnumWrapper
{
  E value;
  QtEnumWrapper() {}
  QtEnumWrapper(E v): value(E(v)) {}
  QtEnumWrapper(QE v): value(E(v)) {}
  operator QE() const {return QE(value);}
  operator E() const {return value;}
  operator int() const {return int(value);}
  bool operator==(QE e) const {return int(e)==int(value);}
};
#endif
