#include"Utils.hh"
#include <boost/range/adaptor/reversed.hpp>

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
  //first of all we need to empty _files_list if not empty
  if( _files_list.size() )
    _files_list.clear();
  
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
  //file list, because folder's content may be changed among method calls
  fill_files_list(extension);
  for(const auto &item: _files_list)
    std::cout << item << '\n';
}

const Dir_handler::Files_list& Dir_handler::get_files_list(const std::string &extension){
  //every time I need to  call this method to refresh the
  //file list, because folder's content may be changed among method calls
  fill_files_list(extension);
  return _files_list; 
}

void
Dir_handler::fill_time_ord_mmap(const std::string &extension) {
  //first of all we need to empty _time_ord_mmap if not empty
  // std::cout << "[Dir_handler::]" << __func__ 
  // 	    << ". Let's clear the previous map of mp4 files" << '\n';
  if( _time_ord_mmap.size() )
    _time_ord_mmap.clear();

  //let's construct the iterator to the beginning of list of files with
  //extension "extension" in the directory
  // std::cout << "[Dir_handler::]" << __func__ 
  // 	    << ". Let's construct the iterator to the beginning" << '\n';
  auto first = boost::make_filter_iterator(std::bind2nd(Match_file_extension(), extension),
					   bf::directory_iterator{_path},
					   bf::directory_iterator{}
					   );

  //let's construct the iterator to the end of list of files with
  //extension "extension" in the directory
  // std::cout << "[Dir_handler::]" << __func__ 
  // 	    << ". Let' s construct the iterator to the end" << '\n';
  auto last = boost::make_filter_iterator(std::bind2nd(Match_file_extension(), extension),
					  bf::directory_iterator{},
					  bf::directory_iterator{}
					  );
#ifdef UTILS_DEBUG
  while(first != last)
    std::cout << *first++ << '\n';
#endif

  // std::cout << "[Dir_handler::]" << __func__ 
  // 	    << ". Let's order" << '\n';

  while(first != last){
    //std::cout << *first << '\n';
    try{
      std::time_t t = bf::last_write_time( *first ) ;
      //std::cout << "On " << std::ctime( &t ) << " the file " << *first 
      //	  << " was modified the last time!\n" ;
      _time_ord_mmap.insert( Time_ord_mmap::value_type(t, *first) );
      ++first;
    }
    catch (bf::filesystem_error &e){
	std::cerr << "[Dir_handler::]" << __func__ 
		  << e.what() << '\n';
	//we need to increment first because otherwise we would have an infinite loop
	++first;
      }
  }
  // std::cout << "[Dir_handler::]" << __func__ 
  // 	    << ". Ordering ended" << std::endl;
}
  
Dir_handler::Time_path_pair
Dir_handler::get_last_modified_file(const std::string &extension){
  //every time I need to  call this method to refresh the
  //map, because folder's content may be changed among method calls
  fill_time_ord_mmap(extension);
  if( _time_ord_mmap.size() ){
    //std::cout << "[Dir_handler::" << __func__ << "]. Map size: " << _time_ord_mmap.size() << '\n';
    //let's retrieve the iter to the last occurrence
    auto iter =  --( _time_ord_mmap.end() );
    return Time_path_pair( iter->first, (iter->second) );
  }
  else
    //std::cout << "[Dir_handler::" << __func__ << "]. the map is empty!" << std::endl;
  return Time_path_pair{};
}

void Dir_handler::time_ordered_list(const std::string &extension, Order order){
  //every time I need to  call this method to refresh the
  //map, because folder's content may be changed among method calls
  fill_time_ord_mmap(extension);

  switch(order){
    case NORMAL:
      for(const auto &item: _time_ord_mmap)
	std::cout << item.second << '\n' ;
      break;
    case REVERSE:
      for( const auto &item: boost::adaptors::reverse(_time_ord_mmap) )
	std::cout << item.second << '\n' ;
      
    }
}
