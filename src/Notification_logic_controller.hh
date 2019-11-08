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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

 
class Notification_logic_controller : public Alarm_system{
  enum class Sensor_type{
    DOOR,
    MOTION,
    WINDOW,
    CAM
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

  typedef std::function<void()> Proc_events_ptr;
  typedef std::pair<Proc_events_ptr, Proc_events_ptr> Proc_events_ptr_pair;
  typedef std::string Sensor_mini_id;
  typedef char Position;  
  struct Sensor_infos{
    Sensor_type _sensor_type;
    Position _position;
    Proc_events_ptr_pair _proc_events_ptr_pair;
  };
  typedef std::map<Sensor_mini_id, Sensor_infos> Sensor_infos_map;
  Sensor_infos_map _sensor_infos_map;
  Sensor_infos_map _sensor_infos_map_2;
  Area_protection &_area_protection;
  Synchronized_queue<mqtt::const_message_ptr> &_queue;
  Publisher &_publisher;
  typedef std::string Cam_directory;
  std::map<Sensor_mini_id, Cam_directory> _sensor_cam_path;
  std::string _hub_id;
  typedef std::pair<Ai_result, Position> Ai_result_position_pair;
  typedef std::function<void(std::string)> Proc_events_ptr_2;
  std::map<Ai_result_position_pair, Proc_events_ptr_2> _ai_result_position_proc_events_map;
  int _ai_response_counter;
  const int _WHICH_FRAME;
  const int _JPEG_QUALITY;
  const int _NUMBER_OF_FRAMES_TO_SEND;
  const int _NUMBER_OF_AI_RESPONSES;
  const int _SIGNIFICANT_TOPIC_CHARS;
  const int _MAX_PAYLOAD_SIZE;
  bool _is_ext_occupied;
  bool _is_int_occupied;
  bool _is_res_occupied;
  typedef bool Motion_sensor_state;
  typedef std::map<Sensor_mini_id, Motion_sensor_state> Motion_sensors_state_map;
  Motion_sensors_state_map _extern_motion_sensors_state_map;
  Motion_sensors_state_map _intern_motion_sensors_state_map;
  Motion_sensors_state_map _reserved_motion_sensors_state_map;

  typedef std::map<Sensor_mini_id, Motion_sensors_state_map*> Motion_sensors_maps;
  Motion_sensors_maps _motion_sensors_maps;

  ///////////////////////////////////////////////////////////////////////////////////////////
  ///this method is called by consume_message method when a message is present in the queue
  ///check the message type and call the appropriate handlers
  ///@param[in] zigbee_message_ptr: zigbee message received fro zigbee2mqtt to be parsed
  ////////////////////////////////////////////////////////////////////////////////////////
  void classify_message(const mqtt::const_message_ptr &zigbee_message_ptr);

  // ///the door sensor sent two messages associate to the same event
  // ///this metod is used to recognize this case
  // ///this methods are no longer necessary because we can set "debounce" property
  // ///in /opt/zigbee2mqtt/data/configuration.yaml
  // //bool is_a_door_sensor_notification_duplicate(const mqtt::const_message_ptr &zigbee_message_ptr);
  // Contact_sensor_state check_contact_sensor_state(const mqtt::const_message_ptr &zigbee_message_ptr);
  // Motion_sensor_state check_motion_sensor_state(const mqtt::const_message_ptr &zigbee_message_ptr);

  ///////////////////////////////////////////////////////////////////////////////////////////
  ///this method is called by classify_message when the message came from a contact sensor
  ///to check if the associated gate (DOOR OR WINDOW) is closed or opened
  ///@param[in] zigbee_message_ptr: zigbee message received from zigbee2mqtt to be parsed
  ///@return Is_gate_opened: boolean value that is true if gate is opened and false otherwise
  ////////////////////////////////////////////////////////////////////////////////////////
  typedef bool Is_gate_opened;
  Is_gate_opened is_gate_opened(const mqtt::const_message_ptr &zigbee_message_ptr);
  ///////////////////////////////////////////////////////////////////////////////////////////
  ///this method is called by classify_message when the message came from a motion sensor
  ///to check if the area covered by the sensor is occupied (true) or not (false)
  ///@param[in] zigbee_message_ptr: zigbee message received from zigbee2mqtt to be parsed
  ///@return Motion_sensor_state: boolean value that is true covered by the sensor is occupied (true) or not (false)
  ////////////////////////////////////////////////////////////////////////////////////////  
  Motion_sensor_state check_motion_sensor_state(const mqtt::const_message_ptr &zigbee_message_ptr);
  
  // ///handler called by classify_message when the message present in the queue is
  // ///associated to a sensor that requests a rich notification
  // mqtt::const_message_ptr prepare_rich_notification(const mqtt::const_message_ptr &zigbee_message_ptr,
  // 						    const std::string &sensor_mini_id);
  
  ///handler called by classify_message when the message present in the queue is
  ///associated to a sensor that requests a rich notification
  mqtt::const_message_ptr prepare_rich_notification(const std::unique_ptr<Dir_handler::Time_path_pair> &to_send_ptr,
						    File_type file_type,
						    const std::string &sensor_mini_id,
						    int jpeg_quality,
						    float tot_frames_percentage);
  
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
			      int start_jpeg_quality,
			      float tot_frames_percentage,
			      File_type file_type);
  void decrease_ai_response_counter();
  void increase_ai_response_counter();
  void init_sensor_cam_path_and_save_urls(const boost::property_tree::ptree &pt);
  typedef std::vector<Sensor_mini_id> Sensor_mini_ids;  
  void associate_sensor_to_events(const boost::property_tree::ptree &pt,
				  const std::string &ring_type,
				  Sensor_type sensor_type,
				  Sensor_mini_ids &sensor_mini_ids);
  int parse_number_of_levels(const boost::property_tree::ptree &pt) const;
  void update_motion_sensor_state(const Sensor_mini_id &topic_info,
				  Motion_sensor_state motion_sensor_state);
  typedef bool Is_ring_occupied;
  Is_ring_occupied is_ring_occupied(const Sensor_mini_id &topic_info);
public:
  Notification_logic_controller(Area_protection &area_protection,
				Synchronized_queue<mqtt::const_message_ptr> &queue,
				Publisher &publisher,
				//std::map<std::string, std::string> &sensor_cam,
				//std::map<std::string, std::string> &cam_path,
				//with input bitrate=2000000 and resolution=1920x1080
				//JPEG_QUALITY = 60 gives decode message of about 112KB
				const int JPEG_QUALITY = 60,
				const int WHICH_FRAME = 1,
				const int NUMBER_OF_FRAMES_TO_SEND = 3,
				const int NUMBER_OF_AI_RESPONSES = 1);
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

  void load_configuration(const std::string &);

  template <class Event>
  void process_event_verbose(const Event &evt);
};

#include "Notification_logic_controller.cxx"
#endif
