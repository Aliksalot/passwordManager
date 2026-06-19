#pragma once

namespace math {

  template<int Mod>
  class RemRing {
  private:
    int n;
  public:
    RemRing();
    RemRing(int n);

    RemRing& operator+=(const RemRing& other);
    RemRing& operator-=(const RemRing& other);
    bool operator==(const RemRing& other) const;
    bool operator!=(const RemRing& other) const;
    RemRing& operator*=(const RemRing& other);
    RemRing& operator/=(const RemRing& other);

    bool isZeroDivisor() const;
    bool hasInverse() const;

    int raw() const;

    RemRing inverse() const;
  };

  template<int Mod>
  inline RemRing<Mod>::RemRing() {
    n = 0;
  }

  template<int Mod>
  inline RemRing<Mod>::RemRing(int n) {
   n %= Mod;
   if(n < 0) n += Mod;
   this->n = n;
  }

  template<int Mod>
  inline int RemRing<Mod>::raw() const {
    return n;
  }

  template<int Mod>
  inline bool RemRing<Mod>::isZeroDivisor() const {

    int q = Mod, d = n;

    while (d != 0) {
        int remainder = q % d;
        q = d;
        d = remainder;
    }

    return q > 1;
  }

  template<int Mod>
  inline bool RemRing<Mod>::hasInverse() const {
    return !isZeroDivisor() && n != 0;
  }

  template<int Mod>
  inline RemRing<Mod> RemRing<Mod>::inverse() const {
    for(int i = 1; i < Mod; i ++) {
      if((n * i) % Mod == 1) {
        return RemRing(i);
      }
    }
    //TODO custom exception
    throw std::runtime_error("Element has no inverse");
  }

  template<int Mod>
  inline RemRing<Mod>& RemRing<Mod>::operator+=(const RemRing<Mod>& other) {
    n = (n + other.n) % Mod;
    return *this;
  }

  template<int Mod>
  inline RemRing<Mod>& RemRing<Mod>::operator-=(const RemRing<Mod>& other) {
    n = (n - other.n + Mod) % Mod;
    return *this;
  }


  template<int Mod>
  inline RemRing<Mod>& RemRing<Mod>::operator*=(const RemRing<Mod>& other) {
    n = (n * other.n) % Mod;
    return *this;
  }

  template<int Mod>
  inline RemRing<Mod>& RemRing<Mod>::operator/=(const RemRing<Mod>& other) {
    //TODO add custom errors
    if(!other.hasInverse())
      throw std::runtime_error("Divison by zero-divisor");

    *this *= other.inverse();
    return *this;
  }

  template<int Mod>
  inline bool RemRing<Mod>::operator==(const RemRing<Mod>& other) const {
    return n == other.n;
  }

  template<int Mod>
  inline bool RemRing<Mod>::operator!=(const RemRing<Mod>& other) const {
    return !(n == other.n);
  }

  template<int Mod>
  inline RemRing<Mod> operator+(RemRing<Mod> l, const RemRing<Mod>& r) {
    l += r;
    return l;
  }

  template<int Mod>
  inline RemRing<Mod> operator*(RemRing<Mod> l, const RemRing<Mod>& r) {
    l *= r;
    return l;
  }

  template<int Mod>
  inline RemRing<Mod> operator-(const RemRing<Mod>& el) {
    RemRing<Mod> res;
    res -= el;
    return res;
  }

  template<int Mod>
  inline RemRing<Mod> operator-(RemRing<Mod> l, const RemRing<Mod>& r) {
    l -= r;
    return l;
  }


  using Z26 = RemRing<26>;
  using Z96 = RemRing<96>;
}

