cmake_minimum_required(VERSION 2.8.3)
project(gsoc_example_project_demo)

find_package(catkin REQUIRED
  roscpp
  unreal_vis_msgs
  message_generation
)

include_directories(
 include
 ${catkin_INCLUDE_DIRS}
)

catkin_package(
)

add_executable(uvis_project_demo src/UVisExample.cpp)
target_link_libraries(uvis_project_demo ${catkin_LIBRARIES})
add_dependencies(uvis_project_demo ${PROJECT_NAME}_generate_messages_cpp )
