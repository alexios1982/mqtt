// read a file into memory
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include "base64.hh"
#include <string>
#include "Utils.hh"

int main () {
  Time_spent<>();
  std::string base64_converted = base64_file_converter("../temp_saved_file_dir/2019-04-30_14:46:22_cam02_5.mp4");
  std::string decoded_string = base64_decode(base64_converted);
  std::ofstream outfile ("../temp_saved_file_dir/coded_and_decoded_2019-04-30_14:46:22_cam02_5.mp4", std::ofstream::binary);
  outfile.write ( (char*)decoded_string.c_str(), decoded_string.size() );    

  return 0;
}

