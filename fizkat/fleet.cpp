#include "fizkat/fleet.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "fizkat/errors.h"
#include "fizkat/inventory.h"

namespace fizkat {

void Fleet::Add(std::unique_ptr<InventoryItem> item) {
  if (!item) {
    throw CommandError("null inventory item");
  }
  for (const auto& current : items_.entries()) {
    if (*current == *item) {
      throw CommandError("duplicate inventory id");
    }
  }
  item->Inspect();
  if (!std::isfinite(TotalEnergy() + item->DailyEnergy())) {
    throw CommandError("total energy is too large");
  }
  std::shared_ptr<InventoryItem> accepted_item(std::move(item));
  items_.Add(std::move(accepted_item));
}

std::size_t Fleet::FindIndex(const std::string& inventory_id) const {
  for (std::size_t index = 0; index < items_.size(); ++index) {
    if (items_[index]->inventory_id() == inventory_id) {
      return index;
    }
  }
  throw CommandError("inventory id not found");
}

void Fleet::Remove(const std::string& inventory_id) {
  items_.Remove(FindIndex(inventory_id));
}

void Fleet::SetFocus(const std::string& inventory_id) {
  focus_ = items_[FindIndex(inventory_id)];
}

void Fleet::ShowFocus(std::ostream& output) const {
  const auto item = focus_.lock();
  if (item) {
    output << *item << '\n';
  } else {
    output << "expired\n";
  }
}

std::vector<const InventoryItem*> Fleet::SortedItems() const {
  std::vector<const InventoryItem*> sorted;
  for (const auto& item : items_.entries()) {
    sorted.push_back(item.get());
  }
  std::sort(sorted.begin(), sorted.end(),
            [](const InventoryItem* left, const InventoryItem* right) {
              return *left < *right;
            });
  return sorted;
}

void Fleet::Print(bool only_novice, std::ostream& output) const {
  for (const auto* item : SortedItems()) {
    if (!only_novice || item->IsOkForNovice()) {
      output << *item << '\n';
    }
  }
}

std::size_t Fleet::CountNovice() const {
  std::size_t count = 0;
  for (const auto& item : items_.entries()) {
    if (item->IsOkForNovice()) {
      ++count;
    }
  }
  return count;
}

}
