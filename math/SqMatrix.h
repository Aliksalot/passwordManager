#pragma once

#include"../utils/array.h"
#include<iostream>
#include"./RemRing.h"

namespace math {

  /**
   * Not a very abstract class, but such isn't needed for this project
   * Arithmetic is defined as arithmetic in Z/26Z
   * Which isn't explicitly defined in the class/function name/names,
   * so that they stay shorter and easier to type out
   */
  using DataList = clib::List<clib::List<Z26>>;

  class SqMatrix {
  private:
    void calculateInverse();

    SqMatrix(DataList& data, DataList& inverse);

    DataList data;
    DataList inverse;

    static clib::String dataListToReadable(const DataList& data);
    static clib::List<Z26> applyDataList(
      const DataList& data,
      const clib::List<Z26>& vec
    );

  public:
    SqMatrix() = delete;
    SqMatrix(const DataList& data);

    const DataList& asList() const;
    const DataList& inverseAsList() const;

    clib::String toReadableString() const;
    clib::String toReadableStringInverse() const;

    clib::String serialize() const;

    clib::List<Z26> apply(const clib::List<Z26>& vec) const;
    clib::List<Z26> applyInverse(const clib::List<Z26>& vec) const;

    static DataList identity(unsigned dim);
    static SqMatrix deserialize(const clib::String& raw);
  };


  inline SqMatrix::SqMatrix(DataList& data, DataList& inverse)
    : 
    data(
      static_cast<DataList&&>(data)
    ),
    inverse(
      static_cast<DataList&&>(inverse)
    ) { };

  inline clib::String SqMatrix::toReadableString() const {
    return dataListToReadable(data);
  }

  inline clib::String SqMatrix::serialize() const {
    clib::String result;

    result += clib::String::fromInt(data.size());
    result += "&";

    for(auto& row: data) {
      for(auto& item: row) {
        result += clib::String::fromInt(item.raw());
        std::cout << clib::String::fromInt(item.raw());
        result += "&";
      }
    }
    for(auto& row: inverse) {
      for(auto& item: row) {
        result += clib::String::fromInt(item.raw());
        result += "&";
      }
    }

    return result;
  }

  inline SqMatrix SqMatrix::deserialize(const clib::String& raw) {
    clib::List<clib::String> tokens = raw.split('&');
    //TODO custom error MAYBE?
    if(tokens.empty())
      throw std::runtime_error("Serialized matrix format doesn't match expected!");

    //Serializer appends extra &
    tokens.pop();

    DataList matrix, inverse;
    std::size_t dim = tokens[0].toInt();

    std::size_t expectedTokenCount = 1 + 2*dim*dim;
    if(tokens.size() != expectedTokenCount) {
      throw std::runtime_error("Serialized matrix format doesn't match expected!");
    }
    
    try{
    for(std::size_t r = 0; r < dim; r ++) {
      matrix.add(clib::List<Z26>());
      inverse.add(clib::List<Z26>());
      for(std::size_t c = 0; c < dim; c ++) {
        matrix[r].add(tokens[
          1 + c + (r * dim)
        ].toInt());
        inverse[r].add(tokens[
          1 + dim*dim + c + (r * dim)
        ].toInt());
      }
    }
    }catch(const std::out_of_range& e) {
      std::cout << "Got " << e.what() << "\nWhile decypting. Format is possibly wrong" << std::endl;
    }

    return SqMatrix(matrix, inverse);
  }

  inline clib::String SqMatrix::toReadableStringInverse() const {
    return dataListToReadable(inverse);
  }

  inline DataList SqMatrix::identity(unsigned dim) {
    DataList l;
    for(unsigned i = 0; i < dim; i ++) {
      l.add(clib::List<Z26>());
      for(unsigned j = 0; j < dim; j ++) {
        if(i == j) { l[i].add(1); continue; }
        l[i].add(0);
      }
    }
    return l;
  }

  inline SqMatrix::SqMatrix(const DataList& data): data(data) {
    std::size_t rCount = data.size();
    for(auto& row: data) {
      //TODO custom error
      if(row.size() != rCount)
        throw std::runtime_error("Matrix must be square, and be a matrix!");
    }
    calculateInverse();
  }

  inline const DataList& SqMatrix::asList() const {
    return data;
  }
  inline const DataList& SqMatrix::inverseAsList() const {
    return inverse;
  }

  inline void SqMatrix::calculateInverse() {

    DataList thisDataCopy = this->data;

    std::size_t dim = thisDataCopy.size();

    DataList inverted = identity(dim);

    clib::List<std::size_t> usedColumns;

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
      //TODO add custom error
      if(pivotColumn < 0)
        throw std::runtime_error("Matrix not invertable!");
      
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

  inline clib::List<Z26> SqMatrix::applyDataList(
    const DataList& data,
    const clib::List<Z26>& vec
  ) {
    
    clib::List<clib::List<Z26>> vectorSplits;
    for(std::size_t i = 0; i < vec.size() / data.size(); i ++) {
      vectorSplits.add(clib::List<Z26>());
      for(std::size_t j = 0; j < data.size(); j ++) {
        vectorSplits[i].add(vec[i * data.size() + j]);
      }
    }
    
    std::size_t dim = data.size();

    clib::List<Z26> result;
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

  inline clib::List<Z26> SqMatrix::apply(const clib::List<Z26>& vec) const {
    return applyDataList(data, vec);
  }
  inline clib::List<Z26> SqMatrix::applyInverse(const clib::List<Z26>& vec) const {
    return applyDataList(inverse, vec);
  }

  inline clib::String SqMatrix::dataListToReadable(const DataList& data)  {
    clib::String res;
    for(auto& row: data) {
      for(auto& el: row) {
        res += clib::String::fromInt(el.raw());
        res += " ";
      }
      res += "\n";
    }
    return res;
  }


}
