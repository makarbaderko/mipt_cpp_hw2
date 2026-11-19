#include "fizkat/inventory.h"

#include <cctype>
#include <cmath>
#include <ostream>
#include <string>

#include "fizkat/errors.h"

namespace fizkat {
namespace {

constexpr int kMinimumScore = 1;
constexpr int kMaximumScore = 10;
constexpr int kMinimumNoviceScore = 6;
constexpr char kRejectedNamePart[] = "broken";

void ValidateEnergy(double energy) {
  if (!std::isfinite(energy) || energy < 0.0) {
    throw CommandError("energy must be finite and non-negative");
  }
}

}

NamedItem::NamedItem(const std::string& inventory_id, const std::string& name)
    : inventory_id_(inventory_id), name_(name) {
  if (inventory_id_.find_first_not_of(" \t\r\n\f\v") == std::string::npos ||
      name_.find_first_not_of(" \t\r\n\f\v") == std::string::npos) {
    throw CommandError("id and name must not be empty");
  }
}

std::ostream& operator<<(std::ostream& output, const InventoryItem& item) {
  item.Describe(output);
  return output;
}

bool operator<(const InventoryItem& left, const InventoryItem& right) {
  return left.inventory_id() < right.inventory_id();
}

bool operator==(const InventoryItem& left, const InventoryItem& right) {
  return left.inventory_id() == right.inventory_id();
}

Transport::Transport(const std::string& inventory_id, const std::string& name,
                     int score)
    : NamedItem(inventory_id, name), score_(score) {
  if (score < kMinimumScore || score > kMaximumScore) {
    throw CommandError("score must be in 1..10");
  }
}

bool Transport::IsOkForNovice() const { return score_ >= kMinimumNoviceScore; }

void Transport::Inspect() const {
  std::string lower_name;
  for (unsigned char symbol : name()) {
    lower_name += static_cast<char>(std::tolower(symbol));
  }
  if (lower_name.find(kRejectedNamePart) != std::string::npos) {
    throw InspectionError();
  }
}

ElectricScooter::ElectricScooter(const std::string& inventory_id,
                                 const std::string& name, double energy,
                                 int score)
    : Transport(inventory_id, name, score), energy_(energy) {
  ValidateEnergy(energy_);
}

ElectricBicycle::ElectricBicycle(const std::string& inventory_id,
                                 const std::string& name, double energy,
                                 int score)
    : Transport(inventory_id, name, score), energy_(energy) {
  ValidateEnergy(energy_);
}

DockStation::DockStation(const std::string& inventory_id,
                         const std::string& name, double energy)
    : NamedItem(inventory_id, name), energy_(energy) {
  ValidateEnergy(energy_);
}

}
