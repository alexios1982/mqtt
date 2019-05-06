//
//  base64 encoding and decoding with C++.
//

#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <string>

const std::string base64_encode(const void* data, const size_t &len);
const std::string base64_decode(const void* data, const size_t &len);
std::string base64_encode(const std::string &str);
std::string base64_decode(const std::string &str64);
std::string base64_file_converter(const std::string &file_path);

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */
