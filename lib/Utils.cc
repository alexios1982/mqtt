#include"Utils.hh"

Advance_debug::Modifier warning(Advance_debug::FG_LIGHT_YELLOW);
Advance_debug::Modifier error(Advance_debug::FG_RED);
Advance_debug::Modifier info(Advance_debug::FG_CYAN);
Advance_debug::Modifier reset(Advance_debug::FG_DEFAULT);

Dir_handler::Dir_handler(const std::string & path,
			 const int FILES_NUMBER): _path{path},
						  _FILES_NUMBER{FILES_NUMBER}{
						    _files_list.reserve(FILES_NUMBER);
						  }

bool Dir_handler::exists() const { return bf::exists(_path); }

void Dir_handler::fill_files_list(const std::string &extension){
  // bf::directory_iterator beginning{_path};
  // bf::directory_iterator end{};

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

  while(first != last)
    _files_list.push_back(*first++);
}

void Dir_handler::list(const std::string &extension){
  //every time I need to  call this method to refresh the
  //file list, because folder's content may be changed in the meantime
  fill_files_list(extension);
  for(const auto &item: _files_list)
    std::cout << item << '\n';
}

void
Dir_handler::fill_time_ord_mmap(const std::string &extension) {
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

      while(first != last){
	//std::cout << *first << '\n';
	std::time_t t = bf::last_write_time( *first ) ;
	//std::cout << "On " << std::ctime( &t ) << " the file " << *first 
	//	  << " was modified the last time!\n" ;
	_time_ord_mmap.insert( Time_ord_mmap::value_type(t, *first) );
	++first;
      }     
      //std::cout << "[Dir_handler::" << __func<< "]" << "no files with specified pattern" << std::endl;
      //return std::pair<const std::time_t, std::string> (std::time_t{}, "");
}
  
Dir_handler::Return_type
Dir_handler::get_last_modified_file(const std::string &extension){
  //every time I need to  call this method to refresh the
  //map, because folder's content may be changed in the meantime
  fill_time_ord_mmap(extension);
  if( _time_ord_mmap.size() ){
    //std::cout << "[Dir_handler::" << __func__ << "]. Map size: " << _time_ord_mmap.size() << '\n';
    //let's retrieve the iter to the last occurrence
    auto iter =  --( _time_ord_mmap.end() );
    return Return_type( iter->first, (iter->second).string() );
  }
  else
    return Return_type{std::time_t{}, ""};
}

void Dir_handler::time_ordered_list(const std::string &extension){
  //every time I need to  call this method to refresh the
  //map, because folder's content may be changed in the meantime
  fill_time_ord_mmap(extension);

  for(const auto &item: _time_ord_mmap)
    std::cout << item.second << '\n' ;
}
