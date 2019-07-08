#include<gst/gst.h>
#include <boost/core/ignore_unused.hpp>

// gpointer is a void
static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data);

//Structure to contain all our information, so we can pass it to callbacks 
typedef struct _CustomData {
  GstElement *pipeline;
  GstElement *kvssink;
  GMainLoop *loop;
} CustomData;

int main(int argc, char **argv){
  boost::ignore_unused(argc);
  boost::ignore_unused(argv);

  // Initialize GStreamer
  gst_init (&argc, &argv);

  CustomData data;  
  GError *parse_error = NULL;
  
  // GstElement *pipeline =
  //   gst_parse_launch ("rtspsrc location=rtsp://192.168.4.12:8554/test latency=0 ! "
  //   			"rtph264depay ! h264parse ! decodebin ! videoconvert ! queue ! x264enc ! rtph264pay !"
  //   			//"rtph264depay ! h264parse ! rtph264pay ! "
  //   "application/x-rtp,media=video,encoding-name=H264,payload=96 ! webrtcbin bundle-policy=max-bundle name=sendrecv" STUN_SERVER,
  //   &error);
    
  data.pipeline = gst_parse_launch("rtspsrc location=rtsp://192.168.4.12:8554/test short-header=TRUE ! rtph264depay ! video/x-h264 ! h264parse ! kvssink stream-name=cam02 access-key=AKIATAX2NJNLYECM2OPQ secret-key=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 aws-region=us-east-1 name=kvssink",
				   &parse_error);

  if(parse_error) {
    g_printerr ("Failed to parse the pipeline: %s\n", parse_error->message);
    g_error_free(parse_error);
    if (data.pipeline)
      g_clear_object (&data.pipeline);
    if (data.kvssink)
      data.kvssink = NULL;

    return 4;
  }
  
  data.kvssink = gst_bin_get_by_name (GST_BIN (data.pipeline), "kvssink");
  g_assert_nonnull(data.kvssink);

  GstStateChangeReturn ret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr ("Unable to set the pipeline to the playing state.\n");
    gst_object_unref (data.pipeline);
    return 3;
  }

  GstBus *bus = gst_element_get_bus (data.pipeline);
  //let's create a GMainLoop structure:
  //NULL as first parameter means that the default context will be used
  data.loop = g_main_loop_new (NULL, FALSE);
  guint bus_watch_id = gst_bus_add_watch (bus, bus_call, data.pipeline);
  g_print ("Running...\n");
  //the main thread stops it till g_main_loop_quit() is called
  //when this event occurs the g_main_loop_run returns
  g_main_loop_run (data.loop);

  gst_object_unref (bus);
  gst_element_set_state (data.pipeline, GST_STATE_NULL);
  gst_object_unref (data.pipeline);
  g_source_remove (bus_watch_id);

  return 0;
}

gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data){
  //GMainLoop *loop = (GMainLoop *)data;
  boost::ignore_unused(bus);
  CustomData *CustomData_ptr = (CustomData *)data;
  GMainLoop *loop = CustomData_ptr->loop;
  //g_print( "got message %s %s \n", GST_MESSAGE_TYPE_NAME(msg), gst_structure_get_name ( gst_message_get_structure(msg) ) );

  switch (GST_MESSAGE_TYPE (msg)) {

  case GST_MESSAGE_EOS:
    g_print ("[multifiles_saving::bus_call]. End of stream\n");
    g_main_loop_quit (loop);
    break;

  case GST_MESSAGE_ERROR: {
    gchar  *debug;
    GError *error;

    gst_message_parse_error (msg, &error, &debug);
    g_free (debug);
    
    g_printerr ("[multifiles_saving::bus_call]. Error: %s\n", error->message);
    g_error_free (error);
    g_main_loop_quit (loop);
    break;
  }
  case GST_MESSAGE_ELEMENT: {
    //g_print("Received a gst_message_element\n");
    break;
  }
  default:
    break;
  }
  
  return true;
}
