#ifndef XSERVER_LOGGER_HPP
#define XSERVER_LOGGER_HPP

#include <iostream>

using std::cout, std::cerr;
using OutStream = std::ostream;

class BaseLogger {
public:
  template<typename T>
  OutStream &operator<<(T msg);

  explicit operator OutStream &();

protected:
  BaseLogger(OutStream &stream, const char *type_str);

  ~BaseLogger();

private:
  OutStream &stream_;

  const char *const type_str_;
};

class [[maybe_unused]] ErrLogger : public BaseLogger {
public:
  ErrLogger();
};

class [[maybe_unused]] InfoLogger : public BaseLogger {
public:
  InfoLogger();
};

#ifndef NDEBUG

class [[maybe_unused]] DbgLogger : public BaseLogger {
public:
  DbgLogger();
};

#endif

#endif //XSERVER_LOGGER_HPP
