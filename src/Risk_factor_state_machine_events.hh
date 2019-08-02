#ifndef _RISK_FACTOR_EVENTS_
#define _RISK_FACTOR_EVENTS_

#include <string>

struct Perimetral_sensor_sig{
  Perimetral_sensor_sig(const std::string &sensor_mini_id=""): _sensor_mini_id(sensor_mini_id){} 
  std::string _sensor_mini_id;
};

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
struct Int_wind_open_sensor_sig{
  Int_wind_open_sensor_sig(const std::string &sensor_mini_id=""): _sensor_mini_id(sensor_mini_id){} 
  std::string _sensor_mini_id;
};

//Someone opened the window associated to the reserved zone
struct Res_wind_open_sensor_sig{
  Res_wind_open_sensor_sig(const std::string &sensor_mini_id=""): _sensor_mini_id(sensor_mini_id){} 
  std::string _sensor_mini_id;
};

//Someone moved in the external zone
struct Ext_motion_sensor_sig{
  Ext_motion_sensor_sig(const std::string &sensor_mini_id=""): _sensor_mini_id(sensor_mini_id){} 
  std::string _sensor_mini_id;
};

//Someone moved in the internal zone
struct Int_motion_sensor_sig{
  Int_motion_sensor_sig(const std::string &sensor_mini_id=""): _sensor_mini_id(sensor_mini_id){} 
  std::string _sensor_mini_id;
};

//Someone moved in the reserved zone
struct Res_motion_sensor_sig{
  Res_motion_sensor_sig(const std::string &sensor_mini_id=""): _sensor_mini_id(sensor_mini_id){} 
  std::string _sensor_mini_id;
};

//Recognized owner in external zone
struct Rec_owner_in_ext{
  Rec_owner_in_ext(const std::string &mmuid = ""): _mmuid(mmuid){}
  std::string _mmuid;
};

//Recognized owner in internal zone
struct Rec_owner_in_int{
  Rec_owner_in_int(const std::string &mmuid = ""): _mmuid(mmuid){}
  std::string _mmuid;
};

//Recognized owner in reserved zone
struct Rec_owner_in_res{
  Rec_owner_in_res(const std::string &mmuid = ""): _mmuid(mmuid){}
  std::string _mmuid;
};

//Recognized monit in external zone
struct Rec_monit_in_ext{
  Rec_monit_in_ext(const std::string &mmuid = ""): _mmuid(mmuid){}
  std::string _mmuid;
};

//Recognized monit in internal zone
struct Rec_monit_in_int{
  Rec_monit_in_int(const std::string &mmuid = ""): _mmuid(mmuid){}
  std::string _mmuid;
};
//Recognized monit in reserved zone
struct Rec_monit_in_res{
  Rec_monit_in_res(const std::string &mmuid = ""): _mmuid(mmuid){}
  std::string _mmuid;
};

//"Recognized an unknown" in external zone
struct Rec_unk_in_ext{
  Rec_unk_in_ext(const std::string &mmuid = ""): _mmuid(mmuid){}
  std::string _mmuid;
};

//"Recognized an unknown" in internal zone
struct Rec_unk_in_int{
  Rec_unk_in_int(const std::string &mmuid = ""): _mmuid(mmuid){}
  std::string _mmuid;
};

//"Recognized an unknown" in reserved zone
struct Rec_unk_in_res{
  Rec_unk_in_res(const std::string &mmuid = ""): _mmuid(mmuid){}
  std::string _mmuid;
};

//Reset from any state to Waiting_for_risk
struct Reset_risk{};
#endif
