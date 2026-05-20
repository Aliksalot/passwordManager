#pragma once

#include<cstddef>
#include<stdexcept>

namespace clib {
  template<typename T>
  class List {

    public:
      List() {}

      ~List() {
        if(data) {
          ::operator delete data;
        }
      }

      void add(const T& item) {
        if(full()) expand();

        data[count ++] = item;
      }

      //copy
      List(const List& l) {
        data = new T[l.cap];
        cap = l.cap;
        count = l.count;
        for(size_t i = 0; i < l.count; i ++) {
          data[i] = l.data[i];
        }
      }
      List& operator=(const List& l) {
        if(&l == this) return *this;

        if(data) ::operator delete data;

        data = new T[l.cap];
        cap = l.cap;
        count = l.count;

        for(size_t i = 0; i < l.count; i ++) {
          data[i] = std::move_if_noexcept(l.data[i]);
        }
        return *this;
      }
      //steal
      List(List&& l) noexcept{
        data = l.data;
        count = l.count;
        cap = l.cap;
        l.data = nullptr;
        l.cap = 0;
        l.count = 0;
      }

      List& operator=(List&& l) noexcept{
        if(this == &l) return *this;

        ::operator delete data;

        data = l.data;
        count = l.count;
        cap = l.cap;
        l.data = nullptr;
        l.cap = 0;
        l.count = 0;

        return *this;
      }

      T& operator[](std::size_t index) {
        if(indexInBounds(index)) {
            return data[index];
        }
        throw std::out_of_range("index out of bounds");
      }

      const T& operator[](std::size_t index) const{
        if(indexInBounds(index)) {
            return data[index];
        }
        throw std::out_of_range("index out of bounds");
      }

      void extend(const List<T>& list) {
        reserve(count + list.size());
        for(size_t i = 0; i < list.size(); i ++){
          data[count ++] = list[i];
        }
      };

      //could fail
      bool contains(const T& item) const {
        for(size_t i = 0; i < count; i ++) {
          if(data[i] == item) return true;
        }
        return false;
      }

      std::size_t size() const{
        return count;
      }

    private:
      T* data = nullptr;
      std::size_t count = 0;
      std::size_t cap = 0;

      void expand();

      void reserve(std::size_t space);

      bool indexInBounds(std::size_t index) const {
        return index < count;
      }

      bool full() const {
        return cap == count;
      }
      bool empty() const {
        return count == 0;
      }
  };

  template<typename T>
  void List<T>::reserve(std::size_t space) {

    if(space <= cap) return;


    T* temp = static_cast<T*>(::operator new((sizeof(T) * space)));

    for(size_t i = 0; i < count; i ++) {
      new (temp + i) T(std::move_if_noexcept(data[i]));
    }

    for(size_t i = 0; i < count; i ++) {
      data[i].~T();
    }

    ::operator delete data;
    data = temp;
    cap = space;
  }

  template<typename T>
  void List<T>::expand() {
    reserve(cap == 0 ? 1 : cap * 2);
  }

}
