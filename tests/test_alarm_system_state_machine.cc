#define DEBUG

#include "Utils.hh"
#include "Alarm_system_state_machine.hh"

//
// Testing utilities.
//
static char const* const state_names[] = { "Init",
					   "Waiting_for_configuration",
					   "Green_alarm",
					   "Orange_alarm_notified",
					   "Red_alarm_notified",
					   "Red_alarm_actuation",
					   "Exit"};

void current_state(Alarm_system const &alarm_system){
  D(std::cout << warning << "Current states -> " << reset << state_names[alarm_system.current_state()[0]] << std::endl);
}
int main(int argc, char **argv){

  Alarm_system alarm_system;
  alarm_system.start();
  current_state(alarm_system);

  //transition from "Init" state to "Waiting_for_configuration" triggered by "Switch_on" event
  alarm_system.process_event(Switch_on{});
  current_state(alarm_system);

  //transition from "Waiting_for_configuration" state to "Green_alarm" triggered by "Initialization_completed" event
  alarm_system.process_event(Initialization_completed{});
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

  return 0;
}
