#find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (${PROJECT_SOURCE_DIR}/lib
  ${PROJECT_SOURCE_DIR}/tests
#  ${Boost_INCLUDE_DIRS}
)

############################################################################################
#for mqtt libraries
find_library(MQTT_LIB
  NAMES paho-mqttpp3 paho-mqtt3a #we provide two names because these paho-mqtt may have different names in different systems
  )#these libraries are in /usr/local/lib and so it is not necessary to specify the third argument 
   #to see the predefined linker paths: g++ -v -x c -E /dev/null (section LIBRARY_PATH)
#find_path(MQTT_INC
#  NAMES include/
# /where_to_find_1 /where_to_find_ 2   #this rule is not necessary beacuse mqtt include files
                                       #are in the predefined /usr/local path
				       #to se the predefined include path: g++ -v -x c -E /dev/null (section #include)
######################################################################################################


#add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable (test_base64 test_base64.cc)
target_link_libraries (test_base64
  base64
  Utils
#  ${Boost_FILESYSTEM_LIBRARY}
#  ${Boost_SYSTEM_LIBRARY}
#  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

# ################################################################
# #### for boost/filesystem #############
# find_library(BOOST_FILESYSTEM
#   NAMES boost_filesystem
#   ) #these libraries are in /usr/local/lib and so it is not necessary to specify the third argument 
# # ################################################################

# ################################################################
# ####for boost_system library needed by boost/filesystem
# find_library(BOOST_SYSTEM
#   NAMES boost_system
#   ) #these libraries are in /usr/local/lib and so it is not necessary to specify the third argument 
# # ################################################################

add_executable (test_dir_handler test_dir_handler.cc)

#BOOST_FILESYSTEM and BOOST_SYSTEM are defined in Utils CMakelists
target_link_libraries(test_dir_handler PUBLIC
  Utils
  ${BOOST_FILESYSTEM}
  ${BOOST_SYSTEM}
  )

add_executable (test_single_sending test_single_sending.cc)
target_link_libraries(test_single_sending PUBLIC
  Utils
  base64
  ${MQTT_LIB}
  )
  
add_executable(test_multiple_sending test_multiple_sending.cc)
target_link_libraries(test_multiple_sending PUBLIC
  Utils
  base64
  ${MQTT_LIB}
  ${BOOST_FILESYSTEM}
  ${BOOST_SYSTEM}
  )

add_executable(test_sensors test_sensors.cc)
target_link_libraries(test_sensors PUBLIC
  ${MQTT_LIB}
  ${BOOST_SYSTEM}
  )

add_executable(test_amazon_broker test_amazon_broker.cc)
target_link_libraries(test_amazon_broker PUBLIC
  ${MQTT_LIB}
  ${BOOST_SYSTEM}
)  

add_executable (test_amazon_single_sending test_amazon_single_sending.cc)
target_link_libraries(test_amazon_single_sending PUBLIC
  Utils
  base64
  ${MQTT_LIB}
  )

#######################################################################
#to be able to compile opencv
find_package(OpenCV REQUIRED)
#######################################################################

add_executable (test_image_extraction_with_opencv test_image_extraction_with_opencv.cc)
target_include_directories(test_image_extraction_with_opencv PUBLIC ${OpenCV_INCLUDE_DIRS})
target_compile_options(test_image_extraction_with_opencv PUBLIC ${OpenCV_CFLAGS})
target_link_libraries(test_image_extraction_with_opencv PUBLIC
  Utils
  ${OpenCV_LIBS}
  )


add_executable (test_alarm_system_state_machine test_alarm_system_state_machine.cc
  ../src/Alarm_system_state_machine.cc
  )

target_link_libraries(test_alarm_system_state_machine PUBLIC
  Utils
  ${BOOST_SYSTEM}
  )

add_executable (test_configuration_file test_configuration_file.cc)
