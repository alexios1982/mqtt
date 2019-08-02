#ifndef _NOTIFICATION_LOGIC_CONTROLLER_
#define _NOTIFICATION_LOGIC_CONTROLLER_

#include "Area_protection.hh"
#include "Synchronized_queue.hh"
#include "Publisher.hh"
#include <memory> //for std::unique_ptr
#include <map>
#include "Alarm_system_state_machine.hh"
#include <functional>
#include "All_states.hh"

//#include "Sensor.hh"

 
class Notification_logic_controller : public Alarm_system{
  enum class Sensor_type{
    DOOR,
    MOTION,
    WINDOW
  };
  enum File_type{
    JPEG,
    MP4
  };
  enum class Ai_result{
    NO_DETECTION,
    UNKNOWN,
    OWNER,
    MONITORED  
  };
  // struct Contact_sensor_state{
  //   bool is_a_duplicate;
  //   bool actual_contact;
  // };
  // struct Motion_sensor_state{
  //   bool is_a_duplicate;
  //   bool actual_occupancy;
  // };
  typedef std::function<void()> Proc_events_ptr;
  typedef std::pair<Proc_events_ptr, Proc_events_ptr> Proc_events_ptr_pair;
  typedef std::string Sensor_mini_id;
  typedef std::map<Sensor_mini_id, Proc_events_ptr_pair> Sensor_proc_events_pair_map;
  typedef std::map<Sensor_mini_id, Sensor_type> Sensor_type_map;
  Sensor_type_map _sensor_type_map;
  Sensor_proc_events_pair_map _sensor_proc_events_map;
  Area_protection &_area_protection;
  Synchronized_queue<mqtt::const_message_ptr> &_queue;
  Publisher &_publisher;
  std::map<Sensor_mini_id, std::string> &_sensor_cam;
  std::map<std::string, std::string> &_cam_path;
  std::string _hub_id;
  typedef char Cam_position;
  std::map<Sensor_mini_id, Cam_position> _sensor_position_map;
  typedef std::pair<Ai_result, Cam_position> Ai_result_position_pair;
  typedef std::function<void(std::string)> Proc_events_ptr_2;
  std::map<Ai_result_position_pair, Proc_events_ptr_2> _ai_result_position_proc_events_map;
  std::vector<int> _jpeg_params;
  int _number_of_sent_frames;
  int _number_of_received_responses;
  const int _NUMBER_OF_FRAMES_TO_SEND;
  bool _is_ext_occupied;
  bool _is_int_occupied;
  bool _is_res_occupied;
  
  ///this method is called by consume_message when a message is present in the queue
  ///check the message type and call the appropriate handlers
  void classify_message(const mqtt::const_message_ptr &zigbee_message_ptr);

  // ///the door sensor sent two messages associate to the same event
  // ///this metod is used to recognize this case
  // //bool is_a_door_sensor_notification_duplicate(const mqtt::const_message_ptr &zigbee_message_ptr);
  // Contact_sensor_state check_contact_sensor_state(const mqtt::const_message_ptr &zigbee_message_ptr);
  // Motion_sensor_state check_motion_sensor_state(const mqtt::const_message_ptr &zigbee_message_ptr);

  typedef bool Is_gate_opened;
  Is_gate_opened is_gate_opened(const mqtt::const_message_ptr &zigbee_message_ptr);
  typedef bool Is_room_occupied;
  Is_room_occupied is_room_occupied(const mqtt::const_message_ptr &zigbee_message_ptr);
  
  // ///handler called by classify_message when the message present in the queue is
  // ///associated to a sensor that requests a rich notification
  // mqtt::const_message_ptr prepare_rich_notification(const mqtt::const_message_ptr &zigbee_message_ptr,
  // 						    const std::string &sensor_mini_id);

  mqtt::const_message_ptr prepare_rich_notification(const std::unique_ptr<Dir_handler::Time_path_pair> &to_send_ptr,
						    File_type file_type,
						    const std::string &sensor_mini_id);
  
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
				const int JPEG_QUALITY = 60,
				const int NUMBER_OF_FRAMES_TO_SEND = 3);
  ///this method check the presence of a message in the queue and start
  ///classify_message in a thread to take the required actions
  void consume_message();
  void update_area_protection();

  Ai_result decode_ai_result(int number_of_owners,
			     int number_of_monitored,
			     int number_of_unknown);

  virtual void send_video_chunk(const Ext_door_open_sensor_sig &evt);
  virtual void send_video_chunk(const Int_door_open_sensor_sig &evt);
  virtual void send_video_chunk(const Res_door_open_sensor_sig &evt);  

  virtual void send_classified_notification_av(char alarm_level, const std::string &mmuid);
  virtual void send_classified_notification_as(char alarm_level, const std::string &sensor_mini_id);
  
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


  virtual void ext_presence_flag_update(const Rec_owner_in_ext &evt);
  virtual void ext_presence_flag_update(const Rec_monit_in_ext &evt);
  virtual void ext_presence_flag_update(const Rec_unk_in_ext &evt);
  virtual void int_presence_flag_update(const Rec_owner_in_int &evt);
  virtual void int_presence_flag_update(const Rec_monit_in_int &evt);
  virtual void int_presence_flag_update(const Rec_unk_in_int &evt);
  virtual void res_presence_flag_update(const Rec_owner_in_res &evt);
  virtual void res_presence_flag_update(const Rec_monit_in_res &evt);
  virtual void res_presence_flag_update(const Rec_unk_in_res &evt);

  virtual void ext_presence_flag_reset(const Clear_ext &evt);
  virtual void int_presence_flag_reset(const Clear_int &evt);
  virtual void res_presence_flag_reset(const Clear_res &evt);

  void load_configuration(const std::string &configuration_file);

  template <class Event>
  void process_event_verbose(const Event &evt);
};

#include "Notification_logic_controller.cxx"
#endif
