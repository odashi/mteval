# mteval unit tests

add_executable(statistics_test statistics_test.cc)

target_link_libraries(statistics_test mteval ${Bost_LIBRARIES})

add_test(NAME statistics_test COMMAND statistics_test)
