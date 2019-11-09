inline py::dict modDict(const char* mod) {
  using namespace py;
  return extract<dict>(import(mod).attr("__dict__"))();
}
