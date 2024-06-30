# CMakeList.txt: gapengine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("gapengine")
file(GLOB_RECURSE MAIN_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

find_package(fmt REQUIRED)
find_package(glm REQUIRED)
find_package(boost_algorithm)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(mstch REQUIRED)
find_package(magic_enum REQUIRED)
find_package(Boost 1.85.0 COMPONENTS program_options REQUIRED)
find_package(Boost 1.85.0 COMPONENTS log REQUIRED)
find_package(Boost 1.85.0 COMPONENTS log_setup REQUIRED)
find_package(glad REQUIRED)
find_package(lua REQUIRED)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
add_compile_definitions(_USE_MATH_DEFINES)
add_compile_definitions(GLFW_INCLUDE_NONE)


# 将源代码添加到此项目的可执行文件。
add_executable (${PROJECT_NAME} ${MAIN_SRC} ${PROJECT_SOURCE_DIR}/gapengine.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC
glm::glm fmt::fmt glfw assimp::assimp Boost::program_options Boost::log_setup Boost::log nlohmann_json mstch::mstch glfw magic_enum::magic_enum  glad::glad
lua
)


target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

file(COPY ${PROJECT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/src/config.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET gapengine PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
