#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>

#include "fizkat/errors.h"
#include "fizkat/fleet.h"
#include "fizkat/inventory.h"

namespace fizkat {
namespace {

void EnsureEnd(std::istringstream& input) {
  input >> std::ws;
  if (!input.eof()) {
    throw CommandError("too many arguments");
  }
}

std::string ReadId(std::istringstream& input) {
  std::string inventory_id;
  if (!(input >> inventory_id)) {
    throw CommandError("expected inventory id");
  }
  EnsureEnd(input);
  return inventory_id;
}

std::unique_ptr<InventoryItem> ReadItem(std::istringstream& input) {
  std::string type;
  std::string inventory_id;
  std::string name;
  double energy = 0.0;
  int score = 0;
  if (!(input >> type >> inventory_id >> name)) {
    throw CommandError("expected type, inventory id and name");
  }
  if (type == "scooter" || type == "ebike" || type == "dock") {
    if (!(input >> energy)) {
      throw CommandError("expected energy");
    }
  }
  if (type == "scooter" || type == "ebike" || type == "bike" ||
      type == "skateboard") {
    if (!(input >> score)) {
      throw CommandError("expected integer score");
    }
  }
  EnsureEnd(input);
  if (type == "scooter") {
    return std::make_unique<ElectricScooter>(inventory_id, name, energy, score);
  }
  if (type == "bike") {
    return std::make_unique<Bicycle>(inventory_id, name, score);
  }
  if (type == "ebike") {
    return std::make_unique<ElectricBicycle>(inventory_id, name, energy, score);
  }
  if (type == "skateboard") {
    return std::make_unique<Skateboard>(inventory_id, name, score);
  }
  if (type == "dock") {
    return std::make_unique<DockStation>(inventory_id, name, energy);
  }
  if (type == "helmet") {
    return std::make_unique<Helmet>(inventory_id, name);
  }
  throw CommandError("unknown inventory type");
}

}

static bool ExecuteCommand(const std::string& line, Fleet& fleet,
                           std::ostream& output) {
  std::istringstream input(line);
  std::string command;
  if (!(input >> command)) {
    return true;
  }
  if (command == "add") {
    fleet.Add(ReadItem(input));
  } else if (command == "remove") {
    fleet.Remove(ReadId(input));
  } else if (command == "focus") {
    fleet.SetFocus(ReadId(input));
  } else {
    EnsureEnd(input);
    if (command == "list" || command == "novice") {
      fleet.Print(command == "novice", output);
    } else if (command == "count_novice") {
      output << fleet.CountNovice() << '\n';
    } else if (command == "energy") {
      output << fleet.TotalEnergy() << '\n';
    } else if (command == "report") {
      output << fleet.size() << ' ' << fleet.TotalEnergy() << '\n';
    } else if (command == "show_focus") {
      fleet.ShowFocus(output);
    } else if (command == "quit") {
      return false;
    } else {
      throw CommandError("unknown command");
    }
  }
  return true;
}

}

int main() {
  constexpr int kEnergyPrecision = 1;
  fizkat::Fleet fleet;
  std::cout << std::fixed << std::setprecision(kEnergyPrecision);
  std::string line;
  while (std::getline(std::cin, line)) {
    try {
      if (!fizkat::ExecuteCommand(line, fleet, std::cout)) {
        break;
      }
    } catch (const std::exception& error) {
      std::cout << "error: " << error.what() << '\n';
    }
  }
  return 0;
}
