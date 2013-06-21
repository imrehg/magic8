#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0x46, 0xCB, 0x20, 0xBD, 0x22, 0xD4, 0x47, 0xA6, 0xA6, 0x7E, 0x9B, 0x31, 0x34, 0x4E, 0xAA, 0x74 }
PBL_APP_INFO(MY_UUID,
             "Magic Eightball", "Gergely Imreh",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;

Layer layer;

/* Possible answers */
char *answers[20] = {"It is certain",
		     "It is decidedly so",
		     "Without a doubt",
		     "Yes definitely",
		     "You may rely on it",
		     "As I see it yes",
		     "Most likely",
		     "Outlook good",
		     "Yes",
		     "Signs point to yes",
		     "Reply hazy try again",
		     "Ask again later",
		     "Better not tell you now",
		     "Cannot predict now",
		     "Concentrate and ask again",
		     "Don't count on it",
		     "My reply is no",
		     "My sources say no",
		     "Outlook not so good",
		     "Very doubtful"};

/* random() related parts are from https://github.com/ad1217/pebbloids/blob/master/src/Pebbloids.c */
long seed;
int random(int max){
    seed=(((seed*214013L+2531011L) >> 16) & 32767);
    return ((seed%max));
}

void update_layer_callback(Layer *me, GContext* ctx) {
  (void)me;
  int reading = random(20);

  graphics_context_set_text_color(ctx, GColorBlack);

  graphics_text_draw(ctx,
		     answers[reading],
		     /* fonts_get_system_font(FONT_KEY_FONT_FALLBACK), */
		     fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD),
		     GRect(10, 10, 144-15, 100),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentLeft,
		     NULL);

}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Magic 8-ball");
  window_stack_push(&window, true);


  layer_init(&layer, window.layer.frame);
  layer.update_proc = update_layer_callback;
  layer_add_child(&window.layer, &layer);

  layer_mark_dirty(&layer);

  /* Random seed */
  PblTm time;
  get_time(&time);
  seed=time.tm_hour*3600+time.tm_min*60+time.tm_sec;
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
