# mteval frontend binaries

add_executable(mteval-corpus mteval-corpus.cc)
add_executable(mteval-sentence mteval-sentence.cc)
add_executable(mteval-pairwise mteval-pairwise.cc)

target_link_libraries(mteval-corpus mteval ${Boost_LIBRARIES})
target_link_libraries(mteval-sentence mteval ${Boost_LIBRARIES})
target_link_libraries(mteval-pairwise mteval ${Boost_LIBRARIES})

