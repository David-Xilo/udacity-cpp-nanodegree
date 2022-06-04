cmake_minimum_required(VERSION 3.11.3)

set(CMAKE_CXX_STANDARD 17)

project(Membot)

# to configure wxWidgets on clion use: https://forums.wxwidgets.org/viewtopic.php?t=45198
# after downloading and installing wxWidgets here: http://www.wxwidgets.org/downloads/
# these lines are not necessary
#set(wxWidgets_ROOT_DIR /home/david/Desktop/Random/Software/wxWidgets/wxWidgets-3.1.5/buildqt)
#set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

file(GLOB project_SRCS src/*.cpp)

add_executable(membot ${project_SRCS})
target_link_libraries(membot ${wxWidgets_LIBRARIES})
target_include_directories(membot PRIVATE ${wxWidgets_INCLUDE_DIRS})
