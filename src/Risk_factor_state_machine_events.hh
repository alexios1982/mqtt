#ifndef _RISK_FACTOR_EVENTS_
#define _RISK_FACTOR_EVENTS_

#include <string>

struct Perimetral_sensor_sig{};

//Someone opened the external door
struct Ext_door_open_sensor_sig{
  Ext_door_open_sensor_sig(const std::string &sensor_mini_id=""): _sensor_mini_id(sensor_mini_id){} 
  std::string _sensor_mini_id;
};

//Someone opened the internal door
struct Int_door_open_sensor_sig{
  Int_door_open_sensor_sig(const std::string &sensor_mini_id=""): _sensor_mini_id(sensor_mini_id){} 
  std::string _sensor_mini_id;
};

//Someone opened the door associated to the reserved zone
struct Res_door_open_sensor_sig{
  Res_door_open_sensor_sig(const std::string &sensor_mini_id=""): _sensor_mini_id(sensor_mini_id){} 
  std::string _sensor_mini_id;
};

//Someone opened the internal window
struct Int_wind_open_sensor_sig{};
//Someone opened the window associated to the reserved zone
struct Res_wind_open_sensor_sig{};

//Someone moved in the external zone
struct Ext_motion_sensor_sig{};
//Someone moved in the internal zone
struct Int_motion_sensor_sig{};
//Someone moved in the reserved zone
struct Res_motion_sensor_sig{};

//Recognized owner in external zone
struct Rec_owner_in_ext{};
//Recognized owner in internal zone
struct Rec_owner_in_int{};
//Recognized owner in reserved zone
struct Rec_owner_in_res{};

//Recognized monit in external zone
struct Rec_monit_in_ext{};
//Recognized monit in internal zone
struct Rec_monit_in_int{};
//Recognized monit in reserved zone
struct Rec_monit_in_res{};

//"Recognized an unknown" in external zone
struct Rec_unk_in_ext{};
//"Recognized an unknown" in internal zone
struct Rec_unk_in_int{};
//"Recognized an unknown" in reserved zone
struct Rec_unk_in_res{};

//Reset from any state to Waiting_for_risk
struct Reset_risk{};
#endif
