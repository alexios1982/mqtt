#include <iomanip>      // std::get_time
#include <ctime>        // struct std::tm
#include <sstream>
#include <iostream>

int main(){
  std::string example_string("2019-04-30_14:46:22_cam02_5.mp4");
  std::string time_string = example_string.substr(11, 8);
  std::cout << "time_string: " << time_string << std::endl;
  struct std::tm when;
  std::istringstream ss(time_string);
  ss >> std::get_time(&when, "%H:%M:%S"); // or just %T in this case
  std::cout << when.tm_hour << " hours and " << when.tm_min << " minutes\n";
  return 0;
}
