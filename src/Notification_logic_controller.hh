#ifndef _NOTIFICATION_LOGIC_CONTROLLER_
#define _NOTIFICATION_LOGIC_CONTROLLER_

#include "Area_protection.hh"
#include "Synchronized_queue.hh"
#include "Publisher.hh"
#include <memory> //for std::unique_ptr
#include <map>
#include "Alarm_system_state_machine.hh"
#include <functional>
//#include "Sensor.hh"

class Notification_logic_controller : public Alarm_system{
  enum File_type{
    JPEG,
    MP4
  };
  struct Contact_sensor_state{
    bool is_a_duplicate;
    bool actual_contact;
  };
  typedef std::function<void()> Proc_events_ptr;
  typedef std::map<std::string, Proc_events_ptr> Sensor_proc_events_map;
  Sensor_proc_events_map _sensor_proc_events_map;
  Area_protection &_area_protection;
  Synchronized_queue<mqtt::const_message_ptr> &_queue;
  Publisher &_publisher;
  std::map<std::string, std::string> &_sensor_cam;
  std::map<std::string, std::string> &_cam_path;
  std::vector<int> _jpeg_params;

  ///this method is called by consume_message when a message is present in the queue
  ///check the message type and call the appropriate handlers
  void classify_message(const mqtt::const_message_ptr &zigbee_message_ptr);
  ///the door sensor sent two messages associate to the same event
  ///this metod is used to recognize this case
  //bool is_a_door_sensor_notification_duplicate(const mqtt::const_message_ptr &zigbee_message_ptr);
  Contact_sensor_state check_contact_sensor_state(const mqtt::const_message_ptr &zigbee_message_ptr);
  
  // ///handler called by classify_message when the message present in the queue is
  // ///associated to a sensor that requests a rich notification
  // mqtt::const_message_ptr prepare_rich_notification(const mqtt::const_message_ptr &zigbee_message_ptr,
  // 						    const std::string &sensor_mini_id);

  mqtt::const_message_ptr prepare_rich_notification(const std::unique_ptr<Dir_handler::Time_path_pair> &to_send_ptr,
						    File_type file_type);
  
  ///handler called by classify_message when the message present in the queue is
  ///associated to a sensor that requests a classified notification  
  mqtt::const_message_ptr prepare_classified_notification(const mqtt::const_message_ptr &zigbee_message_ptr);
  ///publish the notification
  void send_notification(const mqtt::const_message_ptr &message_ptr);
  ///handler called by classify_message when the message present in the queue is
  ///received by the ai server 
  void analyze_ai_response(const mqtt::const_message_ptr &message_ptr);
  std::unique_ptr<Dir_handler::Time_path_pair>
  select_video_chunk(const std::string &sensor_mini_id, int which);
  void send_rich_notifications(const std::string &sensor_mini_id,
			       int which,
			       int how_many_later,
			       File_type file_type=JPEG);
  void send_rich_notification(const std::string &sensor_mini_id,
			      int which,
			      File_type file_type);
public:
  Notification_logic_controller(Area_protection &area_protection,
				Synchronized_queue<mqtt::const_message_ptr> &queue,
				Publisher &publisher,
				std::map<std::string, std::string> &sensor_cam,
				std::map<std::string, std::string> &cam_path,
				//with input bitrate=2000000 and resolution=1920x1080
				//JPEG_QUALITY = 60 gives decode message of about 112KB
				const int JPEG_QUALITY = 60);
  ///this method check the presence of a message in the queue and start
  ///classify_message in a thread to take the required actions
  void consume_message();
  void update_area_protection();


  virtual void send_video_chunk(const Ext_door_open_sensor_sig &evt);
  virtual void send_video_chunk(const Int_door_open_sensor_sig &evt);
  virtual void send_video_chunk(const Res_door_open_sensor_sig &evt);  

  void load_configuration(const std::string &configuration_file);
};
#endif
