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

#include "Alarm_system_states.hh"
#include "Alarm_system_events.hh"

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

// front-end: define the FSM structure
struct Alarm_system_ : public msm::front::state_machine_def<Alarm_system_>{

  typedef Init initial_state;
  
  template <class Event, class FSM>
  void on_entry(Event const& ,FSM&);

  struct transition_table : mpl::vector<
    //  Start                       Event                     Target                     Action                      Guard
    // +---------------------------+-------------------------+--------------------------+---------------------------+-----------    
    Row <Init,                      Switch_on,                Waiting_for_configuration, none,                       none >,
    // +---------------------------+-------------------------+--------------------------+---------------------------+-----------
    Row <Waiting_for_configuration, Initialization_completed, Green_alarm,               none,                       none >,
    // +---------------------------+-------------------------+--------------------------+---------------------------+-----------
    Row <Green_alarm,               Orange_alarm,             Orange_alarm_notified,     none,                       none >,
    Row <Green_alarm,               Red_alarm,                Red_alarm_notified,        none,                       none >,
    Row <Green_alarm,               Switch_off,               Exit,                      none,                       none >,
    // +---------------------------+-------------------------+--------------------------+---------------------------+-----------
    Row <Orange_alarm_notified,     User_cancellation,        Green_alarm,               none,                       none >,
    Row <Orange_alarm_notified,     User_confirmation,        Red_alarm_actuation,       none,                       none >,
    Row <Orange_alarm_notified,     Automatic_cancellation,   Green_alarm,               none,                       none >,
    Row <Orange_alarm_notified,     Red_alarm_timeout,        Red_alarm_actuation,       none,                       none >,
    // +---------------------------+-------------------------+--------------------------+---------------------------+-----------
    Row <Red_alarm_notified,        User_cancellation,        Green_alarm,               none,                       none >,
    // +---------------------------+-------------------------+--------------------------+---------------------------+-----------
    Row <Red_alarm_actuation,       Stop_actuation,           Green_alarm,               none,                       none >,
    // +---------------------------+-------------------------+--------------------------+---------------------------+-----------
    Row <Exit,                      Switch_on,                Green_alarm,               none,                       none >    
    > {};
};

// back-end
typedef msm::back::state_machine<Alarm_system_> Alarm_system;

#include "Alarm_system.cc"

#endif
