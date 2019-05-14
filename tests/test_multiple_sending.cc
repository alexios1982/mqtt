#include <iomanip>      // std::get_time
#include <ctime>        // struct std::tm
#include <sstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp> //for boost filter iterator
#include <map>
#include "Utils.hh"

int main(){
  Dir_handler dir_handler{"/home/pi/gstreamer/multifiles_saving"};
  if( dir_handler.exists() )
    dir_handler.list(".mp4");

  std::cout << "\nlisting only some files in time order" << std::endl;
  if(dir_handler.exists() )
    dir_handler.time_ordered_list(".mp4");


  std::cout << "\nretrieve the last modified file" << std::endl;
  if(dir_handler.exists() )
    std::cout << dir_handler.get_last_modified_file(".mp4").second << std::endl;


  return 0;
}
