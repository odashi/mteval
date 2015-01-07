AM_CXXFLAGS = -I$(srcdir)/../include $(BOOST_CPPFLAGS)
LDADD = ../lib/libmteval.la $(BOOST_PROGRAM_OPTIONS_LIB)

bin_PROGRAMS = mteval

mteval_SOURCES = main.cc
mteval_LDADD = $(LDADD)
