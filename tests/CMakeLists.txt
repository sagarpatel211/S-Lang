#Using GoogleTest for unit testing
include(FetchContent) FetchContent_Declare(googletest GIT_REPOSITORY https
                                           : // github.com/google/googletest.git
                                           GIT_TAG release - 1.12.1)
    set(gtest_force_shared_crt ON CACHE BOOL
        "" FORCE) FetchContent_MakeAvailable(googletest) include(GoogleTest)

#Lexer tests
        add_executable(test_lexer test_lexer.cpp) target_link_libraries(
            test_lexer PRIVATE GTest::gtest_main PRIVATE Lexer)
            target_include_directories(test_lexer PRIVATE "${PROJECT_SOURCE_DIR}/include")
                gtest_discover_tests(test_lexer)

#Parser tests
                    add_executable(test_parser test_parser.cpp) target_link_libraries(
                        test_parser PRIVATE GTest::gtest_main PRIVATE Parser)
                        target_include_directories(test_parser PRIVATE
                                                   "${PROJECT_SOURCE_DIR}/include")
                            gtest_discover_tests(test_parser)

#Set the output directory for our tests
                                set_target_properties(
                                    test_lexer PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                    "${PROJECT_BINARY_DIR}/bin")
                                    set_target_properties(
                                        test_parser PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                        "${PROJECT_BINARY_DIR}/bin")