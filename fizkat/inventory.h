#ifndef FIZKAT_INVENTORY_H_
#define FIZKAT_INVENTORY_H_

#include <ostream>
#include <string>

namespace fizkat {

class InventoryItem {
 public:
  virtual ~InventoryItem() = default;
  virtual const std::string& inventory_id() const = 0;
  virtual const std::string& name() const = 0;
  virtual void Describe(std::ostream& output) const = 0;
  virtual double DailyEnergy() const { return 0.0; }
  virtual bool IsOkForNovice() const { return false; }
  virtual void Inspect() const {}
};

class EnergyConsumer {
 public:
  virtual ~EnergyConsumer() = default;
  virtual double DailyEnergy() const = 0;
};

class NamedItem : public InventoryItem {
 public:
  NamedItem(const std::string& inventory_id, const std::string& name);
  const std::string& inventory_id() const override { return inventory_id_; }
  const std::string& name() const override { return name_; }
  void Describe(std::ostream& output) const override {
    output << inventory_id_ << ' ' << name_;
  }

 private:
  std::string inventory_id_;
  std::string name_;
};

class Transport : public NamedItem {
 public:
  Transport(const std::string& inventory_id, const std::string& name,
            int score);
  bool IsOkForNovice() const override;
  void Inspect() const override;

 private:
  int score_;
};

class Bicycle : public Transport {
 public:
  using Transport::Transport;
};

class ElectricScooter : public Transport, public EnergyConsumer {
 public:
  ElectricScooter(const std::string& inventory_id, const std::string& name,
                  double energy, int score);
  double DailyEnergy() const override { return energy_; }

 private:
  double energy_;
};

class ElectricBicycle : public Transport, public EnergyConsumer {
 public:
  ElectricBicycle(const std::string& inventory_id, const std::string& name,
                  double energy, int score);
  double DailyEnergy() const override { return energy_; }

 private:
  double energy_;
};

class Helmet : public NamedItem {
 public:
  using NamedItem::NamedItem;
};

class DockStation : public NamedItem, public EnergyConsumer {
 public:
  DockStation(const std::string& inventory_id, const std::string& name,
              double energy);
  double DailyEnergy() const override { return energy_; }

 private:
  double energy_;
};

class Skateboard : public Transport {
 public:
  using Transport::Transport;
};

std::ostream& operator<<(std::ostream& output, const InventoryItem& item);
bool operator<(const InventoryItem& left, const InventoryItem& right);
bool operator==(const InventoryItem& left, const InventoryItem& right);

}

#endif
