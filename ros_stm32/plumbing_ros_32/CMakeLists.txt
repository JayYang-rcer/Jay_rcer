cmake_minimum_required(VERSION 3.0.2)
project(plumbing_ros_32)



find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
  message_generation
)



###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
include
  ${catkin_INCLUDE_DIRS}
)

## 配置 msg 源文件
add_message_files(
  FILES
  r2_controller.msg
)

# 生成消息时依赖于 std_msgs
generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES plumbing_ros_32
 CATKIN_DEPENDS roscpp rospy std_msgs geometry_msgs message_runtime 
#  DEPENDS system_lib
)

add_library(head_src
  include/${PROJECT_NAME}/communica.h
  src/communica.cpp
)

add_dependencies(head_src ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(head_src
  ${catkin_LIBRARIES}
)


add_executable(read32 src/read32.cpp)
add_dependencies(read32 ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(read32
  head_src
  ${catkin_LIBRARIES}
)

add_executable(write32 src/write32.cpp)
add_dependencies(write32 ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(write32
  head_src
  ${catkin_LIBRARIES}
)

