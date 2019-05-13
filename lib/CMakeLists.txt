cmake_minimum_required(VERSION 3.1)
add_library(Utils Utils.cc)
add_library(base64 base64.cc)

################################################################
#### for boost/filesystem #############
find_library(BOOST_FILESYSTEM
  NAMES boost_filesystem
  ) #these libraries are in /usr/local/lib and so it is not necessary to specify the third argument 
# ################################################################

################################################################
####for boost_system library needed by boost/filesystem
find_library(BOOST_SYSTEM
  NAMES boost_system
  ) #these libraries are in /usr/local/lib and so it is not necessary to specify the third argument 
# ################################################################

target_link_libraries(Utils PUBLIC
  ${BOOST_FILESYSTEM}
  ${BOOST_SYSTEM}
  )
