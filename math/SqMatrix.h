#pragma once

#include"../utils/darray.h"
#include<iostream>
#include"./RemRing.h"
#include"../utils/exceptions.h"

namespace math {

  /**
   * Not a very abstract class, but such isn't needed for this project
   * Arithmetic is defined as arithmetic in Z/26Z
   * Which isn't explicitly defined in the class/function name/names,
   * so that they stay shorter and easier to type out
   */
  using Datadarray = clib::darray<clib::darray<Z26>>;

  class SqMatrix {
  private:
    void calculateInverse();

    SqMatrix(Datadarray& data, Datadarray& inverse);

    Datadarray data;
    Datadarray inverse;

    static clib::Text datadarrayToReadable(const Datadarray& data);
    static clib::darray<Z26> applyDatadarray(
      const Datadarray& data,
      const clib::darray<Z26>& vec
    );

  public:
    SqMatrix() = delete;
    SqMatrix(const Datadarray& data);

    const Datadarray& asdarray() const;
    const Datadarray& inverseAsdarray() const;

    clib::Text toReadableText() const;
    clib::Text toReadableTextInverse() const;

    clib::Text serialize() const;

    clib::darray<Z26> apply(const clib::darray<Z26>& vec) const;
    clib::darray<Z26> applyInverse(const clib::darray<Z26>& vec) const;

    static Datadarray identity(unsigned dim);
    static SqMatrix deserialize(const clib::Text& raw);
  };


  inline SqMatrix::SqMatrix(Datadarray& data, Datadarray& inverse)
    : 
    data(
      static_cast<Datadarray&&>(data)
    ),
    inverse(
      static_cast<Datadarray&&>(inverse)
    ) { };

  inline clib::Text SqMatrix::toReadableText() const {
    return datadarrayToReadable(data);
  }

  inline clib::Text SqMatrix::serialize() const {
    clib::Text result;

    result += clib::Text::fromInt(data.size());
    result += "&";

    for(auto& row: data) {
      for(auto& item: row) {
        result += clib::Text::fromInt(item.raw());
        result += "&";
      }
    }
    for(auto& row: inverse) {
      for(auto& item: row) {
        result += clib::Text::fromInt(item.raw());
        result += "&";
      }
    }

    return result;
  }

  inline SqMatrix SqMatrix::deserialize(const clib::Text& raw) {
    clib::darray<clib::Text> tokens = raw.split('&');
    if(tokens.empty())
      throw std::runtime_error("Serialized matrix format doesn't match expected!");

    //Serializer appends extra &
    tokens.pop();

    Datadarray matrix, inverse;
    std::size_t dim = tokens[0].toInt();

    std::size_t expectedTokenCount = 1 + 2*dim*dim;
    if(tokens.size() != expectedTokenCount) {
      throw std::runtime_error("Serialized matrix format doesn't match expected!");
    }
    
    try{
      for(std::size_t r = 0; r < dim; r ++) {
        matrix.add(clib::darray<Z26>());
        inverse.add(clib::darray<Z26>());
        for(std::size_t c = 0; c < dim; c ++) {
          matrix[r].add(tokens[
            1 + c + (r * dim)
          ].toInt());
          inverse[r].add(tokens[
            1 + dim*dim + c + (r * dim)
          ].toInt());
        }
      }
    }catch(const std::exception& e) {
      std::cout << "Got " << e.what() << "\nWhile decypting. Format is possibly wrong" << std::endl;
      throw;
    }

    return SqMatrix(matrix, inverse);
  }

  inline clib::Text SqMatrix::toReadableTextInverse() const {
    return datadarrayToReadable(inverse);
  }

  inline Datadarray SqMatrix::identity(unsigned dim) {
    Datadarray l;
    for(unsigned i = 0; i < dim; i ++) {
      l.add(clib::darray<Z26>());
      for(unsigned j = 0; j < dim; j ++) {
        if(i == j) { l[i].add(1); continue; }
        l[i].add(0);
      }
    }
    return l;
  }

  inline SqMatrix::SqMatrix(const Datadarray& data): data(data) {
    std::size_t rCount = data.size();
    for(auto& row: data) {
      //TODO custom error
      if(row.size() != rCount)
        throw std::runtime_error("Matrix must be square, and be a matrix!");
    }
    calculateInverse();
  }

  inline const Datadarray& SqMatrix::asdarray() const {
    return data;
  }
  inline const Datadarray& SqMatrix::inverseAsdarray() const {
    return inverse;
  }

  inline void SqMatrix::calculateInverse() {

    Datadarray thisDataCopy = this->data;

    std::size_t dim = thisDataCopy.size();

    Datadarray inverted = identity(dim);

    clib::darray<std::size_t> usedColumns;

    for(std::size_t r = 0; r < dim; r ++) {

      signed pivotColumn = -1;
      for(std::size_t c = 0; c < dim; c ++) {
        if(
          thisDataCopy[r][c].hasInverse() &&
          !usedColumns.contains(c)
        ) {
          pivotColumn = c;
          usedColumns.add(pivotColumn);
          break;
        }
      }
      if(pivotColumn < 0)
        throw utils::MathError("Matrix not invertable!");
      
      Z26 pivotInverse = thisDataCopy[r][pivotColumn].inverse();

      for(std::size_t c = 0; c < dim; c ++) {
        thisDataCopy[r][c] *= pivotInverse;
        inverted[r][c] *= pivotInverse;
      }

      for(std::size_t otherRow = 0; otherRow < dim; otherRow ++) {
        if(otherRow == r) continue;
        Z26 scaler = -thisDataCopy[otherRow][pivotColumn];
        for(std::size_t c = 0; c < dim; c ++) {
          thisDataCopy[otherRow][c] += thisDataCopy[r][c] * scaler;
          inverted[otherRow][c] += inverted[r][c] * scaler;
        }
      }
    }

    this->inverse = inverted;
  }

  inline clib::darray<Z26> SqMatrix::applyDatadarray(
    const Datadarray& data,
    const clib::darray<Z26>& vec
  ) {
    
    clib::darray<clib::darray<Z26>> vectorSplits;
    for(std::size_t i = 0; i < vec.size() / data.size(); i ++) {
      vectorSplits.add(clib::darray<Z26>());
      for(std::size_t j = 0; j < data.size(); j ++) {
        vectorSplits[i].add(vec[i * data.size() + j]);
      }
    }
    
    std::size_t dim = data.size();

    clib::darray<Z26> result;
    for(std::size_t split = 0; split < vec.size() / data.size(); split ++) {
      for(std::size_t row = 0; row < dim; row ++) {
        Z26 sum;
        for(std::size_t col = 0; col < dim; col ++) {
          sum += data[row][col] * vectorSplits[split][col];
        }
        result.add(sum);
      }
    }

    return result;
  }

  inline clib::darray<Z26> SqMatrix::apply(const clib::darray<Z26>& vec) const {
    return applyDatadarray(data, vec);
  }
  inline clib::darray<Z26> SqMatrix::applyInverse(const clib::darray<Z26>& vec) const {
    return applyDatadarray(inverse, vec);
  }

  inline clib::Text SqMatrix::datadarrayToReadable(const Datadarray& data)  {
    clib::Text res;
    for(auto& row: data) {
      for(auto& el: row) {
        res += clib::Text::fromInt(el.raw());
        res += " ";
      }
      res += "\n";
    }
    return res;
  }


}
