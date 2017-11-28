#define BOOST_TEST_MODULE statistics_test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <mteval/Statistics.h>

#include <utility>

using namespace std;
using namespace MTEval;

BOOST_AUTO_TEST_SUITE(StatisticsTest)

BOOST_AUTO_TEST_CASE(CheckEmpty) {
  Statistics stat;
  BOOST_CHECK_EQUAL(0, stat.intEntries().size());
  BOOST_CHECK_EQUAL(0, stat.realEntries().size());
}

BOOST_AUTO_TEST_CASE(CheckAddEntry1) {
  Statistics stat;
  stat.addInt("foo", 1);
  stat.addInt("bar", 42);
  stat.addReal("baz", 42.0);
  BOOST_CHECK_EQUAL(2, stat.intEntries().size());
  BOOST_CHECK_EQUAL(1, stat.realEntries().size());
  BOOST_CHECK_EQUAL(1, stat.getInt("foo"));
  BOOST_CHECK_EQUAL(42, stat.getInt("bar"));
  BOOST_CHECK_EQUAL(0, stat.getInt("baz"));
  BOOST_CHECK_CLOSE(0.0, stat.getReal("foo"), 1e-12);
  BOOST_CHECK_CLOSE(0.0, stat.getReal("bar"), 1e-12);
  BOOST_CHECK_CLOSE(42.0, stat.getReal("baz"), 1e-12);
}

BOOST_AUTO_TEST_CASE(CheckAddEntry2) {
  Statistics stat;
  stat.addInt("foo", 1);
  stat.addInt("foo", 2);
  stat.addReal("bar", 10.0);
  stat.addReal("bar", 20.0);
  BOOST_CHECK_EQUAL(1, stat.intEntries().size());
  BOOST_CHECK_EQUAL(1, stat.realEntries().size());
  BOOST_CHECK_EQUAL(3, stat.getInt("foo"));
  BOOST_CHECK_CLOSE(30.0, stat.getReal("bar"), 1e-12);
}

BOOST_AUTO_TEST_CASE(CheckCopy) {
  Statistics stat1, stat2;
  stat1.addInt("foo", 1);
  stat1.addInt("bar", 42);
  stat1.addReal("baz", 42.0);
  stat2 = stat1;
  BOOST_CHECK_EQUAL(2, stat2.intEntries().size());
  BOOST_CHECK_EQUAL(1, stat2.realEntries().size());
  BOOST_CHECK_EQUAL(1, stat2.getInt("foo"));
  BOOST_CHECK_EQUAL(42, stat2.getInt("bar"));
  BOOST_CHECK_EQUAL(0, stat2.getInt("baz"));
  BOOST_CHECK_CLOSE(0.0, stat2.getReal("foo"), 1e-12);
  BOOST_CHECK_CLOSE(0.0, stat2.getReal("bar"), 1e-12);
  BOOST_CHECK_CLOSE(42.0, stat2.getReal("baz"), 1e-12);
  Statistics stat3 = stat2;
  BOOST_CHECK_EQUAL(2, stat3.intEntries().size());
  BOOST_CHECK_EQUAL(1, stat3.realEntries().size());
  BOOST_CHECK_EQUAL(1, stat3.getInt("foo"));
  BOOST_CHECK_EQUAL(42, stat3.getInt("bar"));
  BOOST_CHECK_EQUAL(0, stat3.getInt("baz"));
  BOOST_CHECK_CLOSE(0.0, stat3.getReal("foo"), 1e-12);
  BOOST_CHECK_CLOSE(0.0, stat3.getReal("bar"), 1e-12);
  BOOST_CHECK_CLOSE(42.0, stat3.getReal("baz"), 1e-12);
}

