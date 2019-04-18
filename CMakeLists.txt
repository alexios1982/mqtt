project(subscriber_zigbeetomqtt)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
# Set build type if not set.
if(NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release)
endif ()
# Set additional flags.
set( CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic-errors -std=c++11") #pedantic-errors is to avoid compile exetensions
set( CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set( CMAKE_CXX_FLAGS_RELEASE "-O3")


#section to define executable
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#message(STATUS "Output path ${EXECUTABLE_OUTPUT_PATH}")
#to be able to use the src's include file
include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(subscriber_zigbeetomqtt src/subscriber_zigbeetomqtt.cc)

#section to include external libraries to compile
#to tell cmake to compile the library using its own CMakeLists.txt file
add_subdirectory(${PROJECT_SOURCE_DIR}/lib)
#to be able to use the library's include file
include_directories(${PROJECT_SOURCE_DIR}/lib)
############################################################

# #######################################################################
# #to be able to compile gstreamer apps
# find_package(PkgConfig)
# pkg_check_modules(GST REQUIRED gstreamer-1.0>=1.4)
# target_include_directories(multifiles_saving PUBLIC ${GST_INCLUDE_DIRS})
# target_compile_options(multifiles_saving PUBLIC ${GST_CFLAGS})
# #######################################################################

#for mqtt libraries
find_library(MQTT_LIB
  NAMES paho-mqttpp3 paho-mqtt3a
  )#these libraries are in /usr/local/lib and so it is not necessary to specify the third argument 
   #to see the predefined linker paths: g++ -v -x c -E /dev/null (section LIBRARY_PATH)
#find_path(MQTT_INC
#  NAMES include/
# /where_to_find_1 /where_to_find_ 2   #this rule is not necessary beacuse mqtt include files
                                       #are in the predefined /usr/local path
				       #to se the predefined include path: g++ -v -x c -E /dev/null (section #include) 
###################################

#to link the lib to the executable
target_link_libraries(subscriber_zigbeetomqtt PUBLIC
  #${GST_LIBRARIES}
  Utils
  ${MQTT_LIB})

