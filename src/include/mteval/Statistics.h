#pragma once

#include <map>
#include <string>

namespace MTEval {

class Statistics {
public:
  Statistics();
  Statistics(const Statistics& src);
  Statistics(Statistics&& src);
  
  ~Statistics();

  Statistics& operator=(const Statistics& src);
  Statistics& operator=(Statistics&& src);

  Statistics& operator+=(const Statistics& src);

  Statistics operator+(const Statistics& src) const;

  int getInt(const std::string& key) const;
  double getReal(const std::string& key) const;

  void addInt(const std::string& key, const int value);
  void addReal(const std::string& key, const double value);

  const std::map<std::string, int>& intEntries() const { return int_map_; }
  const std::map<std::string, double>& realEntries() const { return real_map_; }

private:
  std::map<std::string, int> int_map_;
  std::map<std::string, double> real_map_;

}; // class Statistics

} // namespace MTEval

