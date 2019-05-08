#include "base64.hh"
#include <iostream>
#include <fstream> 
#include <memory> //for unique_ptr
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <sstream>

static const char* B64chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const int B64index[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  62, 63, 62, 62, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0,  0,  0,
    0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,  0,  0,  0,  63,
    0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

static const std::string base64_padding[] = {"", "==","="};

const std::string base64_encode(const void* data, const size_t &len){
  std::string result((len + 2) / 3 * 4, '=');
  char *p = (char*) data, *str = &result[0];
  size_t j = 0, pad = len % 3;
  const size_t last = len - pad;

  for (size_t i = 0; i < last; i += 3)
    {
      int n = int(p[i]) << 16 | int(p[i + 1]) << 8 | p[i + 2];
      str[j++] = B64chars[n >> 18];
      str[j++] = B64chars[n >> 12 & 0x3F];
      str[j++] = B64chars[n >> 6 & 0x3F];
      str[j++] = B64chars[n & 0x3F];
    }
  if (pad)  /// set padding
    {
      int n = --pad ? int(p[last]) << 8 | p[last + 1] : p[last];
      str[j++] = B64chars[pad ? n >> 10 & 0x3F : n >> 2];
      str[j++] = B64chars[pad ? n >> 4 & 0x03F : n << 4 & 0x3F];
      str[j++] = pad ? B64chars[n << 2 & 0x3F] : '=';
    }
  return result;
}

const std::string broken_lines_base64_encode(const unsigned char *data,
					     const size_t &len){
    using namespace boost::archive::iterators;
    std::stringstream os;
    typedef
      insert_linebreaks<         // insert line breaks every 76 characters
	base64_from_binary<    // convert binary values to base64 characters
	  transform_width<   // retrieve 6 bit integers from a sequence of 8 bit bytes
	    const char *,
	    6,
	    8
	    >
	  >
      ,76
      >
      base64_text; // compose all the above operations in to a new iterator
  
    std::copy(
	      base64_text(data),
	      base64_text(data + len),
	      ostream_iterator<char>(os)
	      );
    //let's add padding with operator "=" if the len is not multiple of three
    os << base64_padding[len % 3];
    return os.str();
}

const std::string base64_decode(const void* data, const size_t &len){
  if (len == 0) return "";

  unsigned char *p = (unsigned char*) data;
  size_t j = 0,
    pad1 = len % 4 || p[len - 1] == '=',
    pad2 = pad1 && (len % 4 > 2 || p[len - 2] != '=');
  const size_t last = (len - pad1) / 4 << 2;
  std::string result(last / 4 * 3 + pad1 + pad2, '\0');
  unsigned char *str = (unsigned char*) &result[0];

  for (size_t i = 0; i < last; i += 4)
    {
      int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
      str[j++] = n >> 16;
      str[j++] = n >> 8 & 0xFF;
      str[j++] = n & 0xFF;
    }
  if (pad1)
    {
      int n = B64index[p[last]] << 18 | B64index[p[last + 1]] << 12;
      str[j++] = n >> 16;
      if (pad2)
	{
	  n |= B64index[p[last + 2]] << 6;
	  str[j++] = n >> 8 & 0xFF;
	}
    }
  return result;
}

std::string base64_encode(const std::string &str){
  return base64_encode(str.c_str(), str.size());
}

std::string base64_decode(const std::string &str64)
{
  return base64_decode(str64.c_str(), str64.size());
}

std::string base64_file_converter(const std::string &file_path,
				  Encoding_type encoding_type){
  std::ifstream is (file_path.c_str(), std::ifstream::binary | std::ios::ate);
  if (is) {
    // get length of file:
    //is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);

    std::unique_ptr<char> buffer (new char [length]);

    //std::cout << "Reading " << length << " characters... ";
    // read data as a block:
    is.read (buffer.get(), length);

    // if (is)
    //   std::cout << "all characters read successfully.\n";
    // else
    if(!is)
      std::cerr<<"[base_64::"<<__func__<<"]"<<". error: only " << is.gcount() << " could be read";
    is.close();
    
    switch (encoding_type){
    case NO_BROKEN_LINES:
      return base64_encode (reinterpret_cast<const unsigned char*>( buffer.get() ), length);
    case BROKEN_LINES:
      return broken_lines_base64_encode (reinterpret_cast<const unsigned char*>( buffer.get() ), length);
    default:
      std::cerr << "[base::" << __func__ << "]. invalid encoding type value" << std::endl;
    }
  }
  else
    std::cerr<<"[base_64::" << __func__<< "]" << ". can't open file to convert" << std::endl;
  return std::string("");
}
