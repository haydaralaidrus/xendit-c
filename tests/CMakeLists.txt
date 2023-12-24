## ./tests CMake file
###############################################################################

## Test executables
set(
	XND_TESTS
	strings http_request xendit balance
)

## Iterate test executables, add to test
foreach(TEST ${XND_TESTS})
	add_executable(${TEST} ${TEST}.c)
	target_link_libraries(${TEST} ${XND_STATIC_LIBRARY})
	add_test(${TEST} ${TEST})
endforeach()
