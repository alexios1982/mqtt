#include"Utils.hh"

Advance_debug::Modifier warning(Advance_debug::FG_LIGHT_YELLOW);
Advance_debug::Modifier error(Advance_debug::FG_RED);
Advance_debug::Modifier info(Advance_debug::FG_CYAN);
Advance_debug::Modifier reset(Advance_debug::FG_DEFAULT);

Dir_handler::Dir_handler(const std::string & path): _path{path}{}

bool Dir_handler::exists() const { return bf::exists(_path); }

void Dir_handler::lists() const {
  bf::directory_iterator beginning{_path};
  bf::directory_iterator end{};

  while(beginning != end){
    std::cout << *beginning++ << '\n';
  }
}

std::pair<const std::time_t, std::string>
Dir_handler::get_last_modified_file(const std::string &extension){
  auto first = boost::make_filter_iterator(std::bind2nd(Match_file_extension(), extension),
					   bf::directory_iterator{_path},
					   bf::directory_iterator{}
					   );

  auto last = boost::make_filter_iterator(std::bind2nd(Match_file_extension(), extension),
					  bf::directory_iterator{},
					  bf::directory_iterator{}
					  );
#ifdef UTILS_DEBUG
  while(first != last)
    std::cout << *first++ << '\n';
#endif

  if(first != last{
      while(first != last){
	// std::cout << *first << '\n';
	std::time_t t = bf::last_write_time( *first ) ;
	// std::cout << "On " << std::ctime( &t ) << " the file " << *filter_iter_first 
	// 	      << " was modified the last time!\n" ;
	_time_ord_map.insert( Time_ord_mmap::value_type(t, *first) );
	++first;
      }
      
    }
    else{
      std::cout << "[Dir_handler::" << __func<< "]" << "no files with specified pattern" << std::endl;
      return std::pair<const std::time_t, std::string> (std::time_t{}, "");
    }
}
