cmake_minimum_required(VERSION 3.10.0)

set(EXECUTEABLE_NAME exe)
set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)


# for llvm and clang library
set(CMAKE_CXX_FLAGS "${LLVM_COMPILE_FLAGS}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

set(CONF_FILES files/entry_point.txt)

set(THIRD_SRC 
        thirdparty/cxxopts/include/cxxopts.hpp
)

file(GLOB CLANG_FRONT
        clang_fornt/*.h
        clang_fornt/*.cpp
)

file(GLOB LIB_SRC
        lib/*.h
        lib/*.cpp
)
set(STATIC_ANA_SRC
        ${THIRD_SRC}
        ${LIB_SRC}
        ${CLANG_FRONT}
        main.cpp
)

#copy all the configure files
configure_file(${CONF_FILES} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)

#export compile database
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DEXECUTEABLE_NAME=\"exe\")

# remove trailing whitespaces
string(STRIP ${LLVM_LDFLAGS} LLVM_LDFLAGS)
add_compile_options(-g -fexceptions -O0)
add_executable(${EXECUTEABLE_NAME} ${LIB_SRC} ${STATIC_ANA_SRC})

# target_compile_definitions(${EXECUTEABLE_NAME} PRIVATE EXECUTEABLE_NAME=${EXECUTEABLE_NAME})

target_link_libraries(${EXECUTEABLE_NAME} stdc++fs)
target_link_libraries(${EXECUTEABLE_NAME} glog)
target_link_libraries(${EXECUTEABLE_NAME} clang ${CLANG_LIBS} ${LLVM_LIBS_CORE} ${LLVM_LDFLAGS})

