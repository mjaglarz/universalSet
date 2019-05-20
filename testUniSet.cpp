#include <iostream>
#include <string>
#include "universalSet.hpp"

template <typename T, std::size_t N>
void printSizeAndElems(const typename UniversalSet<T, N>::Vector& v){
  std::cout << "Size: " << v.count() << std::endl;
  for(auto& e : v) std::cout << e << ", ";
  std::cout << std::endl;
}

int main(){
  constexpr std::size_t N = 10u;
  UniversalSet<std::string, N> us({"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"});
  auto v1 = us.makeVector();
  auto v2 = us.makeVector();

  for(std::size_t i = 0u; i < us.universeSize(); ++i){
    if(i & 1u){
      v1.insert(i);
    }else{
      v2.insert(i);
    }
  }

  auto v3 = v2;

  std::cout << (v1.isMember(1u) ? "Found" : "Couldn't find") << std::endl;
  std::cout << (v1.isMember(2u) ? "Found" : "Couldn't find") << std::endl;
  std::cout << (v2.remove(8u) ? "Removed" : "Couldn't remove") << std::endl;
  std::cout << (v2.remove(9u) ? "Removed" : "Couldn't remove") << std::endl;
  std::cout << (v3.insert(1u).second ? "Added" : "Couldn't add") << std::endl;
  std::cout << (v3.insert(2u).second ? "Added" : "Couldn't add") << std::endl;
  std::cout << (v3.insert(3u).second ? "Added" : "Couldn't add") << std::endl;
  std::cout << (v3.insert(4u).second ? "Added" : "Couldn't add") << std::endl;

  auto v4 = v1 + v3;
  const auto v5 = v3 - v2;
  auto v6 = v3 * v5;
  const auto v7 = v5 / v3;

  printSizeAndElems<std::string, N>(v4);
  printSizeAndElems<std::string, N>(v5);
  printSizeAndElems<std::string, N>(v6);
  printSizeAndElems<std::string, N>(v7);

  auto it1 = v7.elemIterator(1u);
  auto it2 = v7.elemIterator(2u);

  std::cout << (it1 ? *it1 : "end") << std::endl;
  std::cout << (it2 ? *it2 : "end") << std::endl;
  std::cout << us[2] << std::endl;

  return 0;
}
