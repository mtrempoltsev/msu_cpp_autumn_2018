#include <iostream>
#include <string>


enum class Error {
  NoError,
  CorruptedArchive
};

class Serializer {
  std::ostream& out;
  static constexpr char Separator = ' ';

 public:
  explicit Serializer(std::ostream& out_)
      : out(out_) {}

  template <class T>
  Error save(T& obj) {
      return obj.serialize(*this);
  }

  template <class... ArgsT>
  Error operator()(ArgsT... args) {
      return process(args...);
  }

 private:
  template<class T, class... ArgsT>
  Error process(T val, ArgsT... args) {
      if (process(val) == Error::NoError)
          return process(std::forward<ArgsT>(args)...);
      else
          return Error::CorruptedArchive;
  }

  Error process(bool val) {
      if (val)
          out << "true" << Separator;
      else
          out << "false" << Separator;
      return Error::NoError;
  }

  Error process(uint64_t val) {
        out << val << Separator;
        return Error::NoError;
  }

  template <class T>
  Error process(T val) {
      return Error::CorruptedArchive;
  }
};

class Deserializer {
  std::istream& in;
  static constexpr char Separator = ' ';

 public:
  explicit Deserializer(std::istream& in_)
        : in(in_) {}

  template <class T>
  Error load(T& obj) {
      return obj.serialize(*this);
  }

  template <class... ArgsT>
  Error operator()(ArgsT&... args) {
      return process(args...);
  }

 private:
  template<class T, class... ArgsT>
  Error process(T& val, ArgsT&... args) {
      if (process(val) == Error::NoError)
          return process(args...);
      else
          return Error::CorruptedArchive;
  }

  Error process(bool& val) {
      std::string str;
      in >> str;
      if (str == "true")
          val = true;
      else if (str == "false")
          val = false;
      else
          return Error::CorruptedArchive;
      return Error::NoError;
  }

  Error process(uint64_t& val) {
      std::string str;
      in >> str;
      if (str.c_str()[0] == '-') {
          return Error::CorruptedArchive;
      }
      try {
          val = std::stoull(str.c_str());
      } catch (std::invalid_argument) {
          return Error::CorruptedArchive;
      }
      return Error::NoError;
  }

  template <class T>
  Error process(T& val) {
      return Error::CorruptedArchive;
  }
};
