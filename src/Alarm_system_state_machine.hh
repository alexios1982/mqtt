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

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

// front-end: define the FSM structure
struct Alarm_system_ : public msm::front::state_machine_def<Alarm_system_>{

  typedef mpl::vector<Init, Waiting_for_risk> initial_state;
  
  template <class Event, class FSM>
  void on_entry(Event const&, FSM&);

  //the action must be inserted before transition table where they are used
 
  //maybe this action could be collapsed into one using an superclass event from wheach each derives
  void trigger_red_alarm(const Perimetral_sensor_sig &evt);
  void trigger_red_alarm(const Int_wind_open_sensor_sig &evt);
  void trigger_red_alarm(const Res_wind_open_sensor_sig &evt);
  void trigger_red_alarm(const Int_motion_sensor_sig &evt);
  void trigger_red_alarm(const Res_motion_sensor_sig &evt);   
  void trigger_orange_alarm(const Ext_motion_sensor_sig &evt);
  //void trigger_automatic_cancellation(const Rec_owner_in_ext &evt);
  
 
  //maybe this action could be collapsed into one using an superclass event from wheach each derives
  void send_video_chunk(const Ext_door_open_sensor_sig &evt);
  void send_video_chunk(const Int_door_open_sensor_sig &evt);
  void send_video_chunk(const Res_door_open_sensor_sig &evt);

  void counters_update(const Rec_owner_in_ext &evt);

  void counters_update_and_trigger_automatic_cancellation(const Rec_owner_in_ext &evt);
  
  struct transition_table : mpl::vector<
    //    Start                       Event                     Target                     Action                      Guard
    //+-----------------------------+-------------------------+--------------------------+---------------------------+-----------
    // Alarm system state machine orthogonal region
    Row   <Init,                      Switch_on,                Waiting_for_configuration, none,                       none     >,
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
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    Row   <Red_alarm_notified,       User_cancellation,        Green_alarm,                none,                       none     >,
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    Row   <Red_alarm_actuation,      Stop_actuation,           Green_alarm,                 none,                       none     >,
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    Row   <Exit,                     Switch_on,                Green_alarm,                 none,                       none     >,
    // +----------------------------+-------------------------+----------------------------+---------------------------+-----------
    //Risk factor state machine orthogonal region
    a_row <Waiting_for_risk,         Perimetral_sensor_sig,    Evaluating_risk,             &Alarm_system_::trigger_red_alarm    >,
    a_row <Waiting_for_risk,         Ext_door_open_sensor_sig, Evaluating_risk,             &Alarm_system_::send_video_chunk     >,
    a_row <Waiting_for_risk,        Int_door_open_sensor_sig, Evaluating_risk,             &Alarm_system_::send_video_chunk     >,
    a_row <Waiting_for_risk,        Res_door_open_sensor_sig, Evaluating_risk,             &Alarm_system_::send_video_chunk     >,
    a_row <Waiting_for_risk,        Int_wind_open_sensor_sig, Evaluating_risk,             &Alarm_system_::trigger_red_alarm    >,
    a_row <Waiting_for_risk,        Res_wind_open_sensor_sig, Evaluating_risk,             &Alarm_system_::trigger_red_alarm    >,
    a_row <Waiting_for_risk,        Ext_motion_sensor_sig,    Evaluating_risk,             &Alarm_system_::trigger_orange_alarm >,
    a_row <Waiting_for_risk,        Int_motion_sensor_sig,    Evaluating_risk,             &Alarm_system_::trigger_red_alarm    >,
    a_row <Waiting_for_risk,        Res_motion_sensor_sig,    Evaluating_risk,             &Alarm_system_::trigger_red_alarm    >,
    // +---------------------------+-------------------------+----------------------------+---------------------------+-----------
    a_irow<Evaluating_risk,         Perimetral_sensor_sig,                                 &Alarm_system_::trigger_red_alarm    >,
    a_irow<Evaluating_risk,         Ext_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk     >,
    a_irow<Evaluating_risk,         Int_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk     >,
    a_irow<Evaluating_risk,         Res_door_open_sensor_sig,                              &Alarm_system_::send_video_chunk     >,
    a_irow<Evaluating_risk,         Ext_motion_sensor_sig,                                 &Alarm_system_::trigger_orange_alarm >,
    a_irow<Evaluating_risk,         Int_motion_sensor_sig,                                 &Alarm_system_::trigger_red_alarm    >,
    a_irow<Evaluating_risk,         Res_motion_sensor_sig,                                 &Alarm_system_::trigger_red_alarm    >,
    a_row<Evaluating_risk,          Rec_owner_in_ext,         Low_risk,                    &Alarm_system_::counters_update_and_trigger_automatic_cancellation >,        
    //Fake transition to go back to Waiting_for_risk from Evaluating_risk just to test transitions
    _row  <Evaluating_risk,         Reset_risk,               Waiting_for_risk                                                  >
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

#include "Alarm_system_state_machine.cc"

#endif
