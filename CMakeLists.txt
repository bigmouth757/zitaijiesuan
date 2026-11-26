cmake_minimum_required (VERSION 3.14)




project ("zitaijiesuan")



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(OpenCV_DIR C:/Users/omen/Desktop/opencv/build)
find_package(OpenCV REQUIRED COMPONENTS calib3d core features2d flann gapi highgui imgcodecs imgproc world )
get_target_property(_dll_dbg opencv_world IMPORTED_LOCATION_DEBUG)
 get_target_property(_dll_release opencv_world IMPORTED_LOCATION_RELEASE)
add_executable (zitaijiesuan  "core.h" "ColorMask.cpp" "findlight.cpp" "matcharmor.cpp" "draw.cpp" "oneframedetect.cpp" "deviation.cpp" "main.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR} ${OpenCV_INCLUDE_DIRS})


target_link_libraries(zitaijiesuan ${OpenCV_LIBS})

