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
					   "Low_risk"};

void current_state(Alarm_system const &alarm_system){
  //D(std::cout << warning << "Current states -> " << reset << state_names[alarm_system.current_state()[0]] << std::endl);
   // we have now several active states, which we show
  for (unsigned int i=0;
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
  
  // //Fake transition to reset Evaluating_risk
  // alarm_system.process_event(Reset_risk{});
  // current_state(alarm_system);
  // //transition from "Waiting_for_risk" state to "Evaluating_risk" triggered by "Ext_door_open_sensor_sig" event
  // alarm_system.process_event(Ext_door_open_sensor_sig{});
  // current_state(alarm_system);

  
  return 0;
}
