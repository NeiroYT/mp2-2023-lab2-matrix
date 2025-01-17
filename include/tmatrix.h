﻿// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <cassert>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор - 
// шаблонный вектор на динамической памяти
template<typename T>
class TDynamicVector
{
protected:
  size_t sz;
  T* pMem;
public:
  TDynamicVector(size_t size = 1) : sz(size)
  {
      if (sz == 0 || sz > MAX_VECTOR_SIZE)
          throw out_of_range("Vector size should be greater than zero and not greater than MAX_VECTOR_SIZE");
      pMem = new T[sz]();// {}; // У типа T д.б. констуктор по умолчанию
  }
  TDynamicVector(T* arr, size_t s) : sz(s)
  {
      assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
      if (sz == 0 || sz > MAX_VECTOR_SIZE)
          throw out_of_range("Vector size should be greater than zero and not greater than MAX_VECTOR_SIZE");
      pMem = new T[sz];
      copy(arr, arr + sz, pMem);
  }
  TDynamicVector(const TDynamicVector& v)
  {
      sz = v.sz;
      pMem = new T[sz];
      copy(v.pMem, v.pMem + sz, pMem);
  }
  TDynamicVector(TDynamicVector&& v) noexcept
  {
      sz = v.sz;
      pMem = v.pMem;
      v.sz = 0;
      v.pMem = nullptr;
  }
  ~TDynamicVector()
  {
      delete[] pMem;
      sz = 0;
  }
  TDynamicVector& operator=(const TDynamicVector& v)
  {
      if (v == *this) {
          return *this;
      }
      if (sz != v.sz) {
          T *tmp = pMem;
          sz = v.sz;
          pMem = new T[sz];
          delete[] tmp;
      }
      copy(v.pMem, v.pMem + sz, pMem);
      return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept
  {
      sz = v.sz;
      pMem = v.pMem;
      v.sz = 0;
      v.pMem = nullptr;
      return *this;
  }

  size_t size() const noexcept { return sz; }

  // индексация
  T& operator[](size_t ind)
  {
      return pMem[ind];
  }
  const T& operator[](size_t ind) const
  {
      return pMem[ind];
  }
  // индексация с контролем
  T& at(size_t ind)
  {
      if (ind >= sz)
          throw out_of_range("Index should be less than size");
      return pMem[ind];
  }
  const T& at(size_t ind) const
  {
      if (ind >= sz)
          throw out_of_range("Index should be less than size");
      return pMem[ind];
  }

  // сравнение
  bool operator==(const TDynamicVector& v) const noexcept
  {
      if (sz != v.sz) {
          return false;
      }
      for (size_t i = 0; i < sz; i++) {
          if (pMem[i] != v.pMem[i]) {
              return false;
          }
      }
      return true;
  }
  bool operator!=(const TDynamicVector& v) const noexcept
  {
      return !(*this == v);
  }

  // скалярные операции
  TDynamicVector operator+(T val)
  {
      TDynamicVector<T> res = *this;
      for (size_t i = 0; i < sz; i++) {
          res[i] += val;
      }
      return res;
  }
  TDynamicVector operator-(T val)
  {
      TDynamicVector<T> res = *this;
      for (size_t i = 0; i < sz; i++) {
          res[i] -= val;
      }
      return res;
  }
  TDynamicVector operator*(T val)
  {
      TDynamicVector<T> res = *this;
      for (size_t i = 0; i < sz; i++) {
          res[i] *= val;
      }
      return res;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v)
  {
      if (sz != v.sz) {
          throw exception("not equal size");
      }
      TDynamicVector<T> res = *this;
      for (size_t i = 0; i < sz; i++) {
          res[i] += v[i];
      }
      return res;
  }
  TDynamicVector operator-(const TDynamicVector& v)
  {
      if (sz != v.sz) {
          throw exception("not equal size");
      }
      TDynamicVector<T> res = *this;
      for (size_t i = 0; i < sz; i++) {
          res[i] -= v[i];
      }
      return res;
  }
  T operator*(const TDynamicVector& v) //noexcept(noexcept(T())) // ???
  {
      if (sz != v.sz) {
          throw exception("not equal size");
      }
      T res = (T)0;
      for (size_t i = 0; i < sz; i++) {
          res += pMem[i] * v[i];
      }
      return res;
  }

  friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
  {
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.pMem, rhs.pMem);
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicVector& v)
  {
      for (size_t i = 0; i < v.sz; i++)
          istr >> v.pMem[i]; // требуется оператор>> для типа T
          if (istr.fail()) {
              throw invalid_argument("bad matrix input");
          }
      return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
  {
      for (size_t i = 0; i < v.sz; i++)
        ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
      return ostr;
  }
};


// Динамическая матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;
public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
  {
      if (sz > MAX_MATRIX_SIZE) {
          throw out_of_range("Matrix size should be <= MAX_MATRIX_SIZE");
      }
      for (size_t i = 0; i < sz; i++)
          pMem[i] = TDynamicVector<T>(sz);
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];
  using TDynamicVector<TDynamicVector<T>>::at;
  using TDynamicVector<TDynamicVector<T>>::size;

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
      return TDynamicVector<TDynamicVector<T>>::operator==(m);
  }
  bool operator!=(const TDynamicMatrix &m) const noexcept
  {
      return TDynamicVector<TDynamicVector<T>>::operator!=(m);
  }

  // матрично-скалярные операции
  TDynamicMatrix<T> operator*(const T& val)
  {
      TDynamicMatrix<T> res = *this;
      for (size_t i = 0; i < sz; i++) {
          res[i] = res[i] * val;
      }
      return res;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v)
  {
      TDynamicVector<T> res(sz);
      for (size_t i = 0; i < sz; i++) {
          res[i] = pMem[i] * v[i];
      }
      return res;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m)
  {
      if (sz != m.sz) {
          throw exception("not equal matrix size");
      }
      TDynamicMatrix<T> res = *this;
      for (size_t i = 0; i < sz; i++) {
          res[i] = res[i] + m[i];
      }
      return res;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m)
  {
      if (sz != m.sz) {
          throw exception("not equal matrix size");
      }
      TDynamicMatrix<T> res = *this;
      for (size_t i = 0; i < sz; i++) {
          res[i] = res[i] - m[i];
      }
      return res;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m)
  {
      if (sz != m.sz) {
          throw exception("not equal matrix size");
      }
      TDynamicMatrix<T> m_transposed = m.transpose();
      TDynamicMatrix<T> res(sz);
      for (size_t i = 0; i < sz; i++) {
          for (size_t j = 0; j < sz; j++) {
              res[i][j] = pMem[i] * m_transposed[j];
          }
      }
      return res;
  }
  TDynamicMatrix transpose() const {
      TDynamicMatrix<T> res(sz);
      for (size_t i = 0; i < sz; i++) {
          for (size_t j = 0; j < sz; j++) {
              res[i][j] = pMem[j][i];
          }
      }
      return res;
  }
  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicMatrix& v)
  {
      for (size_t i = 0; i < v.sz; i++) {
          istr >> v.pMem[i];
      }
      return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& v)
  {
      for (size_t i = 0; i < v.sz; i++) {
          ostr << v.pMem[i] << '\n';
      }
      return ostr;
  }
};

#endif
