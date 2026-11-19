#ifndef FIZKAT_FLEET_H_
#define FIZKAT_FLEET_H_

#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "fizkat/inventory.h"
#include "fizkat/registry.h"

namespace fizkat {

class Fleet {
 public:
  void Add(std::unique_ptr<InventoryItem> item);

  void Remove(const std::string& inventory_id);

  void SetFocus(const std::string& inventory_id);

  void ShowFocus(std::ostream& output) const;

  void Print(bool only_novice, std::ostream& output) const;

  std::size_t CountNovice() const;

  double TotalEnergy() const { return SumEnergy(items_); }

  std::size_t size() const { return items_.size(); }

 private:
  std::size_t FindIndex(const std::string& inventory_id) const;

  std::vector<const InventoryItem*> SortedItems() const;

  Registry<std::shared_ptr<InventoryItem>> items_;

  std::weak_ptr<InventoryItem> focus_;
};

}

#endif
