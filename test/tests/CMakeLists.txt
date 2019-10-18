include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(VPTTests QuickSelectTest.cpp VPTTest.cpp)

target_link_libraries(VPTTests gtest gtest_main)
target_link_libraries(VPTTests VPT_LIB pthread sodium)