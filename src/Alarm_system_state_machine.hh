#ifndef _ALARM_SYSTEM_
#define _ALARM_SYSTEM_

#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>
// for func_state and func_state_machine
#include <boost/msm/front/euml/state_grammar.hpp>

#include "Alarm_system_state_machine_states.hh"
#include "Alarm_system_state_machine_events.hh"

#include "Risk_factor_state_machine_states.hh"
#include "Risk_factor_state_machine_events.hh"

#include "Occupancy_depth_state_machine_states.hh"
#include "Occupancy_depth_state_machine_events.hh"

#include "Internal_activity_state_machine_states.hh"
#include "Internal_activity_state_machine_events.hh"

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

// front-end: define the FSM structure
struct Alarm_system_ : public msm::front::state_machine_def<Alarm_system_>{

  typedef mpl::vector<Waiting_for_configuration, Waiting_for_risk, None, Idle> initial_state;
  
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&);

  //the action must be inserted before transition table where they are used
 
  //maybe this action could be collapsed into one using an superclass event from which each derives
  template<class Event_type>
  void trigger_red_alarm(const Event_type &evt);
  // void trigger_red_alarm(const Perimetral_sensor_sig &evt);
  // void trigger_red_alarm(const Int_wind_open_sensor_sig &evt);
  // void trigger_red_alarm(const Res_wind_open_sensor_sig &evt);
  // void trigger_red_alarm(const Int_motion_sensor_sig &evt);
  // void trigger_red_alarm(const Res_motion_sensor_sig &evt);   
  // void trigger_red_alarm(const Rec_unk_in_ext &evt);
  // void trigger_red_alarm(const Rec_unk_in_int &evt);
  // void trigger_red_alarm(const Rec_unk_in_res &evt);
  
  template<class Event_type>
  void trigger_orange_alarm(const Event_type &evt);
  // void trigger_orange_alarm(const Ext_motion_sensor_sig &evt);
  // void trigger_orange_alarm(const Rec_monit_in_ext &evt);
  // void trigger_orange_alarm(const Rec_monit_in_res &evt);
  

  //void trigger_automatic_cancellation(const Rec_owner_in_ext &evt);
 
  //maybe this action could be collapsed into one using an superclass event from which each derives
  // template<class Event_type>
  // virtual void send_video_chunk(const Event_type &evt);
  //impossibile to have template virtual methods
  virtual  void send_video_chunk(const Ext_door_open_sensor_sig &evt);
  virtual  void send_video_chunk(const Int_door_open_sensor_sig &evt);
  virtual  void send_video_chunk(const Res_door_open_sensor_sig &evt);

  virtual void increase_ai_response_counter(const Ext_door_open_sensor_sig &evt);
  virtual void increase_ai_response_counter(const Int_door_open_sensor_sig &evt);
  virtual void increase_ai_response_counter(const Res_door_open_sensor_sig &evt);  

  virtual void decrease_ai_response_counter(const Rec_owner_in_ext &evt);
  virtual void decrease_ai_response_counter(const Rec_owner_in_int &evt);
  virtual void decrease_ai_response_counter(const Rec_owner_in_res &evt);
  virtual void decrease_ai_response_counter(const Rec_monit_in_ext &evt);
  virtual void decrease_ai_response_counter(const Rec_monit_in_int &evt);
  virtual void decrease_ai_response_counter(const Rec_monit_in_res &evt);
  virtual void decrease_ai_response_counter(const Rec_unk_in_ext &evt);
  virtual void decrease_ai_response_counter(const Rec_unk_in_int &evt);
  virtual void decrease_ai_response_counter(const Rec_unk_in_res &evt);
  

  template<class Event_type>
  void ext_presence_flag_update(const Event_type &evt);  
  template<class Event_type>
  void int_presence_flag_update(const Event_type &evt);  
  template<class Event_type>
  void res_presence_flag_update(const Event_type &evt);  

  template<class Event_type>
  void ext_presence_flag_reset(const Event_type &evt);  
  template<class Event_type>
  void int_presence_flag_reset(const Event_type &evt);  
  template<class Event_type>
  void res_presence_flag_reset(const Event_type &evt);  


  // void presence_flag_update(const Rec_owner_in_ext &evt);
  // void presence_flag_update(const Rec_owner_in_int &evt);
  // void presence_flag_update(const Rec_owner_in_res &evt);
  // void presence_flag_update(const Rec_monit_in_ext &evt);
  // void presence_flag_update(const Rec_monit_in_int &evt);
  // void presence_flag_update(const Rec_monit_in_res &evt);
  // void presence_flag_update(const Rec_unk_in_ext &evt);
  // void presence_flag_update(const Rec_unk_in_int &evt);
  // void presence_flag_update(const Rec_unk_in_res &evt);
  

  template<class Event_type>
  void ext_presence_flag_update_and_trigger_automatic_cancellation(const Event_type &evt);
  template<class Event_type>
  void int_presence_flag_update_and_trigger_automatic_cancellation(const Event_type &evt);
  template<class Event_type>
  void res_presence_flag_update_and_trigger_automatic_cancellation(const Event_type &evt);
  // void presence_flag_update_and_trigger_automatic_cancellation(const Rec_owner_in_ext &evt);
  // void presence_flag_update_and_trigger_automatic_cancellation(const Rec_owner_in_int &evt);
  // void presence_flag_update_and_trigger_automatic_cancellation(const Rec_owner_in_res &evt);

  template<class Event_type>
  void ext_presence_flag_update_and_trigger_orange_alarm(const Event_type &evt);
  template<class Event_type>
  void int_presence_flag_update_and_trigger_orange_alarm(const Event_type &evt);
  template<class Event_type>
  void res_presence_flag_update_and_trigger_orange_alarm(const Event_type &evt);
  // void presence_flag_update_and_trigger_orange_alarm(const Rec_monit_in_ext &evt);
  // void presence_flag_update_and_trigger_orange_alarm(const Rec_monit_in_res &evt);

  template<class Event_type>
  void ext_presence_flag_update_and_trigger_red_alarm(const Event_type &evt);  
  template<class Event_type>
  void int_presence_flag_update_and_trigger_red_alarm(const Event_type &evt);  
  template<class Event_type>
  void res_presence_flag_update_and_trigger_red_alarm(const Event_type &evt);  

  // void presence_flag_update_and_trigger_red_alarm(const Rec_unk_in_ext &evt);
  // void presence_flag_update_and_trigger_red_alarm(const Rec_unk_in_int &evt);
  // void presence_flag_update_and_trigger_red_alarm(const Rec_unk_in_res &evt);
  void reset_presence_flags(const Reset_risk &evt);

  bool is_ext_empty();
  bool is_int_empty();
  bool is_res_empty();

  template<class Event_type>  
  void trigger_red_alarm_if_ext_empty(const Event_type &evt);
  template<class Event_type>  
  void trigger_red_alarm_if_int_empty(const Event_type &evt);
  
  template<class Event_type>
  void send_video_chunk_and_trigger_orange_alarm(const Event_type &evt);  

  template<class Event_type>
  void send_video_chunk_and_trigger_red_alarm(const Event_type &evt);  

  struct transition_table : mpl::vector<
    //    Start                       Event                     Target                     Action                      Guard
    //+-----------------------------+-------------------------+--------------------------+---------------------------+-----------
    // Alarm system state machine orthogonal region
    //Row   <Init,                      Switch_on,                Waiting_for_configuration, none,                       none     >,
    // +-----------------------------+---------------------------+----------------------------+---------------------------+-----------
    Row   <Waiting_for_configuration, Initialization_completed, Green_alarm,               none,                       none     >,
    // +-----------------------------+-------------------------+----------------------------+---------------------------+-----------
    Row   <Green_alarm,              Orange_alarm,             Orange_alarm_notified,      none,                       none     >,
    Row   <Green_alarm,              Red_alarm,                Red_alarm_notified,         none,                       none     >,
    Row   <Green_alarm,              Switch_off,               Exit,                       none,                       none     >,
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    Row   <Orange_alarm_notified,    User_cancellation,        Green_alarm,                none,                       none     >,
    Row   <Orange_alarm_notified,    User_confirmation,        Red_alarm_actuation,        none,                       none     >,
    Row   <Orange_alarm_notified,    Automatic_cancellation,   Green_alarm,                none,                       none     >,
    Row   <Orange_alarm_notified,    Red_alarm_timeout,        Red_alarm_actuation,        none,                       none     >,
    Row   <Orange_alarm_notified,    Orange_alarm,             none,                       none,                       none     >,    
    Row   <Orange_alarm_notified,    Red_alarm,                Red_alarm_notified,         none,                       none     >,    
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    Row   <Red_alarm_notified,       User_cancellation,        Green_alarm,                none,                       none     >,
    Row   <Red_alarm_notified,       Red_alarm,                none,                       none,                       none     >,
    Row   <Red_alarm_notified,       Orange_alarm,             none,                       none,                       none     >,
    Row   <Red_alarm_notified,       Automatic_cancellation,   Green_alarm,                none,                       none     >,    
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    Row   <Red_alarm_actuation,      Stop_actuation,           Green_alarm,                 none,                       none     >,
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    Row   <Exit,                     Switch_on,                Green_alarm,                 none,                       none     >,
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    //Risk factor state machine orthogonal region
    a_row <Waiting_for_risk,         Perimetral_sensor_sig,    Evaluating_risk,             &Alarm_system_::trigger_red_alarm<Perimetral_sensor_sig>    >,
    a_row <Waiting_for_risk,         Ext_door_open_sensor_sig, Evaluating_risk,             &Alarm_system_::send_video_chunk                             >,
    a_row <Waiting_for_risk,        Int_door_open_sensor_sig, Evaluating_risk,             &Alarm_system_::send_video_chunk                              >,
    a_row <Waiting_for_risk,        Res_door_open_sensor_sig, Evaluating_risk,             &Alarm_system_::send_video_chunk                               >,
    a_row <Waiting_for_risk,        Int_wind_open_sensor_sig, Evaluating_risk,             &Alarm_system_::trigger_red_alarm<Int_wind_open_sensor_sig>    >,
    a_row <Waiting_for_risk,        Res_wind_open_sensor_sig, Evaluating_risk,             &Alarm_system_::trigger_red_alarm<Res_wind_open_sensor_sig>    >,
    a_row <Waiting_for_risk,        Ext_motion_sensor_sig,    Evaluating_risk,             &Alarm_system_::trigger_orange_alarm<Ext_motion_sensor_sig> >,
    a_row <Waiting_for_risk,        Int_motion_sensor_sig,    Evaluating_risk,             &Alarm_system_::trigger_red_alarm<Int_motion_sensor_sig>    >,
    a_row <Waiting_for_risk,        Res_motion_sensor_sig,    Evaluating_risk,             &Alarm_system_::trigger_red_alarm<Res_motion_sensor_sig>    >,
    // +---------------------------+-------------------------+----------------------------+---------------------------+-----------
    a_irow<Evaluating_risk,         Perimetral_sensor_sig,                                 &Alarm_system_::trigger_red_alarm<Perimetral_sensor_sig>    >,
    a_irow<Evaluating_risk,         Ext_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk     >,
    a_irow<Evaluating_risk,         Int_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk     >,
    a_irow<Evaluating_risk,         Res_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk     >,
    a_irow<Evaluating_risk,         Ext_motion_sensor_sig,                                 &Alarm_system_::trigger_orange_alarm<Ext_motion_sensor_sig> >,
    a_irow<Evaluating_risk,         Int_motion_sensor_sig,                                 &Alarm_system_::trigger_red_alarm<Int_motion_sensor_sig>    >,
    a_irow<Evaluating_risk,         Res_motion_sensor_sig,                                 &Alarm_system_::trigger_red_alarm<Res_motion_sensor_sig>    >,
    a_row<Evaluating_risk,          Rec_owner_in_ext,         Low_risk,                    &Alarm_system_::ext_presence_flag_update<Rec_owner_in_ext> >,
    a_row<Evaluating_risk,          Rec_owner_in_int,         Low_risk,                    &Alarm_system_::int_presence_flag_update<Rec_owner_in_int> >,
    a_row<Evaluating_risk,          Rec_owner_in_res,         Low_risk,                    &Alarm_system_::res_presence_flag_update<Rec_owner_in_res> >,        
    a_row<Evaluating_risk,          Rec_monit_in_ext,         Medium_risk,                 &Alarm_system_::ext_presence_flag_update_and_trigger_orange_alarm<Rec_monit_in_ext> >,
    a_row<Evaluating_risk,          Rec_monit_in_int,         Medium_risk,                 &Alarm_system_::int_presence_flag_update<Rec_monit_in_int>                          >,
    a_row<Evaluating_risk,          Rec_monit_in_res,         Medium_risk,                 &Alarm_system_::res_presence_flag_update_and_trigger_orange_alarm<Rec_monit_in_res> >,
    a_row<Evaluating_risk,          Rec_unk_in_ext,           High_risk,                   &Alarm_system_::ext_presence_flag_update_and_trigger_red_alarm<Rec_unk_in_ext> >,
    a_row<Evaluating_risk,          Rec_unk_in_int,           High_risk,                   &Alarm_system_::int_presence_flag_update_and_trigger_red_alarm<Rec_unk_in_int> >,
    a_row<Evaluating_risk,          Rec_unk_in_res,           High_risk,                   &Alarm_system_::res_presence_flag_update_and_trigger_red_alarm<Rec_unk_in_res> >,         
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    a_irow<Low_risk,                Perimetral_sensor_sig,                                 &Alarm_system_::trigger_orange_alarm<Perimetral_sensor_sig> >,
    a_irow<Low_risk,                Ext_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk                            >,
    a_irow<Low_risk,                Int_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk                          >,
    a_irow<Low_risk,                Res_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk                          >,
    a_irow<Low_risk,                Rec_owner_in_ext,                                      &Alarm_system_::ext_presence_flag_update<Rec_owner_in_ext> >,    
    a_irow<Low_risk,                Rec_owner_in_int,                                      &Alarm_system_::int_presence_flag_update<Rec_owner_in_int> >,
    a_irow<Low_risk,                Rec_owner_in_res,                                      &Alarm_system_::res_presence_flag_update<Rec_owner_in_res> >,
    a_irow<Low_risk,                Rec_monit_in_ext,                                      &Alarm_system_::ext_presence_flag_update<Rec_monit_in_ext> >,    
    a_irow<Low_risk,                Rec_monit_in_int,                                      &Alarm_system_::int_presence_flag_update<Rec_monit_in_int> >,
    a_irow<Low_risk,                Rec_monit_in_res,                                      &Alarm_system_::res_presence_flag_update<Rec_monit_in_res> >,
    a_irow<Low_risk,                Rec_unk_in_ext,                                        &Alarm_system_::ext_presence_flag_update<Rec_unk_in_ext> >,   
    a_row<Low_risk,                 Reset_risk,               Waiting_for_risk,            &Alarm_system_::reset_presence_flags                     >,
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    a_irow<Medium_risk,             Perimetral_sensor_sig,                                 &Alarm_system_::trigger_red_alarm_if_ext_empty<Perimetral_sensor_sig> >,
    a_irow<Medium_risk,             Ext_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk                           >,
    a_irow<Medium_risk,             Int_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk                           >,
    a_irow<Medium_risk,             Res_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk_and_trigger_orange_alarm<Res_door_open_sensor_sig> >,
    a_irow<Medium_risk,             Ext_motion_sensor_sig,                                 &Alarm_system_::trigger_red_alarm_if_ext_empty<Ext_motion_sensor_sig> >,
    a_irow<Medium_risk,             Int_motion_sensor_sig,                                 &Alarm_system_::trigger_red_alarm_if_int_empty<Int_motion_sensor_sig> >,
    a_irow<Medium_risk,             Res_motion_sensor_sig,                                 &Alarm_system_::trigger_red_alarm<Res_motion_sensor_sig> >,
    a_row<Medium_risk,              Rec_owner_in_ext,          Low_risk,                   &Alarm_system_::ext_presence_flag_update_and_trigger_automatic_cancellation<Rec_owner_in_ext> >,
    a_row<Medium_risk,              Rec_owner_in_int,          Low_risk,                   &Alarm_system_::int_presence_flag_update_and_trigger_automatic_cancellation<Rec_owner_in_int> >,
    a_row<Medium_risk,              Rec_owner_in_res,          Low_risk,                   &Alarm_system_::res_presence_flag_update_and_trigger_automatic_cancellation<Rec_owner_in_res> >,
    a_irow<Medium_risk,             Rec_monit_in_ext,                                      &Alarm_system_::ext_presence_flag_update<Rec_monit_in_ext> >,
    a_irow<Medium_risk,             Rec_monit_in_int,                                      &Alarm_system_::int_presence_flag_update<Rec_monit_in_int> >,
    a_irow<Medium_risk,             Rec_monit_in_res,                                      &Alarm_system_::res_presence_flag_update_and_trigger_orange_alarm<Rec_monit_in_res> >,
    a_row<Medium_risk,              Rec_unk_in_ext,            High_risk,                  &Alarm_system_::ext_presence_flag_update_and_trigger_red_alarm<Rec_unk_in_ext> >,
    a_row<Medium_risk,              Rec_unk_in_int,            High_risk,                  &Alarm_system_::int_presence_flag_update_and_trigger_red_alarm<Rec_unk_in_int> >,
    a_row<Medium_risk,              Rec_unk_in_res,            High_risk,                  &Alarm_system_::res_presence_flag_update_and_trigger_red_alarm<Rec_unk_in_res> >,
    a_row<Medium_risk,              Reset_risk,                Waiting_for_risk,           &Alarm_system_::reset_presence_flags>,
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    a_irow<High_risk,               Perimetral_sensor_sig,                                 &Alarm_system_::trigger_red_alarm<Perimetral_sensor_sig> >,
    a_irow<High_risk,               Ext_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk_and_trigger_red_alarm<Ext_door_open_sensor_sig> >,
    a_irow<High_risk,               Int_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk_and_trigger_red_alarm<Int_door_open_sensor_sig> >,
    a_irow<High_risk,               Res_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk_and_trigger_red_alarm<Res_door_open_sensor_sig> >,
    a_irow<High_risk,               Ext_motion_sensor_sig,                                 &Alarm_system_::trigger_red_alarm<Ext_motion_sensor_sig> >,
    a_irow<High_risk,               Int_motion_sensor_sig,                                 &Alarm_system_::trigger_red_alarm<Int_motion_sensor_sig> >,
    a_irow<High_risk,               Res_motion_sensor_sig,                                 &Alarm_system_::trigger_red_alarm<Res_motion_sensor_sig> >,        
    a_row<High_risk,                Rec_owner_in_ext,          Low_risk,                   &Alarm_system_::ext_presence_flag_update_and_trigger_automatic_cancellation<Rec_owner_in_ext> >,
    a_row<High_risk,                Rec_owner_in_int,          Low_risk,                   &Alarm_system_::int_presence_flag_update_and_trigger_automatic_cancellation<Rec_owner_in_int> >,
    a_row<High_risk,                Rec_owner_in_res,          Low_risk,                   &Alarm_system_::res_presence_flag_update_and_trigger_automatic_cancellation<Rec_owner_in_res> >,
    a_irow<High_risk,               Rec_monit_in_ext,                                      &Alarm_system_::ext_presence_flag_update_and_trigger_orange_alarm<Rec_monit_in_ext> >,
    a_irow<High_risk,               Rec_monit_in_int,                                      &Alarm_system_::int_presence_flag_update_and_trigger_orange_alarm<Rec_monit_in_int> >,
    a_irow<High_risk,               Rec_monit_in_res,                                      &Alarm_system_::res_presence_flag_update_and_trigger_orange_alarm<Rec_monit_in_res> >,
    a_irow<High_risk,               Rec_unk_in_ext,                                        &Alarm_system_::ext_presence_flag_update_and_trigger_red_alarm<Rec_unk_in_ext> >,
    a_irow<High_risk,               Rec_unk_in_int,                                        &Alarm_system_::int_presence_flag_update_and_trigger_red_alarm<Rec_unk_in_int> >,
    a_irow<High_risk,               Rec_unk_in_res,                                        &Alarm_system_::res_presence_flag_update_and_trigger_red_alarm<Rec_unk_in_res> >,
    a_row<High_risk,                Reset_risk,                Waiting_for_risk,           &Alarm_system_::reset_presence_flags>,
    //Occupancy_depth state machine transitions
    // +----------------------------+-------------------------+----------------------------+--------------------------------------------------------------------------------+-----------
    _row<None,                      Perimetral_sensor_sig,     Extern                                                                                                     >,
    _row<None,                      Ext_door_open_sensor_sig,  Extern                                                                                                     >,
    //Interal state name gives me error
    a_row<None,                     Int_door_open_sensor_sig,  Intern,                   &Alarm_system_::trigger_orange_alarm<Int_door_open_sensor_sig> >,
    a_row<None,                     Res_door_open_sensor_sig,  Reserved,                   &Alarm_system_::trigger_red_alarm<Res_door_open_sensor_sig> >,
    _row<None,                      Int_wind_open_sensor_sig,  Intern                                                                                                     >,
    _row<None,                      Res_wind_open_sensor_sig,  Reserved                                                                                                     >,
    _row<None,                      Ext_motion_sensor_sig,     Extern                                                                                                     >,
    _row<None,                      Int_motion_sensor_sig,     Intern                                                                                                     >,
    _row<None,                      Res_motion_sensor_sig,     Reserved                                                                                                     >,
    // +----------------------------+-------------------------+----------------------------+--------------------------------------------------------------------------------+-----------    
    _row<Extern,                    Int_door_open_sensor_sig,  Intern                                                                                                    >,
    _row<Extern,                    Res_door_open_sensor_sig,  Reserved                                                                                                  >,
    a_row<Extern,                   Int_wind_open_sensor_sig,  Intern,                        &Alarm_system_::trigger_orange_alarm<Int_wind_open_sensor_sig>              >,
    a_row<Extern,                   Res_wind_open_sensor_sig,  Reserved,                      &Alarm_system_::trigger_red_alarm<Res_wind_open_sensor_sig>              >,
    a_row<Extern,                   Int_motion_sensor_sig,     Intern,                        &Alarm_system_::trigger_orange_alarm<Int_motion_sensor_sig>              >,
    a_row<Extern,                   Res_motion_sensor_sig,     Reserved,                      &Alarm_system_::trigger_red_alarm<Res_motion_sensor_sig>                  >,
    _row<Extern,                    Rec_owner_in_int,          Intern                                                                                                    >,
    _row<Extern,                    Rec_monit_in_int,          Intern                                                                                                    >,
    a_row<Extern,                   Rec_unk_in_int,            Intern,                         &Alarm_system_::trigger_red_alarm<Rec_unk_in_int>                         >,
    _row<Extern,                    Rec_owner_in_res,          Reserved                                                                                                    >,
    _row<Extern,                    Rec_monit_in_res,          Reserved                                                                                                    >,
    a_row<Extern,                   Rec_unk_in_res,            Reserved,                       &Alarm_system_::trigger_red_alarm<Rec_unk_in_res>                         >,
    a_row<Extern,                   Clear_ext,                 None,                           &Alarm_system_::ext_presence_flag_reset<Clear_ext>                         >,
    // +----------------------------+-------------------------+----------------------------+--------------------------------------------------------------------------------+-----------
    _row<Intern,                    Res_door_open_sensor_sig,  Reserved                                                                                                    >,
    _row<Intern,                    Res_wind_open_sensor_sig,  Reserved                                                                                                    >,
    _row<Intern,                    Res_motion_sensor_sig,     Reserved                                                                                                    >,
    _row<Intern,                    Rec_owner_in_res,          Reserved                                                                                                    >,
    a_row<Intern,                   Rec_monit_in_res,          Reserved,                        &Alarm_system_::trigger_orange_alarm<Rec_monit_in_res>                      >,
    a_row<Intern,                   Clear_int,                 Extern,                          &Alarm_system_::int_presence_flag_reset<Clear_int>                      >,
    a_irow<Reserved,                Rec_monit_in_res,                                           &Alarm_system_::trigger_orange_alarm<Rec_monit_in_res>                      >,
    a_irow<Reserved,                Rec_unk_in_res,                                             &Alarm_system_::trigger_red_alarm<Rec_unk_in_res>                      >,
    a_row<Reserved,                 Clear_res,                 Intern,                          &Alarm_system_::res_presence_flag_reset<Clear_res>                         >,
    //Intenal_activity state machine transitions
    // +----------------------------+-------------------------+----------------------------+--------------------------------------------------------------------------------+-----------
    a_row<Idle,                     Ext_door_open_sensor_sig,  Waiting_for_ai_response,         &Alarm_system_::increase_ai_response_counter       >,
    a_row<Idle,                     Int_door_open_sensor_sig,  Waiting_for_ai_response,         &Alarm_system_::increase_ai_response_counter       >,
    a_row<Idle,                     Res_door_open_sensor_sig,  Waiting_for_ai_response,         &Alarm_system_::increase_ai_response_counter       >,
    // +----------------------------+-------------------------+----------------------------+--------------------------------------------------------------------------------+-----------
    a_irow<Waiting_for_ai_response, Rec_owner_in_ext,                                           &Alarm_system_::decrease_ai_response_counter               >,
    a_irow<Waiting_for_ai_response, Rec_monit_in_ext,                                           &Alarm_system_::decrease_ai_response_counter               >,
    a_irow<Waiting_for_ai_response, Rec_unk_in_ext,                                             &Alarm_system_::decrease_ai_response_counter                 >,
    a_irow<Waiting_for_ai_response, Rec_owner_in_int,                                           &Alarm_system_::decrease_ai_response_counter               >,
    a_irow<Waiting_for_ai_response, Rec_monit_in_int,                                           &Alarm_system_::decrease_ai_response_counter               >,
    a_irow<Waiting_for_ai_response, Rec_unk_in_int,                                             &Alarm_system_::decrease_ai_response_counter               >,
    a_irow<Waiting_for_ai_response, Rec_owner_in_res,                                           &Alarm_system_::decrease_ai_response_counter              >,
    a_irow<Waiting_for_ai_response, Rec_monit_in_res,                                           &Alarm_system_::decrease_ai_response_counter               >,
    a_irow<Waiting_for_ai_response, Rec_unk_in_res,                                             &Alarm_system_::decrease_ai_response_counter                >,
    _row<Waiting_for_ai_response,   Ai_response_off,            Idle                                                                                                          >
    > {};

  // Replaces the default no-transition response.
  template <class FSM,class Event>
  void no_transition(Event const& e, FSM&, int state){
    D(std::cout << "no transition from state " << state
      << " on event " << typeid(e).name() << std::endl);
  }
};

// back-end
typedef msm::back::state_machine<Alarm_system_> Alarm_system;

#include "Alarm_system_state_machine.cxx"

#endif
