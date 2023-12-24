## ./src CMake file
###############################################################################

## Build Xendit SDK static library
add_library(
	${XND_STATIC_LIBRARY}
	STATIC strings.c http_request.c xendit.c balance.c
)

## Include paths
target_include_directories(
	${XND_STATIC_LIBRARY}
	PUBLIC  ${XND_INCLUDE_DIRECTORY} ${XND_SRC_DIRECTORY}
	PRIVATE ${CURL_INCLUDE_DIR} ${JSON-C_INCLUDE_DIRS}
)

## Link depended libraries
target_link_libraries(
	${XND_STATIC_LIBRARY}
	PRIVATE ${CURL_LIBRARIES} json-c
)

## Install
install(
	TARGETS ${XND_STATIC_LIBRARY}
	ARCHIVE DESTINATION lib
)
