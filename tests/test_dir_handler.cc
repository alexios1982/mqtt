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

  std::cout << "\nListing all mp4 files" << '\n';
  if( dir_handler.exists() )
    dir_handler.list(".mp4");

  std::cout << "\nlisting all mp4 files in time order" << '\n';
  if(dir_handler.exists() )
    dir_handler.time_ordered_list(".mp4");

  std::cout << "\nlisting all mp4 files in reverse time order" << '\n';
  if(dir_handler.exists() )
    dir_handler.time_ordered_list(".mp4", Dir_handler::REVERSE);

  std::cout << "\nRetrieve the last modified mp4 file" << '\n';
  if(dir_handler.exists() )
    std::cout << ( dir_handler.get_last_modified_file(".mp4").second).string() << '\n';

  std::cout << "'\n Retrieve the list of txt files" << '\n';
  if( dir_handler.exists() ){
    const Dir_handler::Files_list &files_list = dir_handler.get_files_list(".txt");
    for(const auto& item: files_list)
      std::cout << item.string() << '\n';
  }
  return 0;
}
