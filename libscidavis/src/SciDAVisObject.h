#ifndef SCIDAVISOBJECT_H
#define SCIDAVISOBJECT_H
#include <python_base.h>

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
  
  /// generic factory function to add a child object, to avoid bare pointers
  template <class T, class... A>
  T& addChild(A... args) {
    T* child=new T(std::forward<A>(args)...);
    m_setParent(child,this);
    return *child;
  }
};

namespace classdesc_access
{
  template <class B>
  struct access_python<SciDAVisObject<B>>:
    public classdesc::NullDescriptor<classdesc::python_t> {};
}

#endif
