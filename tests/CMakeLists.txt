#find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (${PROJECT_SOURCE_DIR}/lib
#  ${Boost_INCLUDE_DIRS}
  )
#add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable (test_base64 test_base64.cc)
target_link_libraries (test_base64
  base64
  Utils
#  ${Boost_FILESYSTEM_LIBRARY}
#  ${Boost_SYSTEM_LIBRARY}
#  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
add_executable (test_time_conversion test_time_conversion.cc)
  
  
