#include <iomanip>      // std::get_time
#include <ctime>        // struct std::tm
#include <sstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp> //for boost filter iterator
#include <map>
#include "Utils.hh"

constexpr int DOTMP4_SUB_STRING_FROM_END = 4;
using namespace boost::filesystem;

typedef std::multimap<std::time_t, path> time_ord_mmap;
//note that the filename includes the path
//in which it is located, so I cannot refer to the length of its name
struct Is_mp4_file{
  bool operator() (const path &path){
    std::string string_path = path.string();
    std::string dot_mp4 = string_path.substr(string_path.length() - DOTMP4_SUB_STRING_FROM_END);
    return dot_mp4 == ".mp4";
  }
};

time_ord_mmap::value_type get_last_mp4_file(const std::string &string_path){

  boost::system::error_code ec;
  path p {string_path};
  //let's verify that the directory exists
  //path constructor doesn't verify that the string passed
  //as argument contains a valid path
  try{
    //to do some check we need the file_status object
    file_status s = status(p);
    std::cout << std::boolalpha << exists(s) << '\n';
  }
  catch (filesystem_error &e){
      std::cerr << e.what() << '\n';
  }

  typedef directory_iterator base_iterator;
  typedef boost::filter_iterator<Is_mp4_file, base_iterator>
    FilterIter;

  Is_mp4_file predicate;

  base_iterator beginning{p};
  base_iterator end{};

  FilterIter filter_iter_first(predicate, beginning, end);
  FilterIter filter_iter_last(predicate, end, end);

  time_ord_mmap time_ordered_map;
  while(filter_iter_first != filter_iter_last){
    // std::cout << *filter_iter_first << '\n';
    std::time_t t = last_write_time( *filter_iter_first ) ;
    // std::cout << "On " << std::ctime( &t ) << " the file " << *filter_iter_first 
    // 	      << " was modified the last time!\n" ;
    time_ordered_map.insert( time_ord_mmap::value_type(t, *filter_iter_first) );
    ++filter_iter_first;
  }
  for (time_ord_mmap::iterator it =
	 time_ordered_map.begin();
       it != time_ordered_map.end();
       ++it)
    std::cout << (*it).second << " => " << std::ctime( &(*it).first ) << '\n';
  return *( --time_ordered_map.end() );
}

bool is_a_more_recent_mp4(const time_ord_mmap::value_type &current_value,
			  const time_ord_mmap::value_type &to_compare){
  std::string curr_filename{current_value.second.string()};
  return ( curr_filename.substr(curr_filename.length() - DOTMP4_SUB_STRING_FROM_END) == ".mp4" ) &&
    current_value.first > to_compare.first;
}
 int main(){
#if 0
  std::string example_string("2019-04-30_14:46:22_cam02_5.mp4");
  std::string time_substring = example_string.substr(11, 8);
  std::cout << "time_string: " << time_substring << std::endl;
  struct std::tm when;
  std::istringstream ss(time_substring);
  ss >> std::get_time(&when, "%H:%M:%S"); // or just %T in this case
  std::cout << when.tm_hour << " hours and " << when.tm_min << " minutes\n";
#endif
  
#if 0
  time_ord_mmap::value_type to_send = get_last_mp4_file("/home/pi/gstreamer/multifiles_saving");
  std::cout << "the last modififed file is: " << to_send.second << " => " << std::ctime( &(to_send.first) ) << '\n';


  while( !is_a_more_recent_mp4( get_last_mp4_file("/home/pi/gstreamer/multifiles_saving"),
				to_send) );

  std::cout << "il file da inviare e:' " << to_send.second.string() << std::endl;
#endif

  Dir_handler dir_handler{"/home/pi/gstreamer/multifiles_saving"};
  if( dir_handler.exists() )
    dir_handler.lists();

  std::cout << "\nlisting only some files" << std::endl;
  if(dir_handler.exists() ){
    dir_handler.get_last_modified_file(""); 
  }
  return 0;
}
