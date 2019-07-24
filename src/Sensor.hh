#ifndef _SENSOR_
#define _SENSOR_
#include <string>

template <class Event_type>
struct Sensor{
  std::string topic_mini_id;
  Event_type event;
  std::string ring;
  std::string room;
};

#endif
