#define DEBUG

#include "Alarm_system_state_machine.hh"
#include <iostream>
#include "Utils.hh"
#include <boost/core/ignore_unused.hpp>

template <class Event, class FSM>
void Alarm_system_::on_entry(Event const&, FSM&){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

template <class Event_type>
void Alarm_system_::trigger_red_alarm(const Event_type &evt){
  boost::ignore_unused(evt);
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});
  send_classified_notification_as('r', evt._sensor_mini_id);
}

template <class Event_type>
void Alarm_system_::trigger_orange_alarm(const Event_type &evt){
  boost::ignore_unused(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Orange_alarm{});
  send_classified_notification_as('o', evt._sensor_mini_id);
}

// template<class Event_type>
// void Alarm_system_::ext_presence_flag_update(const Event_type &evt){
//     boost::ignore_unused(evt);
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// template<class Event_type>
// void Alarm_system_::int_presence_flag_update(const Event_type &evt){
//   boost::ignore_unused(evt);
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// template<class Event_type>
// void Alarm_system_::res_presence_flag_update(const Event_type &evt){
//   boost::ignore_unused(evt);
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// template<class Event_type>
// void Alarm_system_::ext_presence_flag_reset(const Event_type &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// template<class Event_type>
// void Alarm_system_::int_presence_flag_reset(const Event_type &evt){
//   D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
// }

// template<class Event_type>
// void Alarm_system_::res_presence_flag_reset(const Event_type &evt){
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

template<class Event_type>
void Alarm_system_::ext_presence_flag_update_and_trigger_orange_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);  
  ext_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Orange_alarm{});
  send_classified_notification_av('o', evt._mmuid);
}

template<class Event_type>
void Alarm_system_::int_presence_flag_update_and_trigger_orange_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  int_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Orange_alarm{});
  send_classified_notification_av('o', evt._mmuid);
}

template<class Event_type>
void Alarm_system_::res_presence_flag_update_and_trigger_orange_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  res_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Orange_alarm{});
  send_classified_notification_av('o', evt._mmuid);
}

template<class Event_type>
void Alarm_system_::ext_presence_flag_update_and_trigger_red_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  ext_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});
  send_classified_notification_av('r', evt._mmuid);
}

template<class Event_type>
void Alarm_system_::int_presence_flag_update_and_trigger_red_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  int_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});
  send_classified_notification_av('r', evt._mmuid);
}

template<class Event_type>
void Alarm_system_::res_presence_flag_update_and_trigger_red_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  res_presence_flag_update(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});
  send_classified_notification_av('r', evt._mmuid);
}

template <class Event_type>
void Alarm_system_::trigger_red_alarm_if_ext_empty(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  if( is_ext_empty() ){
    msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
   fsm.process_event(Red_alarm{});
   send_classified_notification_as('r', evt._sensor_mini_id);
  }
}
template <class Event_type>
void Alarm_system_::trigger_red_alarm_if_int_empty(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  if( is_ext_empty() ){
    msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
   fsm.process_event(Red_alarm{});
   send_classified_notification_as('r', evt._sensor_mini_id);
  }
}

template<class Event_type>
void Alarm_system_::send_video_chunk_and_trigger_orange_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  send_video_chunk(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Orange_alarm{});
  send_classified_notification_as('o', evt._sensor_mini_id);
}

template<class Event_type>
void Alarm_system_::send_video_chunk_and_trigger_red_alarm(const Event_type &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  send_video_chunk(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Red_alarm{});
  send_classified_notification_as('r', evt._sensor_mini_id);
}



