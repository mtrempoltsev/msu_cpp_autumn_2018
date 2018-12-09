#pragma once

enum class Error {
    NoError,
    CorruptedArchive,
};

class Serializer {
  static constexpr char Separator = ' ';
  std::ostream& out_;
public:
  explicit Serializer (std::ostream& out): out_(out) {}

  template <class T>
  Error save (T& object) {
    return object.serialize (*this);
  }

  template <class... ArgsT>
  Error operator() (ArgsT... args) {
    return process (args...);
  }

private:
  template <class T, class... ArgsT>
  Error process (T value, ArgsT ... args) {
    process (value);
    process (std::forward<ArgsT> (args)...);
    return Error::NoError;
  }

  Error process (uint64_t value) {
    out_ << value << Separator;
    return Error::NoError;
  }

  Error process (bool value) {
    if (value == true)
      out_ << "true" << Separator;
    else
      out_ << "false" << Separator;
    return Error::NoError;
  }
};

class Deserializer {
  std::istream& in_;
public:
  explicit Deserializer (std::istream& in): in_(in) {}

  template <class T>
  Error load (T& object) {
    return object.serialize (*this);
  }

  template <class... ArgsT>
  Error operator() (ArgsT&... args) {
    return process (args...);
  }

private:
  template <class T, class... ArgsT>
  Error process (T& value, ArgsT& ... args) {
    auto ret = process (value);
    if (ret == Error::NoError)
      return process (args...);
    return Error::CorruptedArchive;
  }

  Error process (bool& value) {
    std::string text;
    in_ >> text;
    if (text == "true")
      value = true;
    else if (text == "false")
      value = false;
    else
      return Error::CorruptedArchive;

    return Error::NoError;
  }

  Error process (uint64_t& value) {
    std::string text;
    in_ >> text;
    if (text[0] == '-')
      return Error::CorruptedArchive;
    try {
      value = std::stoull (text);
    }
    catch (const std::logic_error& error) {
      return Error::CorruptedArchive;
    }

    return Error::NoError;
  }
};
