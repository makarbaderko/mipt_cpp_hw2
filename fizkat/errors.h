#ifndef FIZKAT_ERRORS_H_
#define FIZKAT_ERRORS_H_

#include <stdexcept>
#include <string>

namespace fizkat {

class FleetError : public std::runtime_error {
 public:
  explicit FleetError(const std::string& message)
      : std::runtime_error(message) {}
};

class InspectionError : public FleetError {
 public:
  InspectionError() : FleetError("inspection rejected") {}
};

class CommandError : public FleetError {
 public:
  explicit CommandError(const std::string& message) : FleetError(message) {}
};

}

#endif
