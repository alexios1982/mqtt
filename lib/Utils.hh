#ifndef _UTILS_
#define _UTILS_

#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif

#include <ostream>
namespace Advance_debug {
  enum Code {
    FG_DEFAULT = 39, 
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_LIGHT_GRAY = 37,
    FG_DARK_GRAY = 90,
    FG_LIGHT_RED = 91,
    FG_LIGHT_GREEN = 92,
    FG_LIGHT_YELLOW = 93,
    FG_LIGHT_BLUE = 94,
    FG_LIGHT_MAGENTA = 95,
    FG_LIGHT_CYAN = 96,
    FG_WHITE = 97,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_BLUE = 44,
    BG_DEFAULT = 49
  };
  class Modifier {
    Code code;
  public:
    Modifier(Code pCode) : code(pCode) {}
    friend std::ostream&
    operator<<(std::ostream& os, const Modifier& mod) {
      return os << "\033[" << mod.code << "m";
    }
  };
}
extern Advance_debug::Modifier warning;
extern Advance_debug::Modifier error;
extern Advance_debug::Modifier info;
extern Advance_debug::Modifier reset;

#include<chrono>
#include<iostream>

using namespace std::chrono;

//this class provides spent time measurement
//example
//Time_spent<std::micro> time_spent;
//std::nano	
//std::micro	
//std::milli	
//std::centi	
//std::deci
template<class T = std::milli>
class Time_spent{
  steady_clock::time_point start;
  steady_clock::time_point end;
public:
  Time_spent():start( steady_clock::now() ){}
  ~Time_spent(){
    end = steady_clock::now();
    auto diff = end - start;
    std::cout << std::chrono::duration <double, T> (diff).count() << " ms" << std::endl;
  }
};

#include <ctime>        // struct std::tm
#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp> //for boost filter iterator
#include <map>
#include <string>

namespace bf = boost::filesystem;

class Dir_handler{
  //returns true if the complete file name matches
  //the exentension passed as second parameter
  struct Match_file_extension{
    bool operator() (const bf::path &path,
		     const std::string &extension) const{
      std::string string_path = path.string();
      std::string string_extension = string_path.substr(string_path.length() - extension.size() );
    return string_extension == extension;
    }
    //this typedef must be defined after the operator() definition
    //and are necessary for working with std::bind
    typedef bf::path first_argument_type;
    typedef std::string second_argument_type;
    typedef bool result_type;
};

  typedef std::multimap<std::time_t, bf::path> Time_ord_mmap;

  //I need to define Files_list and Time_string_pair public
  //because they will be return values
  //I cannot move these definition more down,
  //because Files_list is the type of private member value
public:
  typedef std::vector<bf::path> Files_list;
  typedef std::pair<const std::time_t, bf::path> Time_path_pair;

  enum Order{
    NORMAL,
    REVERSE
  };
  
private:
  Time_ord_mmap _time_ord_mmap;
  Files_list _files_list;
  bf::path _path;
  const int _FILES_NUMBER;

  void fill_files_list(const std::string &extension);
  void fill_time_ord_mmap(const std::string &extension);
public:

  Dir_handler(const std::string &path, const int FILES_NUMBER=0);
  bool exists() const;
  void list(const std::string &extension = "");
  void time_ordered_list(const std::string &extension = "", Order = NORMAL);
  Time_path_pair
  get_last_modified_file(const std::string &extension = "");
  const Files_list&
  get_files_list(const std::string &extension = "");
};
#endif
