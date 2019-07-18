#define DEBUG

#include "Alarm_system_state_machine.hh"
#include <iostream>
#include "Utils.hh"

template <class Event, class FSM>
void Alarm_system_::on_entry(Event const&, FSM&){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::trigger_red_alarm(const Perimetral_sensor_sig &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});
}

void Alarm_system_::trigger_red_alarm(const Int_wind_open_sensor_sig &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});
}

void Alarm_system_::trigger_red_alarm(const Res_wind_open_sensor_sig &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});
}

void Alarm_system_::trigger_red_alarm(const Int_motion_sensor_sig &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});
}

void Alarm_system_::trigger_red_alarm(const Res_motion_sensor_sig &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});
  
}

void Alarm_system_::trigger_orange_alarm(const Ext_motion_sensor_sig &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Orange_alarm{});
}

// void Alarm_system_::trigger_automatic_cancellation(const Rec_owner_in_ext &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Automatic_cancellation{});
// }

void Alarm_system_::send_video_chunk(const Ext_door_open_sensor_sig &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::send_video_chunk(const Int_door_open_sensor_sig &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::send_video_chunk(const Res_door_open_sensor_sig &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::counters_update(const Rec_owner_in_ext &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::counters_update_and_trigger_automatic_cancellation(const Rec_owner_in_ext &evt){
  counters_update(evt);
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Automatic_cancellation{});

}