BOOST_AUTO_TEST_CASE(CheckMove) {
  Statistics stat1, stat2;
  stat1.addInt("foo", 1);
  stat1.addInt("bar", 42);
  stat1.addReal("baz", 42.0);
  stat2 = move(stat1);
  BOOST_CHECK_EQUAL(2, stat2.intEntries().size());
  BOOST_CHECK_EQUAL(1, stat2.realEntries().size());
  BOOST_CHECK_EQUAL(1, stat2.getInt("foo"));
  BOOST_CHECK_EQUAL(42, stat2.getInt("bar"));
  BOOST_CHECK_EQUAL(0, stat2.getInt("baz"));
  BOOST_CHECK_CLOSE(0.0, stat2.getReal("foo"), 1e-12);
  BOOST_CHECK_CLOSE(0.0, stat2.getReal("bar"), 1e-12);
  BOOST_CHECK_CLOSE(42.0, stat2.getReal("baz"), 1e-12);
  Statistics stat3 = move(stat2);
  BOOST_CHECK_EQUAL(2, stat3.intEntries().size());
  BOOST_CHECK_EQUAL(1, stat3.realEntries().size());
  BOOST_CHECK_EQUAL(1, stat3.getInt("foo"));
  BOOST_CHECK_EQUAL(42, stat3.getInt("bar"));
  BOOST_CHECK_EQUAL(0, stat3.getInt("baz"));
  BOOST_CHECK_CLOSE(0.0, stat3.getReal("foo"), 1e-12);
  BOOST_CHECK_CLOSE(0.0, stat3.getReal("bar"), 1e-12);
  BOOST_CHECK_CLOSE(42.0, stat3.getReal("baz"), 1e-12);
}

BOOST_AUTO_TEST_CASE(CheckIntegrate1) {
  Statistics stat1, stat2;
  stat1.addInt("foo", 1);
  stat1.addInt("bar", 42);
  stat1.addReal("foo", 1.0);
  stat1.addReal("bar", 42.0);
  stat2.addInt("bar", 1);
  stat2.addInt("baz", 42);
  stat2.addReal("bar", 1.0);
  stat2.addReal("baz", 42.0);
  stat1 += stat2;
  BOOST_CHECK_EQUAL(3, stat1.intEntries().size());
  BOOST_CHECK_EQUAL(3, stat1.realEntries().size());
  BOOST_CHECK_EQUAL(1, stat1.getInt("foo"));
  BOOST_CHECK_EQUAL(43, stat1.getInt("bar"));
  BOOST_CHECK_EQUAL(42, stat1.getInt("baz"));
  BOOST_CHECK_CLOSE(1.0, stat1.getReal("foo"), 1e-12);
  BOOST_CHECK_CLOSE(43.0, stat1.getReal("bar"), 1e-12);
  BOOST_CHECK_CLOSE(42.0, stat1.getReal("baz"), 1e-12);
}

BOOST_AUTO_TEST_CASE(CheckIntegrate2) {
  Statistics stat1, stat2, stat3;
  stat1.addInt("foo", 1);
  stat1.addInt("bar", 42);
  stat1.addReal("foo", 1.0);
  stat1.addReal("bar", 42.0);
  stat2.addInt("bar", 1);
  stat2.addInt("baz", 42);
  stat2.addReal("bar", 1.0);
  stat2.addReal("baz", 42.0);
  stat3 = stat1 + stat2;
  BOOST_CHECK_EQUAL(3, stat3.intEntries().size());
  BOOST_CHECK_EQUAL(3, stat3.realEntries().size());
  BOOST_CHECK_EQUAL(1, stat3.getInt("foo"));
  BOOST_CHECK_EQUAL(43, stat3.getInt("bar"));
  BOOST_CHECK_EQUAL(42, stat3.getInt("baz"));
  BOOST_CHECK_CLOSE(1.0, stat3.getReal("foo"), 1e-12);
  BOOST_CHECK_CLOSE(43.0, stat3.getReal("bar"), 1e-12);
  BOOST_CHECK_CLOSE(42.0, stat3.getReal("baz"), 1e-12);
}

BOOST_AUTO_TEST_SUITE_END()

