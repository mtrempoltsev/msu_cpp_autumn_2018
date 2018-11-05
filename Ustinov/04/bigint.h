class BigInt {
 public:
    BigInt():
      len(1), sign(1) {
      number = new int[1];
      number[0] = 0;
    }

    BigInt(int64_t x) {
      if (x == 0) {
          len = 1;
          sign = 1;
          number = new int[1];
          number[0] = 0;
      } else {
          int len_ = 0;
          int64_t number_ = x;
          if (x < 0) {
              sign = -1;
              x = -1 * x;
          } else {
              sign = 1;
          }
          while (number_ != 0) {
              len_++;
              number_ = number_ / 10;
          }
          len = len_;
          number = new int[len];
          for (int i=len-1, num; i >= 0; --i) {
              num = x % 10;
              number[i] = num;
              x = x / 10;
          }
        }
    }

    ~BigInt() {
      delete[] number;
    }

    BigInt(const BigInt& copy):
          len(copy.len), sign(copy.sign) {
      number = new int[len];
      for (int i=0; i < len; ++i) {
          number[i] = copy.number[i];
      }
    }

    BigInt& operator=(const BigInt& copy) {
      if (this == &copy)
          return *this;
      int *ptr = new int[copy.len];
      delete[] number;
      len = copy.len;
      sign = copy.sign;
      number = ptr;
      for (int i=0; i < len; ++i) {
          number[i] = copy.number[i];
      }
    }

    BigInt& operator=(int64_t x) {
     int len_ = 0;
     int64_t number_ = x;
     if (x < 0) {
         sign = -1;
         x = -1 * x;
     } else {
         sign = 1;
     }
     while (number_ != 0) {
         len_++;
         number_ = number_ / 10;
     }
     len = len_;
     int *ptr = new int[len];
     delete[] number;
     number = ptr;
     for (int i=len-1, num; i >= 0; --i) {
         num = x % 10;
         number[i] = num;
         x = x / 10;
     }
  }

  BigInt operator+(const BigInt& other) const {
      if (sign + other.sign == 0) {
          if (sign == -1) {
              BigInt x(*this);
              x.sign = 1;
              BigInt y(other);
              BigInt tmp = y - x;
              return tmp;
          } else {
              BigInt x(*this);
              BigInt y(other);
              y.sign = 1;
              BigInt tmp = x - y;
              return tmp;
          }
      }
      int max_len, min_len, sum;
      int *max_num;
      BigInt tmp;
      if (len >= other.len) {
          max_len = len;
          min_len = other.len;
          max_num = number;
      } else {
          max_len = other.len;
          min_len = len;
          max_num = other.number;
      }
      tmp.len = max_len + 1;
      tmp.number = new int[max_len+1];
      tmp.sign = sign;
      for (int i=0; i < tmp.len; ++i) {
          tmp.number[i] = 0;
      }
      for (int i=0; i < min_len; ++i) {
          sum = number[len-1-i] + other.number[other.len-1-i]
                                + tmp.number[max_len-i];
          tmp.number[max_len-i] = sum % 10;
          tmp.number[max_len-1-i] += sum / 10;
      }
      for (int i=min_len; i < max_len; ++i) {
          sum = tmp.number[max_len-i] + max_num[max_len-1-i];
          tmp.number[max_len-i] = sum % 10;
          tmp.number[max_len-1-i] = sum / 10;
      }
      if (tmp.number[0] == 0) {
          int *res = new int[max_len];
          for (int i=max_len; i > 0; --i) {
              res[i-1] = tmp.number[i];
          }
          delete[] tmp.number;
          tmp.number = res;
          tmp.len = max_len;
      }
      return tmp;
    }

    BigInt operator-(const BigInt& other) const {
      if (sign + other.sign == 0) {
          if (sign == -1) {
              BigInt x(*this);
              BigInt y(other);
              y.sign = -1;
              BigInt tmp = x + y;
              return tmp;
          } else {
            BigInt x(*this);
            BigInt y(other);
            y.sign = 1;
            BigInt tmp = x + y;
            return tmp;
          }
      }
      int max_len, min_len, sum, equal = 1;
      int *max_num, *min_num;
      BigInt tmp;
      if (len > other.len) {
          max_len = len;
          min_len = other.len;
          max_num = number;
          min_num = other.number;
          tmp.sign = sign;
      } else if (len < other.len) {
          max_len = other.len;
          min_len = len;
          max_num = other.number;
          min_num = number;
          tmp.sign = -1 * other.sign;
      } else {
          max_len = min_len = len;
          for (int i=0; i < len; ++i) {
              if (number[i] > other.number[i]) {
                max_num = number;
                min_num = other.number;
                tmp.sign = sign;
                equal = 0;
                break;
              } else if (number[i] < other.number[i]) {
                max_num = other.number;
                min_num = number;
                tmp.sign = -1 * other.sign;
                equal = 0;
                break;
              }
          }
          if (equal == 1) {
              tmp.len = 1;
              tmp.sign = 1;
              tmp.number = new int[1];
              tmp.number[0] = 0;
              return tmp;
          }
      }
      tmp.len = max_len;
      tmp.number = new int[max_len];

      for (int i=0; i < tmp.len; ++i) {
          tmp.number[i] = 0;
      }
      for (int i=0; i < min_len; ++i) {
          sum = max_num[max_len-1-i] - min_num[min_len-1-i] + 10;
          tmp.number[max_len-1-i] += sum % 10;
          if (sum / 10 == 0) {
              tmp.number[max_len-2-i] = -1;
          }
          if (tmp.number[max_len-1-i] == -1) {
              tmp.number[max_len-1-i] = 9;
              tmp.number[max_len-2-i] = -1;
          }
      }
      for (int i=min_len; i < max_len; ++i) {
          tmp.number[max_len-1-i] += max_num[max_len-1-i];
          if (tmp.number[max_len-i-1] == -1) {
              tmp.number[max_len-i-1] = 9;
              tmp.number[max_len-i-2] = -1;
          }
      }
      int len_ = tmp.len;
      for (int i=0; i < tmp.len; ++i) {
          if (tmp.number[i] == 0) {
              len_--;
          } else {
              break;
          }
      }
      int *ptr = new int[len_];
      for (int i=0; i < len_; ++i) {
          ptr[len_-1-i] = tmp.number[tmp.len-1-i];
      }
      delete[] tmp.number;
      tmp.len = len_;
      tmp.number = ptr;
      return tmp;
    }

    BigInt operator-() const {
      BigInt tmp(*this);
      tmp.sign = -1;
      return tmp;
    }

    bool operator==(const BigInt& other) const {
      if (this == &other) {
          return true;
      }
      if (len != other.len) {
          return false;
      }
      for (int i=0; i < len; ++i) {
          if (number[i] != other.number[i]) {
              return false;
          }
      }
      return true;
    }

    bool operator!=(const BigInt& other) const {
      return !(*this == other);
    }

    bool operator>(const BigInt& other) const {
      if (this == &other) {
          return false;
      }
      if (sign * len > other.sign * other.len) {
          return true;
      } else if (sign * len < other.sign * other.len) {
          return false;
      }
      if (sign == 1 && other.sign == -1) {
          return true;
      } else if (sign == -1 && other.sign == 1) {
          return false;
      }
      for (int i=0; i < len; ++i) {
          if (number[i] > other.number[i]) {
              if (sign == 1)
                  return true;
              else
                  return false;
          } else if (number[i] < other.number[i]) {
              if (sign == 1)
                  return false;
              else
                  return true;
          }
      }
      return false;
    }

    bool operator<(const BigInt& other) const {
     if (*this == other) {
         return false;
     }
     if (*this > other) {
        return false;
     }
     return true;
    }

    bool operator>=(const BigInt& other) const {
      if (*this == other) {
          return true;
      }
      if (*this > other) {
          return true;
      }
      return false;
    }

    bool operator<=(const BigInt& other) const {
      if (*this == other) {
          return true;
      }
      if (*this < other) {
          return true;
      }
      return false;
    }

    int len;
    int sign;
    int* number;
};

std::ostream& operator<<(std::ostream& out, const BigInt& value) {
    if (value.sign == -1) {
        out << '-';
    }
    for (int i=0; i < value.len; ++i) {
        out << value.number[i];
    }
    return out;
}
