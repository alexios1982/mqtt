#define DEBUG

#include "Utils.hh"
#include "Alarm_system_state_machine.hh"
#include <boost/core/ignore_unused.hpp>

//
// Testing utilities.
//
static char const* const state_names[] = {// "Init",
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
    D(std::cout << warning << "::current_states. Current states -> " << reset << state_names[alarm_system.current_state()[i]] << '\n');
  }
  std::cout << '\n' << std::endl;
}

int main(int argc, char **argv){
  boost::ignore_unused(argc);
  boost::ignore_unused(argv);
  
  Alarm_system alarm_system;
  alarm_system.start();
  //At start, the machine is in Init and Waiting_for_risk
  current_state(alarm_system);

  int number_of_levels = alarm_system.get_number_of_levels();
  std::cout << error;
  std::cout << "Number of levels: " << reset;
  std::cout << number_of_levels << '\n' << std::endl;
  
  //Alarm system state machine test
  std::cout << error;
  std::cout << "****************************************************************" << std::endl;
  std::cout << "************** Alarm system state machine test *****************" << std::endl;
  std::cout << "****************************************************************" << std::endl;
  std::cout << reset << std::endl;

  std::cout << error << "Transition triggered by " << reset << "Initialization_completed" << std::endl;
  //transition from "Waiting_for_configuration" state to "Green_alarm" triggered by "Initialization_completed" event
  alarm_system.process_event(Initialization_completed{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Red_alarm" << std::endl;
  //transition from "Green_alarm" state to "Red_alarm_notified" triggered by "Red_alarm" event
  alarm_system.process_event(Red_alarm{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "User_cancellation" << std::endl;
  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Orange_alarm" << std::endl;
  //transition from "Green_alarm" state to "Orange_alarm_notified" triggered by "Orange_alarm" event
  alarm_system.process_event(Orange_alarm{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "User_confirmation" << std::endl;
  //transition from "Orange_alarm_notified" state to "Red_alarm_actuation" triggered by "User_confirmation" event
  alarm_system.process_event(User_confirmation{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Stop_actuation" << std::endl;
  //transition from "Red_alarm_actuation" state to "Green_alarm" triggered by "Stop_actuation" event
  alarm_system.process_event(Stop_actuation{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Orange_alarm" << std::endl;
  //transition from "Green_alarm" state to "Orange_alarm_notified" triggered by "Orange_alarm" event
  alarm_system.process_event(Orange_alarm{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "User_cancellation" << std::endl;
  //transition from "Orange_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Red_alarm" << std::endl;
  //transition from "Green_alarm" state to "Red_alarm_notified" triggered by "Red_alarm" event
  alarm_system.process_event(Red_alarm{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "User_cancellation" << std::endl;
  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Orange_alarm" << std::endl;
  //transition from "Green_alarm" state to "Orange_alarm_notified" triggered by "Orange_alarm" event
  alarm_system.process_event(Orange_alarm{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Red_alarm_timeout" << std::endl;
  //transition from "Orange_alarm_notified" state to "Red_alarm_actuation" triggered by "Red_alarm_timeout" event
  alarm_system.process_event(Red_alarm_timeout{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Stop_actuation" << std::endl;
  //transition from "Red_alarm_actuation" state to "Green_alarm" triggered by "Stop_actuation" event
  alarm_system.process_event(Stop_actuation{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Orange_alarm" << std::endl;
  //transition from "Green_alarm" state to "Orange_alarm_notified" triggered by "Orange_alarm" event
  alarm_system.process_event(Orange_alarm{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Automatic_cancellation" << std::endl;
  //transition from "Orange_alarm_notified" state to "Green_alarm" triggered by "Automatic_cancellation" event
  alarm_system.process_event(Automatic_cancellation{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Switch_off" << std::endl;
  //transition from "Green_alarm" state to "Exit" triggered by "Switch_off" event
  alarm_system.process_event(Switch_off{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Switch_on" << std::endl;
  //transition from "Exit" state to "Green_alarm" triggered by "Switch_on" event
  alarm_system.process_event(Switch_on{});
  current_state(alarm_system);

  std::cout << error;
  std::cout << "****************************************************************" << std::endl;
  std::cout << "************** Risk factor state machine test ******************" << std::endl;
  std::cout << "****************************************************************" << std::endl;
  std::cout << reset << std::endl;

  //transition from "Waiting_for_risk" state to "Evaluating_risk" triggered by "Perimetral sensor signal" event
  //This transition triggers "Red_alarm" event that forces a transition form "Green_alarm" to "Red_alarm_notified"
  std::cout << error << "Transition triggered by " << reset << "Perimetral_sensor_sig" << std::endl;
  alarm_system.process_event(Perimetral_sensor_sig{});
  current_state(alarm_system);

  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  std::cout << error << "Transition triggered by " << reset << "User_cancellation" << std::endl;
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  //Internal transition in "Evaluating_risk" state triggered by "Perimetral sensor signal" event
  //This transition triggers "Red_alarm event that forces a transition form "Green_alarm" to "Red_alarm"
  std::cout << error << "Transition triggered by " << reset << "Perimetral_sensor_sig" << std::endl;
  alarm_system.process_event(Perimetral_sensor_sig{});
  current_state(alarm_system);

  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  std::cout << error << "Transition triggered by " << reset << "User_cancellation" << std::endl;
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  //Internal transition in "Evaluating_risk" state triggered by "Ext_door_open_sensor_sig" event
  //Because we are also in Idle, this event triggers a transition to Waiting_for_ai_response calling increase_ai_response_counter
  std::cout << error << "Transition triggered by " << reset << "Ext_door_open_sensor_sig" << std::endl;
  alarm_system.process_event(Ext_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in "Evaluating_risk" state triggered by "Int_door_open_sensor_sig" event
  //Because we are already in Waiting_for_ai_response  no event is triggered in the Internal_activity transition state
  std::cout << error << "Transition triggered by " << reset << "Int_door_open_sensor_sig" << std::endl;
  alarm_system.process_event(Int_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in "Evaluating_risk" state triggered by "Res_door_open_sensor_sig" event
  //Because we are already in Waiting_for_ai_response  no event is triggered in the Internal_activity transition state
  std::cout << error << "Transition triggered by " << reset << "Res_door_open_sensor_sig" << std::endl;
  alarm_system.process_event(Res_door_open_sensor_sig{});
  current_state(alarm_system);

  //Internal transition in "Evaluating_risk" state triggered by "Ext_motion_sensor_sig" event
  //This transition triggers "Orange_alarm" event that forces a transition form "Green_alarm" to "Orange_alarm"
  std::cout << error << "Transition triggered by " << reset << "Ext_motion_sensor_sig" << std::endl;
  alarm_system.process_event(Ext_motion_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Automatic_cancellation" << std::endl;
  //transition from "Orange_alarm_notified" state to "Green_alarm" triggered by "Automatic_cancellation" event
  alarm_system.process_event(Automatic_cancellation{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Int_motion_sensor_sig" << std::endl;
  //Internal transition in "Evaluating_risk" state triggered by "Int_motion_sensor_sig" event
  //This transition triggers "Red_alarm" event that forces a transition form "Green_alarm" to "Red_alarm"   
  alarm_system.process_event(Int_motion_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "User_cancellation" << std::endl;
  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Res_motion_sensor_sig" << std::endl;
  //Internal transition in "Evaluating_risk" state triggered by "Res_motion_sensor_sig" event
  alarm_system.process_event(Res_motion_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "User_cancellation" << std::endl;
  //transition from "Red_alarm_notified" state to "Green_alarm" triggered by "User_cancellation" event
  alarm_system.process_event(User_cancellation{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_owner_in_ext" << std::endl;
  //Transition from "Evaluating_risk" state to "Low_risk" state triggered by "Rec_owner_in_ext" event
  alarm_system.process_event(Rec_owner_in_ext{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Perimetral_sensor_sig" << std::endl;
  //Internal transition in Low_risk triggered by "Perimetral sensor_signal".
  //This transition triggers "Orange_alarm" event that forces a transition to "Orange_alarm_notified"
  alarm_system.process_event(Perimetral_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Ext_door_open_sensor_sig" << std::endl;
  //Internal transition in Low_risk triggered by "Ext_door_open_sensor_signal".
  //This transition calls send_video_chunk action
  alarm_system.process_event(Ext_door_open_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Int_door_open_sensor_sig" << std::endl;
  //Internal transition in Low_risk triggered by "Int_door_open_sensor_signal".
  //This transition calls send_video_chunk action
  alarm_system.process_event(Int_door_open_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Res_door_open_sensor_sig" << std::endl;
  //Internal transition in Low_risk triggered by "Res_door_open_sensor_signal".
  //This transition calls send_video_chunk action
  alarm_system.process_event(Res_door_open_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_owner_in_ext" << std::endl;
  //Internal transition in Low_risk triggered by "Rec_owner_in_ext".
  //This transition calls ext_presence_flag_update
  alarm_system.process_event(Rec_owner_in_ext{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_owner_in_int" << std::endl;
  //Internal transition in Low_risk triggered by "Rec_owner_in_int".
  //This transition calls int_presence_flag_update
  alarm_system.process_event(Rec_owner_in_int{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_owner_in_res" << std::endl;
  //Internal transition in Low_risk triggered by "Rec_owner_in_res".
  //This transition calls int_presence_flag_update
  alarm_system.process_event(Rec_owner_in_res{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_monit_in_ext" << std::endl;
  //Internal transition in Low_risk triggered by "Rec_monit_in_ext".
  //This transition calls ext_presence_flag_update
  alarm_system.process_event(Rec_monit_in_ext{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_monit_in_int" << std::endl;
  //Internal transition in Low_risk triggered by "Rec_monit_in_int".
  //This transition calls int_presence_flag_update
  alarm_system.process_event(Rec_monit_in_int{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_monit_in_res" << std::endl;
  //Internal transition in Low_risk triggered by "Rec_monit_in_res".
  //This transition calls res_presence_flag_update
  alarm_system.process_event(Rec_monit_in_res{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_unk_in_ext" << std::endl;
  //Internal transition in Low_risk triggered by "Rec_unk_in_ext".
  //This transition calls ext_presence_flag_update
  alarm_system.process_event(Rec_unk_in_ext{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Reset_risk" << std::endl;
  //Transition from Low_risk to Waiting_for_risk triggered by "Reset_risk".
  //This transition calls reset_presence_flags
  alarm_system.process_event(Reset_risk{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Automatic_cancellation" << std::endl;
  //Transition from Orange_alarm_notified to Greem_alarm triggered by "Automatic_cancellation".
  alarm_system.process_event(Automatic_cancellation{});
  current_state(alarm_system);

  std::cout << std::endl;
  std::cout << error << "!!!!!!!!!!!!!   creating first step !!!!!!!!!!!!!" << reset << std::endl;
  std::cout << std::endl;

  std::cout << error << "Transition triggered by " << reset << "Ext_door_open_sensor_sig and Rec_monit_in_ext" << std::endl;
  //Transition from Waiting_for_risk to Evaluating_risk and then to Medium_risk triggered by Ext_door_open_sensor_sig and Rec_monit_in_external
  alarm_system.process_event(Ext_door_open_sensor_sig{});
  alarm_system.process_event(Rec_monit_in_ext{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Perimetral_sensor_sig" << std::endl;
  //Internal transition in Medium_risk triggered by "Perimetral sensor_signal".
  //This transition triggers "Red_alarm" event if external_ring_is_empty (supposed true)
  alarm_system.process_event(Perimetral_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Ext_door_open_sensor_sig" << std::endl;
  //Internal transition in Medium_risk triggered by "Ext_door_open_sensor_signal".
  //This transition calls send_video_chunk action
  alarm_system.process_event(Ext_door_open_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Int_door_open_sensor_sig" << std::endl;
  //Internal transition in Medium_risk triggered by "Int_door_open_sensor_signal".
  //This transition calls send_video_chunk action
  alarm_system.process_event(Int_door_open_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Res_door_open_sensor_sig" << std::endl;
  //Internal transition in Medium_risk triggered by "Res_door_open_sensor_signal".
  //This transition calls send_video_chunk_and_trigger_orange_alarm
  alarm_system.process_event(Res_door_open_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Ext_motion_sensor_sig" << std::endl;
  //Internal transition in Medium_risk triggered by "Ext_motion_sensor_signal".
  //This transition calls trigger_red_alarm_if_ext_empty (supposed true)
  alarm_system.process_event(Ext_motion_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Int_motion_sensor_sig" << std::endl;
  //Internal transition in Medium_risk triggered by "Int_motion_sensor_signal".
  //This transition calls trigger_red_alarm_if_ext_empty (supposed true)
  alarm_system.process_event(Int_motion_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Res_motion_sensor_sig" << std::endl;
  //Internal transition in Medium_risk triggered by "Res_motion_sensor_signal".
  //This transition triggers Red_alarm
  alarm_system.process_event(Res_motion_sensor_sig{});
  current_state(alarm_system);

  std::cout << std::endl;
  std::cout << error << "!!!!!!!!!!!!! creating second step !!!!!!!!!!!!!" << reset << std::endl;
  std::cout << std::endl;


  std::cout << error << "Transition triggered by " << reset << "Rec_owner_in_ext" << std::endl;
  //Transition from Medium_risk to Low_risk triggered by Rec_owner_in_external/Rec_owner_in_external/Rec_owner_in_res.
  //This events calls ext_presence_flag_update_and_trigger_automatic_cancellation/int_presence_flag_update_and_trigger_automatic_cancellation/res_presence_flag_update_and_trigger_automatic_cancellation that triggers transition from Red_alarm_notified to Green_alarm (but there is no transition from Red_alarm triggered by automatic_cancellation)
  alarm_system.process_event(Rec_owner_in_ext{});
  //alarm_system.process_event(Rec_owner_in_int{});
  //alarm_system.process_event(Rec_owner_in_res{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Reset_risk" << std::endl;
  //Transition from Low_risk to Waiting_for_risk triggered by "Reset_risk".
  //This transition calls reset_presence_flags
  alarm_system.process_event(Reset_risk{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Int_door_open_sensor_sig and Rec_monit_in_int" << std::endl;
  //Transition from Waiting_for_risk to Evaluating_risk and then to Medium_risk triggered by Int_door_open_sensor_sig and Rec_monit_in_internal
  alarm_system.process_event(Int_door_open_sensor_sig{});
  alarm_system.process_event(Rec_monit_in_int{});
  current_state(alarm_system);


  std::cout << error << "Transition triggered by " << reset << "Rec_monit_in_ext" << std::endl;
  //Internal transition in Medium_risk triggered by "Rec_monit_in_ext".
  //This transition calls ext_presence_flag_update
  alarm_system.process_event(Rec_monit_in_ext{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_monit_in_int" << std::endl;
  //Internal transition in Medium_risk triggered by "Rec_monit_in_int".
  //This transition calls int_presence_flag_update
  alarm_system.process_event(Rec_monit_in_int{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_monit_in_res" << std::endl;
  //Internal transition in Medium_risk triggered by "Rec_monit_in_res".
  //This transition calls ext_presence_flag_update_and_trigger_orange_alarm
  alarm_system.process_event(Rec_monit_in_res{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_unk_in_ext" << std::endl;
  //Transition from Medium_risk to High_risk triggered by "Rec_unk_in_ext"
  //This transition calls ext_presence_flag_update_and_trigger_red_alarm
  alarm_system.process_event(Rec_unk_in_ext{});
  current_state(alarm_system);  

  std::cout << error << "Transition triggered by " << reset << "Reset_risk" << std::endl;
  //Reset transition to Waiting_for_risk
  alarm_system.process_event(Reset_risk{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Res_door_open_sensor_sig and Rec_monit_in_res" << std::endl;
   //Transition from Waiting_for_risk to Evaluating_risk and then to Medium_risk triggered by Res_door_open_sensor_sig and Rec_monit_in_res
  alarm_system.process_event(Res_door_open_sensor_sig{});
  alarm_system.process_event(Rec_monit_in_res{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_unk_in_res" << std::endl;
  //Transition from Medium_risk to High_risk triggered by "Rec_unk_in_res"
  //This transition calls res_presence_flag_update_and_trigger_red_alarm
  alarm_system.process_event(Rec_unk_in_res{});
  current_state(alarm_system);  

  std::cout << error << "Transition triggered by " << reset << "Reset_risk" << std::endl;
  //Reset transition to Waiting_for_risk
  alarm_system.process_event(Reset_risk{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Int_door_open_sensor_sig and Rec_monit_in_int" << std::endl;
  //Transition from Waiting_for_risk to Evaluating_risk and then to Medium_risk triggered by Int_door_open_sensor_sig and Rec_monit_in_res
  alarm_system.process_event(Int_door_open_sensor_sig{});
  alarm_system.process_event(Rec_monit_in_int{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_unk_in_int" << std::endl;
  //Transition from Medium_risk to High_risk triggered by "Rec_unk_in_int"
  //This transition calls int_presence_flag_update_and_trigger_red_alarm
  alarm_system.process_event(Rec_unk_in_int{});
  current_state(alarm_system);  

  std::cout << std::endl;
  std::cout << error << "!!!!!!!!!!!!! creating third step !!!!!!!!!!!!!" << reset << std::endl;
  std::cout << std::endl;

  std::cout << error << "Transition triggered by " << reset << "Perimetral_sensor_sig" << std::endl;
  //Internal transition in High_risk triggered by "Perimetral_sensor_signal".
  //This transition calls red_alarm 
  alarm_system.process_event(Perimetral_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Ext_door_open_sensor_sig" << std::endl;
  //Internal transition in High_risk triggered by "Ext_door_open_sensor_signal".
  //This transition calls send_video_chunk_and_trigger_red_alarm action
  alarm_system.process_event(Ext_door_open_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Int_door_open_sensor_sig" << std::endl;
  //Internal transition in High_risk triggered by "Int_door_open_sensor_signal".
  //This transition calls send_video_chunk_and_trigger_red_alarm action
  alarm_system.process_event(Int_door_open_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Res_door_open_sensor_sig" << std::endl;
  //Internal transition in High_risk triggered by "Res_door_open_sensor_signal".
  //This transition calls send_video_chunk_and_trigger_red_alarm action
  alarm_system.process_event(Res_door_open_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Ext_motion_sensor_sig" << std::endl;
  //Internal transition in High_risk triggered by "Ext_motion_sensor_signal".
  //This transition calls trigger_red_alarm
  alarm_system.process_event(Ext_motion_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Int_motion_sensor_sig" << std::endl;
  //Internal transition in High_risk triggered by "Int_motion_sensor_signal".
  //This transition calls trigger_red_alarm
  alarm_system.process_event(Int_motion_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Res_motion_sensor_sig" << std::endl;
  //Internal transition in Medium_risk triggered by "Res_motion_sensor_signal".
  //This transition triggers Red_alarm
  alarm_system.process_event(Res_motion_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_owner_in_res" << std::endl;
  //Transition from High_risk to Low_risk triggered by "Rec_owner_in_ext/Rec_owner_in_int/Rec_owner_in_res".
  //This transition triggers Green_alarm
  //alarm_system.process_event(Rec_owner_in_ext{});
  //alarm_system.process_event(Rec_owner_in_int{});
  alarm_system.process_event(Rec_owner_in_res{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Clear_res" << std::endl;
  //Transition from Reserved to Intern triggered by "Clear_res".
  alarm_system.process_event(Clear_res{});
  current_state(alarm_system);

 
  std::cout << error << "Transition triggered by " << reset << "Clear_int" << std::endl;
  //Transition from Intern to Extern triggered by "Clear_int".
  alarm_system.process_event(Clear_int{});
  current_state(alarm_system);

   std::cout << error << "Transition triggered by " << reset << "Int_door_open_sensor_sig" << std::endl;
  //Transition from Extern to Intern triggered by "Int_door_open_sensor_sig".
  alarm_system.process_event(Int_door_open_sensor_sig{});
  current_state(alarm_system);

  alarm_system.set_number_of_levels(Alarm_system_::HOUSE_WITHOUT_GARDEN_N_OF_LEVELS);
  //we set the numbers of levels to 2 to check the other transition from Intern triggered by Clear_int to the state None 
   std::cout << error << "Transition triggered by " << reset << "Clear_int" << std::endl;
  //Transition from Extern to Intern triggered by "Int_door_open_sensor_sig".
  alarm_system.process_event(Clear_int{});
  current_state(alarm_system);
  
   std::cout << error << "Transition triggered by " << reset << "Int_door_open_sensor_sig" << std::endl;
  //Transition from Extern to Intern triggered by "Int_door_open_sensor_sig".
  alarm_system.process_event(Int_door_open_sensor_sig{});
  current_state(alarm_system);

  alarm_system.set_number_of_levels(Alarm_system_::HOUSE_WITHOUT_GARDEN_N_OF_LEVELS);
  //we set the numbers of levels to 2 to check the other transition from Intern triggered by Clear_int to the state None 
   std::cout << error << "Transition triggered by " << reset << "Clear_int" << std::endl;
  alarm_system.process_event(Clear_int{});
  current_state(alarm_system);
  
 
  // std::cout << error << "Transition triggered by " << reset << "Int_door_open_sensor_sig" << std::endl;
  // //Transition from None to Intern triggered by Int_door_open_sensor_signal
  // alarm_system.process_event(Int_door_open_sensor_sig{});
  // current_state(alarm_system);

  
  std::cout << error << "Transition triggered by " << reset << "Int_wind_open_sensor_sig" << std::endl;
  //Transition from Waiting_for_risk to Medium_risk triggered by Int_window_open_sensor_signal
  alarm_system.process_event(Int_wind_open_sensor_sig{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_unk_in_ext" << std::endl;
  //Transition from Medium_risk to High_risk triggered by Rec_unk_in_ext
  alarm_system.process_event(Rec_unk_in_ext{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_monit_in_ext" << std::endl;
  //Internal transition in High_risk triggered by Rec_monit_in_ext
  alarm_system.process_event(Rec_monit_in_ext{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_monit_in_int" << std::endl;
  //Internal transition in High_risk triggered by Rec_monit_in_int
  alarm_system.process_event(Rec_monit_in_int{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_monit_in_res" << std::endl;
  //Internal transition in High_risk triggered by Rec_monit_in_res
  alarm_system.process_event(Rec_monit_in_res{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_unk_in_ext" << std::endl;
  //Internal transition in High_risk triggered by Rec_unk_in_ext
  alarm_system.process_event(Rec_unk_in_ext{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_unk_in_int" << std::endl;
  //Internal transition in High_risk triggered by Rec_unk_in_int
  alarm_system.process_event(Rec_unk_in_int{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Rec_unk_in_res" << std::endl;
  //Internal transition in High_risk triggered by Rec_unk_in_res
  alarm_system.process_event(Rec_unk_in_res{});
  current_state(alarm_system);

  std::cout << error << "Transition triggered by " << reset << "Reset_risk" << std::endl;
  //Reset transition to Waiting_for_risk
  alarm_system.process_event(Reset_risk{});
  current_state(alarm_system);
  
  return 0;
}
