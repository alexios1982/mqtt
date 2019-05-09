#find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (${PROJECT_SOURCE_DIR}/lib
  ${PROJECT_SOURCE_DIR}/tests
#  ${Boost_INCLUDE_DIRS}
)

############################################################################################
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
add_executable (test_time_conversion test_string_to_time_conversion.cc)
add_executable (test_single_sending test_single_sending.cc)
target_link_libraries(test_single_sending PUBLIC
  Utils
  base64
  ${MQTT_LIB}
  )
  
  
