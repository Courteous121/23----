cmake_minimum_required(VERSION 3.0.2)
project(all)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  visualization_msgs
cv_bridge
  image_transport
sensor_msgs
  )


catkin_package(

)


include_directories(

  ${catkin_INCLUDE_DIRS}
)

find_package(OpenCV REQUIRED)

add_executable (ali src/all.cpp)


include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(ali ${OpenCV_LIBS} ${catkin_LIBRARIES})




