#include <vector>
#include <string.h>

class BigInt {
public:
  int sign = 0;
  int* data = nullptr;
  int size = 0;

  BigInt () {
    if (data)
      delete [] data;
    data = new int [1];
    sign = 0;
    size = 0;
    data[size] = 0;
    ++size;
  }

  ~BigInt () {
    if (data)
      delete [] data;
  }

  static int get_len (int64_t n) {
    if (n == 0)
      return 1;
    int i = 0;
    for (; n > 0; n /= 10, ++i);
    return i;
  }

  BigInt (int64_t value) {
    if (data)
      delete [] data;
    size = 0;
    if (value == 0) {
      data = new int [1];
      sign = 0;
      size = 0;
      data[size] = 0;
      ++size;
    }
    else {
      if (value > 0)
        sign = 1;
      if (value < 0)
        sign = -1;
      value *= sign;
      data = new int [get_len (value)];
      while (value > 0) {
        data[size] = value % 10;
        ++size;
        value /= 10;
      }
    }
  }

  BigInt (const BigInt& rvalue) {
    data = new int [rvalue.size];
    size = rvalue.size;
    for (int i = 0; i < size; ++i)
      data[i] = rvalue.data[i];
    sign = rvalue.sign;
  }

  BigInt& operator= (const BigInt& rvalue) {
    if (data)
      delete [] data;
    size = 0;
    data = new int [rvalue.size];
    size = rvalue.size;
    for (int i = 0; i < size; ++i)
      data[i] = rvalue.data[i];
    sign = rvalue.sign;
  }

  BigInt& operator= (int64_t rvalue) {
    if (data)
      delete [] data;
    size = 0;
    if (rvalue == 0) {
      data = new int [1];
      sign = 0;
      size = 0;
      data[size] = 0;
      ++size;
    }
    else {
      if (rvalue > 0)
        sign = 1;
      if (rvalue < 0)
        sign = -1;
      rvalue *= sign;
      data = new int [get_len (rvalue)];
      while (rvalue > 0) {
        data[size] = rvalue % 10;
        ++size;
        rvalue /= 10;
      }
    }
    return *this;
  }

  static bool compare (const int* left, const int* right, int size) {
    int i = size - 1;
    for (; i > 0 && left[i] == right[i]; --i);
    if (left[i] < right[i])
      return true;
    return false;
  }

  static bool compare_eq (const int* left, const int* right, int size) {
    for (int i = 0; i < size; ++i)
      if (left[i] != right[i])
        return false;
    return true;
  }

  bool operator< (const BigInt& rvalue) const {
    if (sign < rvalue.sign)
      return true;
    if (sign > rvalue.sign)
      return false;

    if (sign > 0) {
      if (size  < rvalue.size)
        return true;
      if (size > rvalue.size)
        return false;
      return compare (data ,rvalue.data, size);
    }
    else {
      if (size < rvalue.size)
        return false;
      if (size > rvalue.size)
        return true;
      return !compare (data ,rvalue.data, size);
    }
  }

  bool operator== (const BigInt& rvalue) const {
    if (sign != rvalue.sign || size != rvalue.size)
      return false;
    return compare_eq (data, rvalue.data, size);
  }

  bool operator!= (const BigInt& rvalue) const {
    return !(*this == rvalue);
  }

  bool operator> (const BigInt& rvalue) const {
    return !(*this < rvalue) && !(*this == rvalue);
  }

  bool operator<= (const BigInt& rvalue) const {
    return !(*this > rvalue);
  }

  bool operator>= (const BigInt& rvalue) const {
    return !(*this < rvalue);
  }

  BigInt operator+ (const BigInt& rvalue) const {
    if (sign != rvalue.sign) {
      if (sign > 0) {
        return *this - (-rvalue);
      }
      else {
        return rvalue - (-*this);
      }
    }
    BigInt tmp;
    delete [] tmp.data;
    tmp.sign = sign;
    int lenl = size;
    int lenr = rvalue.size;
    tmp.data = new int[lenl > lenr ? lenl + 1 : lenr + 1];
    tmp.size = 0;
    int l = 0, r = 0;
    int residual = 0;
    for (; l < lenl && r < lenr; ++l, ++r) {
      int step = data[l] + rvalue.data[r] + residual;
      tmp.data[tmp.size] = step % 10;
      ++tmp.size;
      residual = step / 10;
    }
    for (; l < lenl; ++l) {
      int step = data[l] + residual;
      tmp.data[tmp.size] = step % 10;
      ++tmp.size;
      residual = step / 10;
    }
    for (; r < lenr; ++r) {
      int step = rvalue.data[r] + residual;
      tmp.data[tmp.size] = step % 10;
      ++tmp.size;
      residual = step / 10;
    }
    if (residual > 0) {
      tmp.data[tmp.size] = residual;
      ++tmp.size;
    }
    return tmp;
  }

  BigInt operator- (const BigInt& rvalue) const {
    if (*this == rvalue)
      return BigInt ();

    if (size == 1 && data[0] == 0)
      return BigInt (-rvalue);

    if (rvalue.size == 1 && rvalue.data[0] == 0)
      return BigInt (*this);

    if (sign > 0) {
      if (rvalue.sign > 0) {
        if (rvalue > *this)
          return -(rvalue - *this);
      }
      else {
        return *this + (-rvalue);
      }
    }
    else {
      if (rvalue.sign > 0) {
        return -(rvalue + (-*this));
      }
      else {
        return -((-*this) - (-rvalue));
      }
    }
    BigInt tmp;
    delete [] tmp.data;
    tmp.sign = sign;
    int lenl = size;
    int lenr = rvalue.size;
    tmp.data = new int[lenl > lenr ? lenl : lenr];
    tmp.size = 0;
    int *buf = new int [size];
    memcpy (buf, data, size * sizeof (int));
    int l = 0, r = 0;
    for (; l < lenl && r < lenr; ++l, ++r) {
      int step = buf[l] - rvalue.data[r];
      if (step < 0) {
        int l_tmp = l + 1;
        for (; buf[l_tmp] == 0; ++l_tmp);
        --buf[l_tmp];
        --l_tmp;
        for (; l_tmp > l; --l_tmp)
          buf[l_tmp] = 9;
        step += 10;
      }
      tmp.data[tmp.size] = step;
      ++tmp.size;
    }
    for (; l < lenl; ++l) {
      tmp.data[tmp.size] = buf[l];
      ++tmp.size;
    }
    while (tmp.data[tmp.size - 1] == 0)
      --tmp.size;
    delete [] buf;
    return tmp;
  }

  BigInt operator- () const {
    BigInt tmp (*this);
    tmp.sign = sign * -1;
    return tmp;
  }


};

std::ostream& operator<<(std::ostream& out, const BigInt& rvalue) {
  if (rvalue.sign == -1) {
      out << '-';
  }
  for (int i = rvalue.size - 1; i >= 0; --i) {
      out << rvalue.data[i];
  }
  return out;
}
