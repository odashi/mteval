#include <mteval/Statistics.h>

#include <utility>

using namespace std;

namespace MTEval {

Statistics::Statistics() {}

Statistics::Statistics(const Statistics& src) :
  int_map_(src.int_map_),
  real_map_(src.real_map_) {}

Statistics::Statistics(Statistics&& src) :
  int_map_(move(src.int_map_)),
  real_map_(move(src.real_map_)) {}

Statistics::~Statistics() {}

Statistics& Statistics::operator=(const Statistics& src) {
  int_map_ = src.int_map_;
  real_map_ = src.real_map_;
  return *this;
}

Statistics& Statistics::operator=(Statistics&& src) {
  int_map_ = move(src.int_map_);
  real_map_ = move(src.real_map_);
  return *this;
}

Statistics& Statistics::operator+=(const Statistics& src) {
  for (const auto& kv : src.int_map_) {
    int_map_[kv.first] += kv.second;
  }
  for (const auto& kv : src.real_map_) {
    real_map_[kv.first] += kv.second;
  }
  return *this;
}

Statistics Statistics::operator+(const Statistics& src) const {
  return move(Statistics(*this) += src);
}

int Statistics::getInt(const string& key) const {
  const auto it = int_map_.find(key);
  if (it == int_map_.end()) return 0;
  else return it->second;
}

double Statistics::getReal(const string& key) const {
  const auto it = real_map_.find(key);
  if (it == real_map_.end()) return 0.0;
  else return it->second;
}

void Statistics::addInt(const string& key, const int value) {
  int_map_[key] += value;
}

void Statistics::addReal(const string& key, const double value) {
  real_map_[key] += value;
}

} // namespace MTEval

