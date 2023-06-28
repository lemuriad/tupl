file (GLOB TEST_SRC "*.cpp")

# tupl API test suites
compile_all("true" "tupl" "tupl/tests" "${TEST_SRC}")

