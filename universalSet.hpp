#ifndef UNIVERSALSET_H
#define UNIVERSALSET_H

#include <cstdint>
#include <array>
#include <bitset>

template<typename T, std::size_t UNIVERSE_SIZE>
class UniversalSet{
public:
  class Vector{
    friend class Iterator;
  public:
    class Iterator{
      friend class Vector;
    private:
      const Vector* vector_ptr_;
      std::size_t i_;

      Iterator(const Vector* vector_ptr):vector_ptr_(vector_ptr){}
      Iterator(const Vector* vector_ptr, std::size_t i):vector_ptr_(vector_ptr), i_(i){}
    public:
      bool operator==(const Iterator& it) const;
      bool operator!=(const Iterator& it) const;
      Iterator& operator++();
      Iterator operator++(int);
      const T& operator*() const;
      const T* operator->() const;
      operator bool() const;

      Iterator& operator=(const Iterator&) = default;
      Iterator& operator=(Iterator&&) = default;

      Iterator() = default;
      Iterator(const Iterator&) = default;
      Iterator(Iterator&&) = default;
      ~Iterator() = default;
    };

  private:
    std::bitset<UNIVERSE_SIZE> bs_;
    const UniversalSet<T, UNIVERSE_SIZE>* universe_ptr_;

  public:
    std::pair<Iterator, bool> insert(std::size_t i);
    bool remove(std::size_t i);
    bool isMember(std::size_t i) const;
    Iterator elemIterator(std::size_t i) const;
    std::size_t count() const;

    bool operator==(const Vector& v2) const;
    bool operator!=(const Vector& v2) const;
    Vector operator+(const Vector& v2) const;
    Vector operator-(const Vector& v2) const;
    Vector operator*(const Vector& v2) const;
    Vector operator/(const Vector& v2) const;

    Iterator begin() const;
    Iterator end() const;

    Vector& operator=(const Vector& universe) = default;
    Vector& operator=(Vector&& universe) = default;

    Vector(const UniversalSet* universe_ptr):universe_ptr_(universe_ptr){}
    Vector(const UniversalSet* universe_ptr, const std::bitset<UNIVERSE_SIZE> &bs):bs_(bs), universe_ptr_(universe_ptr){}
    Vector(const Vector&) = default;
    Vector(Vector&& universe) = default;
    ~Vector() = default;
  };

private:
  std::array<T, UNIVERSE_SIZE> elems_;

public:
  const T& elem(std::size_t i) const;
  const T& operator[](std::size_t i) const;
  Vector makeVector() const;
  constexpr std::size_t universeSize() const { return UNIVERSE_SIZE; }

  UniversalSet& operator=(const UniversalSet&) = default;
  UniversalSet& operator=(UniversalSet&&) = default;

