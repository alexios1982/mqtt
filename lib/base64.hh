//
//  base64 encoding and decoding with C++.
//

#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <string>

enum Encoding_type{ NO_BROKEN_LINES, BROKEN_LINES};
  
const std::string base64_encode(const void* data, const size_t &len);
const std::string base64_decode(const void* data, const size_t &len);
const std::string broken_lines_base64_encode(const unsigned char *data, const size_t &len); 

std::string base64_encode(const std::string &str);
std::string base64_decode(const std::string &str64);
std::string base64_file_converter(const std::string &file_path, Encoding_type encoding_type_=NO_BROKEN_LINES);

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */
