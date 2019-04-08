#ifndef SCIDAVISOBJECT_H
#define SCIDAVISOBJECT_H
#include <QWidget>

/**
   Generic base class for Qt based classes in SciDAVis
**/
template <class Base>
class SciDAVisObject: public Base
{
  void m_setParent(QObject* child, QObject* parent)
  {child->setParent(parent);}
  // overload that preserves the default window flags
  void m_setParent(QWidget* child, QWidget* parent)
  {child->setParent(parent,child->windowFlags());}
  
public:
  template <class... A>
  SciDAVisObject(A... args): Base(std::forward<A>(args)...) {}

  // QObjects are not copiable, but because the copy constructor has
  // probably hidden in a private section rather than deleted,
  // explicitly delete this here so that the python descriptor can
  // handle noncopiable objects correctly.
  SciDAVisObject(const SciDAVisObject&)=delete;
  
  /// generic factory function to add a child object, to avoid bare pointers
  template <class T, class... A>
  T& addChild(A... args) {
    T* child=new T(std::forward<A>(args)...);
    m_setParent(child,this);
    return *child;
  }
};

// thrown in the event an object reference cannot be returned
struct NoSuchObject: public std::exception
{
  const char* what() const noexcept override
  {return qPrintable(QObject::tr("No such object"));}
};

#endif
