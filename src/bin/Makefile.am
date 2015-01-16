AM_CXXFLAGS = -I$(srcdir)/../include $(BOOST_CPPFLAGS)
LDADD = ../lib/libmteval.la $(BOOST_PROGRAM_OPTIONS_LIB)

bin_PROGRAMS = mteval-corpus mteval-sentence

mteval_corpus_SOURCES = mteval-corpus.cc
mteval_corpus_LDADD = $(LDADD)

mteval_sentence_SOURCES = mteval-sentence.cc
mteval_sentence_LDADD = $(LDADD)
