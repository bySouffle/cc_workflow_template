cmake_minimum_required(VERSION 3.16)
set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "build type")
set(CMAKE_SKIP_RPATH TRUE)
project(template
        VERSION 0.1.0
        LANGUAGES C CXX)

if (CYGWIN)
    message(FATAL_ERROR "Sorry, DO NOT support Cygwin")
endif ()

if (MINGW)
    message(FATAL_ERROR "Sorry, DO NOT support MinGW")
endif ()

#### CHECK

include(CheckIncludeFile)
include(CheckIncludeFileCXX)

include(GNUInstallDirs)

message("CMAKE_C_FLAGS_DEBUG is ${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_C_FLAGS_RELEASE is ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_C_FLAGS_RELWITHDEBINFO is ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message("CMAKE_C_FLAGS_MINSIZEREL is ${CMAKE_C_FLAGS_MINSIZEREL}")

message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS_RELWITHDEBINFO is ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message("CMAKE_CXX_FLAGS_MINSIZEREL is ${CMAKE_CXX_FLAGS_MINSIZEREL}")

if (WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}   /MP /wd4200")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /wd4200 /std:c++17")
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}   -Wall -fPIC -pipe -std=gnu90 -O0")
    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -std=c++17 -fno-exceptions -Wno-invalid-offsetof")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -std=c++17 -Wno-invalid-offsetof -O0")
endif ()
#### PREPARE

set(CMAKE_CXX_STANDARD 17)

find_package(wfrest REQUIRED CONFIG HINTS ..)
include_directories(
        ${WORKFLOW_INCLUDE_DIR}
        ${WFREST_INCLUDE_DIR}
)
find_package(spdlog REQUIRED)


find_package(Protobuf CONFIG REQUIRED)
set(CMAKE_PREFIX_PATH "/home/siasun/.local/lib/cmake/grpc" ${CMAKE_PREFIX_PATH})
find_package(gRPC REQUIRED)

find_package(opentelemetry-cpp CONFIG REQUIRED)
message("opentelemetry-cpp include: ${OPENTELEMETRY_CPP_INCLUDE_DIR}")
message("opentelemetry-cpp lib: ${OPENTELEMETRY_CPP_LIBRARIES}")

include_directories(
        ${OPENTELEMETRY_CPP_INCLUDE_DIR}

)
include_directories(
        app
        pkg
)

file(GLOB_RECURSE INC
        pkg/*.h
        pkg/*.cc
)


file(GLOB_RECURSE TemplateSrc
        app/template/internal/*.h
        app/template/internal/*.cc
)

file(GLOB_RECURSE Template
        app/template/cmd/*.h
        app/template/cmd/*.cc
)

add_executable(${PROJECT_NAME} ${TemplateSrc} ${INC} ${Template})
target_link_directories(${PROJECT_NAME} PRIVATE ${OPENTELEMETRY_CPP_LIBS})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        wfrest
        workflow
        pthread
        yaml-cpp
        spdlog
        ${OPENTELEMETRY_CPP_LIBRARIES}
)
