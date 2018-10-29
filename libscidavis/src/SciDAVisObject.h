#ifndef SCIDAVISOBJECT_H
#define SCIDAVISOBJECT_H
/**
   Generic base class for Qt based classes in SciDAVis
**/
template <class Base>
class SciDAVisObject: public Base
{
public:
  template <class... A>
  SciDAVisObject(A... args): Base(std::forward<A>(args)...) {}
  
  /// generic factory function to add a child object, to avoid bare pointers
  template <class T, class... A>
  T& addChild(A... args) {
    T* child=new T(std::forward<A>(args)...);
    child->setParent(this);
    return *child;
  }
};

#endif
