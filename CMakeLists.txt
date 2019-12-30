cmake_minimum_required(VERSION 3.9)
project(isesolRTW)

unset(source_files)

set(CMAKE_CXX_STANDARD 11)

ADD_DEFINITIONS(-DJSON_IS_AMALGAMATION)

set(srcdirs
        json/
        .)

include_directories(
        .
)

foreach(srcdir ${srcdirs})
    aux_source_directory(${srcdir} source_files)
endforeach()
#target_link_libraries(isesol)

add_library(isesolRTW SHARED ${source_files})