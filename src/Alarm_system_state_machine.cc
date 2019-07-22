#define DEBUG

#include "Alarm_system_state_machine.hh"
#include <iostream>
#include "Utils.hh"

template <class Event, class FSM>
void Alarm_system_::on_entry(Event const&, FSM&){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

template <class Event_type>
void Alarm_system_::trigger_red_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});
}

// void Alarm_system_::trigger_red_alarm(const Perimetral_sensor_sig &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Red_alarm{});
// }

// void Alarm_system_::trigger_red_alarm(const Int_wind_open_sensor_sig &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Red_alarm{});
// }

// void Alarm_system_::trigger_red_alarm(const Res_wind_open_sensor_sig &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Red_alarm{});
// }

// void Alarm_system_::trigger_red_alarm(const Int_motion_sensor_sig &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Red_alarm{});
// }

// void Alarm_system_::trigger_red_alarm(const Res_motion_sensor_sig &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Red_alarm{});
  
// }

// void Alarm_system_::trigger_red_alarm(const Rec_unk_in_ext &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Red_alarm{});
  
// }

// void Alarm_system_::trigger_red_alarm(const Rec_unk_in_int &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Red_alarm{});
  
// }

// void Alarm_system_::trigger_red_alarm(const Rec_unk_in_res &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Red_alarm{});
  
// }

template <class Event_type>
void Alarm_system_::trigger_orange_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Orange_alarm{});
}

// void Alarm_system_::trigger_orange_alarm(const Ext_motion_sensor_sig &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Orange_alarm{});
// }

// void Alarm_system_::trigger_automatic_cancellation(const Rec_owner_in_ext &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Automatic_cancellation{});
// }

template<class Event_type>
void Alarm_system_::send_video_chunk(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

// void Alarm_system_::send_video_chunk(const Ext_door_open_sensor_sig &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// void Alarm_system_::send_video_chunk(const Int_door_open_sensor_sig &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// void Alarm_system_::send_video_chunk(const Res_door_open_sensor_sig &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

template<class Event_type>
void Alarm_system_::ext_presence_flag_update(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

template<class Event_type>
void Alarm_system_::int_presence_flag_update(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

template<class Event_type>
void Alarm_system_::res_presence_flag_update(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

// void Alarm_system_::presence_flag_update(const Rec_owner_in_ext &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// void Alarm_system_::presence_flag_update(const Rec_owner_in_int &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// void Alarm_system_::presence_flag_update(const Rec_owner_in_res &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// void Alarm_system_::presence_flag_update(const Rec_monit_in_ext &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// void Alarm_system_::presence_flag_update(const Rec_monit_in_int &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// void Alarm_system_::presence_flag_update(const Rec_monit_in_res &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// void Alarm_system_::presence_flag_update(const Rec_unk_in_ext &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// void Alarm_system_::presence_flag_update(const Rec_unk_in_res &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// void Alarm_system_::presence_flag_update(const Rec_unk_in_int &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

template<class Event_type>
void Alarm_system_::ext_presence_flag_update_and_trigger_automatic_cancellation(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  ext_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Automatic_cancellation{});
}

template<class Event_type>
void Alarm_system_::int_presence_flag_update_and_trigger_automatic_cancellation(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  int_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Automatic_cancellation{});
}

template<class Event_type>
void Alarm_system_::res_presence_flag_update_and_trigger_automatic_cancellation(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);  
  res_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Automatic_cancellation{});
}

// void Alarm_system_::presence_flag_update_and_trigger_automatic_cancellation(const Rec_owner_in_ext &evt){
//   presence_flag_update(evt);
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Automatic_cancellation{});

// }

// void Alarm_system_::presence_flag_update_and_trigger_automatic_cancellation(const Rec_owner_in_int &evt){
//   presence_flag_update(evt);
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Automatic_cancellation{});

// }

// void Alarm_system_::presence_flag_update_and_trigger_automatic_cancellation(const Rec_owner_in_res &evt){
//   presence_flag_update(evt);
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Automatic_cancellation{});
// }

template<class Event_type>
void Alarm_system_::ext_presence_flag_update_and_trigger_orange_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);  
  ext_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Orange_alarm{});
}

template<class Event_type>
void Alarm_system_::int_presence_flag_update_and_trigger_orange_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  int_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Orange_alarm{});
}

template<class Event_type>
void Alarm_system_::res_presence_flag_update_and_trigger_orange_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  res_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Orange_alarm{});
}

// void Alarm_system_::presence_flag_update_and_trigger_orange_alarm(const Rec_monit_in_ext &evt){
//   presence_flag_update(evt);
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Orange_alarm{});
// }

// void Alarm_system_::presence_flag_update_and_trigger_orange_alarm(const Rec_monit_in_res &evt){
//   presence_flag_update(evt);
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Orange_alarm{});
// }
template<class Event_type>
void Alarm_system_::ext_presence_flag_update_and_trigger_red_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  ext_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});  
}

template<class Event_type>
void Alarm_system_::int_presence_flag_update_and_trigger_red_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  int_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});  
}

template<class Event_type>
void Alarm_system_::res_presence_flag_update_and_trigger_red_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  res_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});  
}

// void Alarm_system_::presence_flag_update_and_trigger_red_alarm(const Rec_unk_in_ext &evt){
//   presence_flag_update(evt);
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Red_alarm{});
// }

// void Alarm_system_::presence_flag_update_and_trigger_red_alarm(const Rec_unk_in_int &evt){
//   presence_flag_update(evt);
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Red_alarm{});
// }

// void Alarm_system_::presence_flag_update_and_trigger_red_alarm(const Rec_unk_in_res &evt){
//   presence_flag_update(evt);
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
//   msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
//   fsm.process_event(Red_alarm{});
// }

void Alarm_system_::reset_presence_flags(const Reset_risk &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

bool Alarm_system_::is_ext_empty(){
  return true;
}

bool Alarm_system_::is_int_empty(){
  return true;
}

bool Alarm_system_::is_res_empty(){
  return true;
}

template <class Event_type>
void Alarm_system_::trigger_red_alarm_if_ext_empty(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  if( is_ext_empty() ){
    msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
   fsm.process_event(Red_alarm{});  
  }
}
template <class Event_type>
void Alarm_system_::trigger_red_alarm_if_int_empty(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  if( is_ext_empty() ){
    msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
   fsm.process_event(Red_alarm{});  
  }
}

template<class Event_type>
void Alarm_system_::send_video_chunk_and_trigger_orange_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  send_video_chunk(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Orange_alarm{});  
}

template<class Event_type>
void Alarm_system_::send_video_chunk_and_trigger_red_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  send_video_chunk(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});  
}

