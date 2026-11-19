#ifndef FIZKAT_REGISTRY_H_
#define FIZKAT_REGISTRY_H_

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

namespace fizkat {

template <typename T>
class Registry {
 public:
  void Add(T value) { entries_.push_back(std::move(value)); }
  std::size_t size() const { return entries_.size(); }

  const T& operator[](std::size_t index) const { return entries_.at(index); }
  const std::vector<T>& entries() const { return entries_; }

  void Remove(std::size_t index) {
    if (index >= entries_.size()) {
      throw std::out_of_range("registry index out of range");
    }
    entries_.erase(entries_.begin() + index);
  }

 private:
  std::vector<T> entries_;
};

template <typename Container>
double SumEnergy(const Container& items) {
  double total = 0.0;
  for (std::size_t index = 0; index < items.size(); ++index) {
    total += items[index]->DailyEnergy();
  }
  return total;
}

}

#endif
