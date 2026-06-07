#pragma once

namespace math {

  class Z26 {
  private:
    int n;
  public:
    Z26();
    Z26(int n);

    Z26& operator+=(const Z26& other);
    Z26& operator-=(const Z26& other);
    bool operator==(const Z26& other);
    Z26& operator*=(const Z26& other);
    Z26& operator/=(const Z26& other);

    bool isZeroDivisor() const;
    bool hasInverse() const;

    int raw() const;

    Z26 inverse() const;
     static clib::List<math::Z26> toZ26(const clib::String& text);
     static clib::String fromZ26(const clib::List<math::Z26> tokens);
  };

  inline Z26::Z26() {
    n = 0;
  }
  inline Z26::Z26(int n) {
   n %= 26;
   if(n < 0) n += 26;
   this->n = n;
  }

  inline int Z26::raw() const {
    return n;
  }

  inline bool Z26::isZeroDivisor() const {
    return (n != 0) && (n % 2 == 0 || n == 13);
  }

  inline bool Z26::hasInverse() const {
    return !isZeroDivisor() && n != 0;
  }

  inline Z26 Z26::inverse() const {
    for(int i = 1; i < 26; i ++) {
      if((n * i) % 26 == 1) {
        return Z26(i);
      }
    }
    //TODO custom exception
    throw std::runtime_error("Element has no inverse");
  }
  inline Z26& Z26::operator+=(const Z26& other) {
    n = (n + other.n) % 26;
    return *this;
  }

  inline Z26& Z26::operator-=(const Z26& other) {
    n = (n - other.n + 26) % 26;
    return *this;
  }

  inline Z26& Z26::operator*=(const Z26& other) {
    n = (n * other.n) % 26;
    return *this;
  }

  inline Z26& Z26::operator/=(const Z26& other) {
    //TODO add custom errors
    if(!other.hasInverse())
      throw std::runtime_error("Divison by zero-divisor");

    *this *= other.inverse();
    return *this;
  }

  inline bool Z26::operator==(const Z26& other) {
    return n == other.n;
  }

  inline Z26 operator+(Z26 l, const Z26& r) {
    l += r;
    return l;
  }

  inline Z26 operator*(Z26 l, const Z26& r) {
    l *= r;
    return l;
  }

  inline Z26 operator-(const Z26& el) {
    Z26 res;
    res -= el;
    return res;
  }

  inline Z26 operator-(Z26 l, const Z26& r) {
    l -= r;
    return l;
  }

  inline clib::List<math::Z26> Z26::toZ26(const clib::String& text) {
    clib::List<math::Z26> res;
    for(std::size_t i = 0; i < text.size(); i ++) {
      char c = text[i];
      if(c < 'a' || c > 'z')
        throw std::runtime_error("HillCipher alphabet consists of only a-z lowercase!");
      res.add(math::Z26(signed(c - 'a')));
    }
    return res;
  }

  inline clib::String Z26::fromZ26(const clib::List<math::Z26> tokens) {
    clib::String res;
    for(auto& code: tokens) {
      res += char(code.raw() + 'a');
    }
    return res;
  }

}
