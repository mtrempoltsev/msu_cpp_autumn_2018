#include <vector>

class BigInt {
public:
  int sign = 0;
  std::vector<int> data;

  BigInt () {
    data.clear ();
    sign = 0;
    data.push_back (0);
  }

  BigInt (int64_t value) {
    data.clear ();
    if (value == 0) {
      sign = 0;
      data.push_back (value);
    }
    else {
      if (value > 0)
        sign = 1;
      if (value < 0)
        sign = -1;
      value *= sign;
      while (value > 0) {
        data.push_back (value % 10);
        value /= 10;
      }
    }
  }

  BigInt (const BigInt& rvalue) = default;
  BigInt& operator= (const BigInt& rvalue) = default;
  BigInt& operator= (int64_t rvalue) {
    data.clear ();
    if (rvalue == 0) {
      sign = 0;
      data.push_back (rvalue);
    }
    else {
      if (rvalue > 0)
        sign = 1;
      if (rvalue < 0)
        sign = -1;
      rvalue *= sign;
      while (rvalue > 0) {
        data.push_back (rvalue % 10);
        rvalue /= 10;
      }
    }
    return *this;
  }

  static bool compare (const std::vector<int>& left, const std::vector<int>& right) {
    int i = left.size () - 1;
    for (; i > 0 && left[i] == right[i]; --i);
    if (left[i] < right[i])
      return true;
    return false;
  }

  bool operator< (const BigInt& rvalue) const {
    if (sign < rvalue.sign)
      return true;
    if (sign > rvalue.sign)
      return false;

    if (sign > 0) {
      if (data.size () < rvalue.data.size ())
        return true;
      if (data.size () > rvalue.data.size ())
        return false;
      return compare (data ,rvalue.data);
    }
    else {
      if (data.size () < rvalue.data.size ())
        return false;
      if (data.size () > rvalue.data.size ())
        return true;
      return !compare (data ,rvalue.data);
    }
  }

  bool operator== (const BigInt& rvalue) const {
    if (sign != rvalue.sign || data.size () != rvalue.data.size ())
      return false;
    return data == rvalue.data;
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
    tmp.data.clear ();
    tmp.sign = sign;
    int lenl = data.size ();
    int lenr = rvalue.data.size ();
    int l = 0, r = 0;
    int residual = 0;
    for (; l < lenl && r < lenr; ++l, ++r) {
      int step = data[l] + rvalue.data[r] + residual;
      tmp.data.push_back (step % 10);
      residual = step / 10;
    }
    for (; l < lenl; ++l) {
      int step = data[l] + residual;
      tmp.data.push_back (step % 10);
      residual = step / 10;
    }
    for (; r < lenr; ++r) {
      int step = rvalue.data[r] + residual;
      tmp.data.push_back (step % 10);
      residual = step / 10;
    }
    if (residual > 0)
      tmp.data.push_back (residual);
    return tmp;
  }

  BigInt operator- (const BigInt& rvalue) const {
    if (*this == rvalue)
      return BigInt ();

    if (data.size () == 1 && data[0] == 0)
      return BigInt (-rvalue);

    if (rvalue.data.size () == 1 && rvalue.data[0] == 0)
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
    tmp.data.clear ();
    auto buf = data;
    tmp.sign = sign;
    int lenl = data.size ();
    int lenr = rvalue.data.size ();
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
      tmp.data.push_back (step);
    }
    for (; l < lenl; ++l)
      tmp.data.push_back (buf[l]);
    while (tmp.data[tmp.data.size () - 1] == 0)
      tmp.data.pop_back ();
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
  for (int i = rvalue.data.size () - 1; i >= 0; --i) {
      out << rvalue.data[i];
  }
  return out;
}
