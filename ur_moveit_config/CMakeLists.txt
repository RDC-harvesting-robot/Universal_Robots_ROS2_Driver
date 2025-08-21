cmake_minimum_required(VERSION 3.5)
project(ur_moveit_config)

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

add_executable(joint_states_manager ur_moveit_config/joint_states_manager.cpp)
target_include_directories(joint_states_manager PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

ament_target_dependencies(joint_states_manager
  rclcpp
  sensor_msgs  
)
install(TARGETS
  joint_states_manager
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY config launch rviz srdf
  DESTINATION share/${PROJECT_NAME}
)

# Install Python modules
ament_python_install_package(${PROJECT_NAME})
ament_python_install_module(${PROJECT_NAME}/launch_common.py)

ament_package()
