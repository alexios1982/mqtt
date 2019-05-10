#include <iomanip>      // std::get_time
#include <ctime>        // struct std::tm
#include <sstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp> //for boost filter iterator

constexpr int DOTMP4_SUB_STRING_FROM_END = 4;
using namespace boost::filesystem;

//note that the filename includes the path
//in which it is located, so I cannot refer to the length of its name
struct Is_mp4_file{
  bool operator() (const path &path){
    std::string string_path = path.string();
    std::string dot_mp4 = string_path.substr(string_path.length() - DOTMP4_SUB_STRING_FROM_END);
    return dot_mp4 == ".mp4";
  }
};

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

  boost::system::error_code ec;
  path p {"/home/pi/gstreamer/multifiles_saving"};
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

  while(filter_iter_first != filter_iter_last){
    std::cout << *filter_iter_first++ << '\n';
  }
  return 0;
}
