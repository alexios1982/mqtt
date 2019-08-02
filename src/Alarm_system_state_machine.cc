#include "Alarm_system_state_machine.hh"
#include "Utils.hh"

void Alarm_system_::send_video_chunk(const Ext_door_open_sensor_sig &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::send_video_chunk(const Int_door_open_sensor_sig &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::send_video_chunk(const Res_door_open_sensor_sig &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::send_classified_notification_av(char , const std::string &){ 
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::increase_ai_response_counter(const Ext_door_open_sensor_sig &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::increase_ai_response_counter(const Int_door_open_sensor_sig &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::increase_ai_response_counter(const Res_door_open_sensor_sig &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::decrease_ai_response_counter(const Rec_owner_in_ext &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::decrease_ai_response_counter(const Rec_owner_in_int &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::decrease_ai_response_counter(const Rec_owner_in_res &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::decrease_ai_response_counter(const Rec_monit_in_ext &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::decrease_ai_response_counter(const Rec_monit_in_int &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::decrease_ai_response_counter(const Rec_monit_in_res &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::decrease_ai_response_counter(const Rec_unk_in_ext &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::decrease_ai_response_counter(const Rec_unk_in_int &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::decrease_ai_response_counter(const Rec_unk_in_res &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::reset_presence_flags(const Reset_risk &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::ext_presence_flag_update(const Rec_owner_in_ext &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::ext_presence_flag_update(const Rec_monit_in_ext &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::ext_presence_flag_update(const Rec_unk_in_ext &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::int_presence_flag_update(const Rec_owner_in_int &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::int_presence_flag_update(const Rec_monit_in_int &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::int_presence_flag_update(const Rec_unk_in_int &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::res_presence_flag_update(const Rec_owner_in_res &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::res_presence_flag_update(const Rec_monit_in_res &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::res_presence_flag_update(const Rec_unk_in_res &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::ext_presence_flag_reset(const Clear_ext &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}
void Alarm_system_::int_presence_flag_reset(const Clear_int &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
}

void Alarm_system_::res_presence_flag_reset(const Clear_res &){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);  
}

void Alarm_system_::ext_presence_flag_reset_and_trigger_reset_risk(const Clear_ext &evt){
  D(std::cout << info << "[Alarm_sytem::" << __func__ << "]. "  << reset << std::endl);
  ext_presence_flag_reset(evt);
  msm::back::state_machine<Alarm_system_> &fsm = static_cast<msm::back::state_machine<Alarm_system_> &>(*this);
  fsm.process_event(Reset_risk{});
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
