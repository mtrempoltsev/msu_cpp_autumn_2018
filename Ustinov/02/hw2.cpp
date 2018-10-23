#include<iostream>
#include<string>
#include<sstream>
#include<exception>


int error() {
  std::cout << "error" << '\n';
  return 1;
}


class Calculator {
 public:
  int64_t Parse(const char* str) {
      std::istringstream expr(str);
      return ParseAddSub(expr);
  }

 private:
  struct Operations {
      static const char Add = '+';
      static const char Sub = '-';
      static const char Mul = '*';
      static const char Div = '/';
      static const char Minus = '-';
  };

  int64_t ParseAddSub(std::istringstream& expr) {
      int64_t left = ParseMulDiv(expr);
      char oper;
      while (expr >> oper
            && (oper == Operations::Add
            || oper == Operations::Sub)) {
          int64_t right = ParseMulDiv(expr);
          if (oper == Operations::Add) {
              left += right;
          } else {
              left -= right;
          }
      }

      return left;
  }

  int64_t ParseMulDiv(std::istringstream& expr) {
      int64_t left = ParseNum(expr);
      char oper;
      while (expr >> oper
            && (oper == Operations::Mul
            || oper == Operations::Div)) {
          int64_t right = ParseNum(expr);
          if (oper == Operations::Mul) {
              left *= right;
          } else {
              if (right == 0) {
                  throw std::logic_error("Divivsion by zero");
              }
              left /= right;
          }
      }
      if (oper != Operations::Mul
        || oper != Operations::Div)   expr.putback(oper);
      return left;
  }

  int64_t ParseNum(std::istringstream& expr) {
      char num;
      int sign = 1;
      expr >> num;
      if (num == Operations::Minus) {
          sign = -1;
          expr >> num;
      }
      if (num < '0' || num > '9') {
          throw std::logic_error("Must be int");
      }

      int64_t x = num - 48;
      return x * sign;
  }
};


int main(int argc, char* argv[]) {
  if (argc == 2) {
      Calculator calc;
      try {
          std::cout << calc.Parse(argv[1]) << '\n';
      } catch (std::exception& e) {
          return error();
      }
  }
  else  return error();

  return 0;
}
