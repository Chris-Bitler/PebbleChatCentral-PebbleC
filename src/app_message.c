#include <pebble.h>

Window *window;	
static TextLayer *title_text_layer;
static char final[300];
// Key values for AppMessage Dictionary
enum {
	CHANNEL = 0,	
	SENDER = 1,
  MESSAGE = 2
};

// Write message to buffer & send
void send_message(void){
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *c_tuple,*s_tuple,*m_tuple; 
	c_tuple = dict_find(received, CHANNEL);
  char channel[32];
  char sender[32];
  char message[200];
	if(c_tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Channel: %s", c_tuple->value->cstring); 
    memcpy(channel, c_tuple->value->cstring, c_tuple->length); 
	}
	
	s_tuple = dict_find(received, SENDER);
	if(s_tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Sender: %s", s_tuple->value->cstring);
    memcpy(sender, s_tuple->value->cstring, s_tuple->length); 
	}
  
  m_tuple = dict_find(received, MESSAGE);
	if(m_tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", m_tuple->value->cstring); 
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstri
    memcpy(message, m_tuple->value->cstring, m_tuple->length); 
    //strncpy(message,tuple->value->cstring, sizeof(tuple->value->cstring));
	}
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Debug: %s",channel);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Debug: %s",sender);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Debug: %s",message);
  final[0] = '\0';
  strcpy(final, channel);
  strcat(final, ": ");
  strcat(final, sender);
  strcat(final,": ");
  strcat(final,message);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Debug: %s",final);
  text_layer_set_text(title_text_layer, final);
  layer_mark_dirty(window_get_root_layer(window));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", text_layer_get_text(title_text_layer));
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}



static void main_window_load(Window *window) {
   Layer *window_layer = window_get_root_layer(window);

  // Get the bounds of the window for sizing the text layer
  GRect bounds = layer_get_bounds(window_layer);

  // Create and Add to layer hierarchy:
  title_text_layer = text_layer_create(GRect(5, 5, bounds.size.w, bounds.size.h));
  text_layer_set_overflow_mode(title_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_text(title_text_layer, "This is TextLayer");
  layer_add_child(window_layer, text_layer_get_layer(title_text_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(title_text_layer);

  // Destroy Window
  window_destroy(window);
}

void init(void) {
	window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
	window_stack_push(window, true);
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	send_message();
}

void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}