  UniversalSet(const std::array<T, UNIVERSE_SIZE> &elems):elems_(elems){}
  UniversalSet(const UniversalSet&) = default;
  UniversalSet(UniversalSet&&) = default;
  ~UniversalSet() = default;
};

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator==(const Iterator& it) const{
  return (vector_ptr_ == it.vector_ptr_ && i_ == it.i_);
}

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator!=(const Iterator& it) const{
  return !(*this == it);
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator&
UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator++(){
  if(vector_ptr_->bs_.none()){
    throw "None of the bits are set to true.";
  }

  size_t index = i_ + 1;
  while(index < vector_ptr_->bs_.size() && !vector_ptr_->bs_.test(index)){
    ++index;
  }

  i_ = index;
  return *this;
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator
UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator++(int){
  Iterator result(*this);
  ++(*this);
  return result;
}

template<typename T, std::size_t UNIVERSE_SIZE>
const T& UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator*() const{
  return vector_ptr_->universe_ptr_->elem(i_);
}

template<typename T, std::size_t UNIVERSE_SIZE>
const T* UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator->() const{
  return &vector_ptr_->universe_ptr_->elem(i_);
}

template<typename T, std::size_t UNIVERSE_SIZE>
UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator bool() const{
   return i_ != UNIVERSE_SIZE;
 }


template<typename T, std::size_t UNIVERSE_SIZE>
std::pair<typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator, bool>
UniversalSet<T, UNIVERSE_SIZE>::Vector::insert(std::size_t i){
  if(isMember(i)){
    return std::make_pair(end(), false);
  }else{
    bs_.set(i);
    return std::make_pair(Iterator(this, i), true);
  }
}

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T, UNIVERSE_SIZE>::Vector::remove(std::size_t i){
  if(isMember(i)){
    bs_.reset(i);
    return true;
  }else{
    return false;
  }
}

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T, UNIVERSE_SIZE>::Vector::isMember(std::size_t i) const{
  return bs_.test(i);
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator
UniversalSet<T, UNIVERSE_SIZE>::Vector::elemIterator(std::size_t i) const{
  if(isMember(i)){
    return Iterator(this, i);
  }else{
    return end();
  }
}

template<typename T, std::size_t UNIVERSE_SIZE>
std::size_t UniversalSet<T, UNIVERSE_SIZE>::Vector::count() const{
  return bs_.count();
}

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T, UNIVERSE_SIZE>::Vector::operator==(const Vector& v2) const{
  return bs_ == v2.bs_;
}

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T, UNIVERSE_SIZE>::Vector::operator!=(const Vector& v2) const{
  return bs_ != v2.bs_;
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector
UniversalSet<T, UNIVERSE_SIZE>::Vector::operator+(const Vector& v2) const{
  std::bitset<UNIVERSE_SIZE> bs = v2.bs_;
  for(size_t i = 0u; i < UNIVERSE_SIZE; ++i){
    bs |= bs_;
  }

  return Vector(universe_ptr_, bs);
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector
UniversalSet<T, UNIVERSE_SIZE>::Vector::operator-(const Vector& v2) const{
  std::bitset<UNIVERSE_SIZE> bs;
  for(size_t i = 0u; i < UNIVERSE_SIZE; ++i){
    if(bs_.test(i) && !v2.bs_.test(i)){
      bs.set(i);
    }else{
      bs.reset(i);
    }
  }

  return Vector(universe_ptr_, bs);
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector
UniversalSet<T, UNIVERSE_SIZE>::Vector::operator*(const Vector& v2) const{
  std::bitset<UNIVERSE_SIZE> bs = v2.bs_;
  for(size_t i = 0u; i < UNIVERSE_SIZE; ++i){
    bs &= bs_;
  }

  return Vector(universe_ptr_, bs);
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector
UniversalSet<T, UNIVERSE_SIZE>::Vector::operator/(const Vector& v2) const{
  std::bitset<UNIVERSE_SIZE> bs;
  for(size_t i = 0u; i < UNIVERSE_SIZE; ++i){
    if(!bs_.test(i) != !v2.bs_.test(i)){
      bs.set(i);
    }else{
      bs.reset(i);
    }
  }

  return Vector(universe_ptr_, bs);
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator
UniversalSet<T, UNIVERSE_SIZE>::Vector::begin() const{
  if(bs_.none()){
    return end();
  }else{
    size_t index = 0u;
    while(index < bs_.size() && !bs_.test(index)){
      ++index;
    }

    return Iterator(this, index);
  }
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator
UniversalSet<T, UNIVERSE_SIZE>::Vector::end() const{
  return Iterator(this, UNIVERSE_SIZE);
}


template<typename T, std::size_t UNIVERSE_SIZE>
const T& UniversalSet<T, UNIVERSE_SIZE>::elem(std::size_t i) const{
  return elems_.at(i);
}

template<typename T, std::size_t UNIVERSE_SIZE>
const T& UniversalSet<T, UNIVERSE_SIZE>::operator[](std::size_t i) const{
  return elem(i);
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector UniversalSet<T, UNIVERSE_SIZE>::makeVector() const{
  Vector vector(this);
  return vector;
}

#endif
