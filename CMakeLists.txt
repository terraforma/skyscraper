CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(skyscraper)

# Find source files
FILE(GLOB_RECURSE SKYSCRAPER_HEADERS include/*.h)
FILE(GLOB_RECURSE SKYSCRAPER_SOURCES src/*.cpp)

# Set libraries and include paths
SET(LIBS terra)
SET(INCLUDE ${PROJECT_SOURCE_DIR}/include)

SET(LIBRARY_OUTPUT_PATH lib/)
SET(EXECUTABLE_OUTPUT_PATH bin/)

# Define executable
ADD_EXECUTABLE(skyscraper ${SKYSCRAPER_SOURCES})
INCLUDE_DIRECTORIES(${INCLUDE})
TARGET_LINK_LIBRARIES(skyscraper ${LIBS})
