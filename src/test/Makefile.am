AM_CXXFLAGS = -I$(srcdir)/../include $(BOOST_CPPFLAGS)
LDADD = -lstdc++ ../lib/libmteval.la $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

check_PROGRAMS = \
	statistics_test

TESTS = \
	statistics_test

statistics_test_SOURCES = statistics_test.cc
