# mteval library

set(mteval_LIBRARY_HDRS
  BLEUEvaluator.h
  Dictionary.h
  Evaluator.h
  EvaluatorFactory.h
  NISTEvaluator.h
  RIBESEvaluator.h
  Statistics.h
  WEREvaluator.h
  utils.h
)

set(mteval_LIBRARY_SRCS
  BLEUEvaluator.cc
  Dictionary.cc
  EvaluatorFactory.cc
  NISTEvaluator.cc
  RIBESEvaluator.cc
  Statistics.cc
  WEREvaluator.cc
  utils.cc
)

add_library(mteval ${mteval_LIBRARY_HDRS} ${mteval_LIBRARY_SRCS})
target_link_libraries(mteval ${Boost_LIBRARIES})
