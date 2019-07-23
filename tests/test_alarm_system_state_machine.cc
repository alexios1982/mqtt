#define DEBUG

#include "Utils.hh"
#include "Alarm_system_state_machine.hh"
#include <boost/core/ignore_unused.hpp>

//
// Testing utilities.
//
static char const* const state_names[] = { "Init",
					   "Waiting_for_configuration",
					   "Green_alarm",
					   "Orange_alarm_notified",
					   "Red_alarm_notified",
					   "Red_alarm_actuation",
					   "Exit",
					   "Waiting_for_risk",
					   "Evaluating_risk",
					   "Low_risk",
					   "Medium_risk",
					   "High_risk",
					   "None",
					   "Extern",
					   "Intern",
					   "Reserved",
					   "Idle",
					   "Waiting_for_ai_response"};

void current_state(Alarm_system const &alarm_system){
  //D(std::cout << warning << "Current states -> " << reset << state_names[alarm_system.current_state()[0]] << std::endl);
   // we have now several active states, which we show
  for (unsigned int i = 0;
       i < Alarm_system::nr_regions::value;
       ++i){
      D(std::cout << warning << "::current_states. Current states -> " << reset << state_names[alarm_system.current_state()[i]] << std::endl);
    }
}
int main(int argc, char **argv){

  Alarm_system alarm_system;
  alarm_system.start();
  //At start, the machine is in Init and Waiting_for_risk
  current_state(alarm_system);

  //Alarm system state machine test
  
  //transition from "Init" state to "Waiting_for_configuration" triggered by "Switch_on" event
  alarm_system.process_event(Switch_on{});
  current_state(alarm_system);

  //transition from "Waiting_for_configuration" state to "Green_alarm" triggered by "Initialization_completed" event
  alarm_system.process_event(Initialization_completed{});
  current_state(alarm_system);

  //transition from "Green_alarm" state to "Red_alarm_notified" triggered by "Red_alarm" event
  alarm_system.process_event(Red_alarm{});
  current_state(alarm_system);

  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  //transition from "Green_alarm" state to "Orange_alarm_notified" triggered by "Orange_alarm" event
  alarm_system.process_event(Orange_alarm{});
  current_state(alarm_system);

  //transition from "Orange_alarm_notified" state to "Red_alarm_actuation" triggered by "User_confirmation" event
  alarm_system.process_event(User_confirmation{});
  current_state(alarm_system);

  //transition from "Red_alarm_actuation" state to "Green_alarm" triggered by "Stop_actuation" event
  alarm_system.process_event(Stop_actuation{});
  current_state(alarm_system);

   //transition from "Green_alarm" state to "Orange_alarm_notified" triggered by "Orange_alarm" event
  alarm_system.process_event(Orange_alarm{});
  current_state(alarm_system);

  //transition from "Orange_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);
  
  //transition from "Green_alarm" state to "Red_alarm_notified" triggered by "Red_alarm" event
  alarm_system.process_event(Red_alarm{});
  current_state(alarm_system);

  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  //transition from "Green_alarm" state to "Orange_alarm_notified" triggered by "Orange_alarm" event
  alarm_system.process_event(Orange_alarm{});
  current_state(alarm_system);

  //transition from "Orange_alarm_notified" state to "Red_alarm_actuation" triggered by "Red_alarm_timeout" event
  alarm_system.process_event(Red_alarm_timeout{});
  current_state(alarm_system);

  //transition from "Red_alarm_actuation" state to "Green_alarm" triggered by "Stop_actuation" event
  alarm_system.process_event(Stop_actuation{});
  current_state(alarm_system);

  //transition from "Green_alarm" state to "Orange_alarm_notified" triggered by "Orange_alarm" event
  alarm_system.process_event(Orange_alarm{});
  current_state(alarm_system);

  //transition from "Orange_alarm_notified" state to "Green_alarm" triggered by "Automatic_cancellation" event
  alarm_system.process_event(Automatic_cancellation{});
  current_state(alarm_system);
  
  //transition from "Green_alarm" state to "Exit" triggered by "Switch_off" event
  alarm_system.process_event(Switch_off{});
  current_state(alarm_system);

  //transition from "Exit" state to "Green_alarm" triggered by "Switch_on" event
  alarm_system.process_event(Switch_on{});
  current_state(alarm_system);

  //Risk factor state machine test

  std::cout << std::endl;
  std::cout << "Vediamo un po" << std::endl;
  std::cout << std::endl;
  
  //transition from "Waiting_for_risk" state to "Evaluating_risk" triggered by "Perimetral sensor signal" event
  //This transition triggers "Red_alarm event that forces a transition form "Green_alarm" to "Red_alarm"
  alarm_system.process_event(Perimetral_sensor_sig{});
  current_state(alarm_system);

  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  //Internal transition in "Waiting_for_risk" state triggered by "Perimetral sensor signal" event
  //This transition triggers "Red_alarm event that forces a transition form "Green_alarm" to "Red_alarm"  
  alarm_system.process_event(Perimetral_sensor_sig{});
  current_state(alarm_system);

  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  //Internal transition in "Waiting_for_risk" state triggered by "Ext_door_open_sensor_sig" event
  alarm_system.process_event(Ext_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in "Waiting_for_risk" state triggered by "Int_door_open_sensor_sig" event
  alarm_system.process_event(Int_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in "Waiting_for_risk" state triggered by "Res_door_open_sensor_sig" event
  alarm_system.process_event(Res_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in "Waiting_for_risk" state triggered by "Ext_motion_sensor_sig" event
  //This transition triggers "Orange_alarm" event that forces a transition form "Green_alarm" to "Orange_alarm" 
  alarm_system.process_event(Ext_motion_sensor_sig{});
  current_state(alarm_system);

  //transition from "Orange_alarm_notified" state to "Green_alarm" triggered by "Automatic_cancellation" event
  alarm_system.process_event(Automatic_cancellation{});
  current_state(alarm_system);

  //Internal transition in "Waiting_for_risk" state triggered by "Int_motion_sensor_sig" event
  //This transition triggers "Red_alarm" event that forces a transition form "Green_alarm" to "Red_alarm"   
  alarm_system.process_event(Int_motion_sensor_sig{});
  current_state(alarm_system);

  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  //Internal transition in "Waiting_for_risk" state triggered by "Res_motion_sensor_sig" event
  alarm_system.process_event(Res_motion_sensor_sig{});
  current_state(alarm_system);

  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);


  //Internal transition in "Evaluating_risk" state triggered by "Ext_motion_sensor_sig" event
  //This transition triggers "Orange_alarm" event that forces a transition form "Green_alarm" to "Orange_alarm" 
  alarm_system.process_event(Ext_motion_sensor_sig{});
  current_state(alarm_system);

  //Transition from "Evaluating_risk" state to "Low_risk" state triggered by "Rec_owner_in_ext" event
  //This transition triggers "Automatic_cancellation" event that forces a transition form "Orange_alarm" to "Green_alarm" 
  alarm_system.process_event(Rec_owner_in_ext{});
  current_state(alarm_system);

  //Internal transition in Low_risk triggered by "Perimetral sensor_signal".
  //This transition triggers "Orange_alarm" event that forces a transition to "Orange_alarm_notified"
  alarm_system.process_event(Perimetral_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in Low_risk triggered by "Ext_door_open_sensor_signal".
  //This transition calls send_video_chunk action
  alarm_system.process_event(Ext_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in Low_risk triggered by "Int_door_open_sensor_signal".
  //This transition calls send_video_chunk action
  alarm_system.process_event(Int_door_open_sensor_sig{});
  current_state(alarm_system);
  
  //Internal transition in Low_risk triggered by "Res_door_open_sensor_signal".
  //This transition calls send_video_chunk action
  alarm_system.process_event(Res_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in Low_risk triggered by "Rec_owner_in_ext".
  //This transition calls ext_presence_flag_update
  alarm_system.process_event(Rec_owner_in_ext{});
  current_state(alarm_system);
  
  //Internal transition in Low_risk triggered by "Rec_owner_in_int".
  //This transition calls int_presence_flag_update
  alarm_system.process_event(Rec_owner_in_int{});
  current_state(alarm_system);

  //Internal transition in Low_risk triggered by "Rec_owner_in_res".
  //This transition calls int_presence_flag_update
  alarm_system.process_event(Rec_owner_in_res{});
  current_state(alarm_system);

  //Internal transition in Low_risk triggered by "Rec_monit_in_ext".
  //This transition calls ext_presence_flag_update
  alarm_system.process_event(Rec_monit_in_ext{});
  current_state(alarm_system);
  
  //Internal transition in Low_risk triggered by "Rec_monit_in_int".
  //This transition calls int_presence_flag_update
  alarm_system.process_event(Rec_monit_in_int{});
  current_state(alarm_system);

  //Internal transition in Low_risk triggered by "Rec_monit_in_res".
  //This transition calls res_presence_flag_update
  alarm_system.process_event(Rec_monit_in_res{});
  current_state(alarm_system);
  
  //Internal transition in Low_risk triggered by "Rec_unk_in_ext".
  //This transition calls res_presence_flag_update
  alarm_system.process_event(Rec_unk_in_ext{});
  current_state(alarm_system);

  //Transition from Low_risk to Waiting_for_risk triggered by "Reset_risk".
  //This transition calls reset_presence_flags
  alarm_system.process_event(Reset_risk{});
  current_state(alarm_system);

  //Transition from Orange_alarm_notified to Greem_alarm triggered by "Automatic_cancellation".
  alarm_system.process_event(Automatic_cancellation{});
  current_state(alarm_system);
  
  //Transition from Waiting_for_risk to Evaluating_risk and then to  Medium_risk triggered by Ext_door_open_sensor_sig and Rec_monit_in_external
  alarm_system.process_event(Ext_door_open_sensor_sig{});
  alarm_system.process_event(Rec_monit_in_ext{});

  //Internal transition in Medium_risk triggered by "Perimetral sensor_signal".
  //This transition triggers "Red_alarm" event if external_ring_is_empty (supposed true)
  alarm_system.process_event(Perimetral_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in Medium_risk triggered by "Ext_door_open_sensor_signal".
  //This transition calls send_video_chunk action
  alarm_system.process_event(Ext_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in Medium_risk triggered by "Int_door_open_sensor_signal".
  //This transition calls send_video_chunk action
  alarm_system.process_event(Int_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in Medium_risk triggered by "Res_door_open_sensor_signal".
  //This transition calls send_video_chunk_and_trigger_orange_alarm
  alarm_system.process_event(Res_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in Medium_risk triggered by "Ext_motion_sensor_signal".
  //This transition calls trigger_red_alarm_if_ext_empty (supposed true)
  alarm_system.process_event(Ext_motion_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in Medium_risk triggered by "Int_motion_sensor_signal".
  //This transition calls trigger_red_alarm_if_ext_empty (supposed true)
  alarm_system.process_event(Int_motion_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in Medium_risk triggered by "Res_motion_sensor_signal".
  //This transition triggers Red_alarm
  alarm_system.process_event(Res_motion_sensor_sig{});
  current_state(alarm_system);

  // //Transition from Medium_risk to Low_risk triggered by Rec_owner_in_external/Rec_owner_in_external/Rec_owner_in_res.
  // //This events calls ext_presence_flag_update_and_trigger_automatic_cancellation/int_presence_flag_update_and_trigger_automatic_cancellation/res_presence_flag_update_and_trigger_automatic_cancellation that triggers transition from Red_alarm_notified to Green_alarm (but there is no transition from Red_alarm triggered by automatic_cancellation)
  // //I can't test all of them because there is no transition from Low_risk to Medium_risk
  // alarm_system.process_event(Rec_owner_in_ext{});
  // //alarm_system.process_event(Rec_owner_in_int{});
  // //alarm_system.process_event(Rec_owner_in_res{});
  // current_state(alarm_system);
  
  //Internal transition in Medium_risk triggered by "Rec_monit_in_ext".
  //This transition calls ext_presence_flag_update
  alarm_system.process_event(Rec_monit_in_ext{});
  current_state(alarm_system);

  //Internal transition in Medium_risk triggered by "Rec_monit_in_int".
  //This transition calls int_presence_flag_update
  alarm_system.process_event(Rec_monit_in_int{});
  current_state(alarm_system);

  //Internal transition in Medium_risk triggered by "Rec_monit_in_res".
  //This transition calls ext_presence_flag_update_and_trigger_orange_alarm
  alarm_system.process_event(Rec_monit_in_res{});
  current_state(alarm_system);

  std::cout << std::endl;
  std::cout << " Sono qua!" << std::endl;
  std::cout << std::endl;
  
  //Transition from Medium_risk to High_risk triggered by "Rec_unk_in_ext"
  //This transition calls ext_presence_flag_update_and_trigger_red_alarm
  alarm_system.process_event(Rec_unk_in_ext{});
  current_state(alarm_system);  

  //Transition from Medium_risk to High_risk triggered by "Rec_unk_in_int"
  //This transition calls int_presence_flag_update_and_trigger_red_alarm
  alarm_system.process_event(Rec_unk_in_int{});
  current_state(alarm_system);  

  //Transition from Medium_risk to High_risk triggered by "Rec_unk_in_res"
  //This transition calls res_presence_flag_update_and_trigger_red_alarm
  alarm_system.process_event(Rec_unk_in_res{});
  current_state(alarm_system);  

  //Internal transition in High_risk triggered by "Perimetral_sensor_signal".
  //This transition calls red_alarm 
  alarm_system.process_event(Perimetral_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in High_risk triggered by "Ext_door_open_sensor_signal".
  //This transition calls send_video_chunk_and_trigger_red_alarm action
  alarm_system.process_event(Ext_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in High_risk triggered by "Int_door_open_sensor_signal".
  //This transition calls send_video_chunk_and_trigger_red_alarm action
  alarm_system.process_event(Int_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in High_risk triggered by "Res_door_open_sensor_signal".
  //This transition calls send_video_chunk_and_trigger_red_alarm action
  alarm_system.process_event(Res_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in High_risk triggered by "Ext_motion_sensor_signal".
  //This transition calls trigger_red_alarm
  alarm_system.process_event(Ext_motion_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in High_risk triggered by "Int_motion_sensor_signal".
  //This transition calls trigger_red_alarm
  alarm_system.process_event(Int_motion_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in Medium_risk triggered by "Res_motion_sensor_signal".
  //This transition triggers Red_alarm
  alarm_system.process_event(Res_motion_sensor_sig{});
  current_state(alarm_system);

  
  // //Reset transition to Waiting_for_risk
  // alarm_system.process_event(Reset_risk{});
  // current_state(alarm_system);
  
  return 0;
}
