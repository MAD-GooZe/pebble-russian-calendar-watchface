#include <pebble.h>
#include <pebble_fonts.h>

static Window *window;

static TextLayer *month_year_layer;

#define CALENDAR_WIDTH 7
#define CALENDAR_HEIGHT 5

static TextLayer *weekday_name_layers[CALENDAR_WIDTH];
static TextLayer *date_layers[CALENDAR_HEIGHT][CALENDAR_WIDTH];

static char *date_txt[CALENDAR_HEIGHT][CALENDAR_WIDTH];
#define DATE_LENGTH 3

static const char* weekday_names[] = {
    "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс"
};
static const char* month_names[] = {
    "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"
};

static char* caption;
#define CAPTION_LENGTH 100

static bool inverted;

static void invert_colors() {
    inverted = !inverted;
}

static int get_days_in_month (int month, int year) {
    month += 1; // month should be 1..12
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else if (month == 2) { // february
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) // check for leap year
            return 29;
        else
            return 28;
    } else {
        return 31;
    }
}


static void setup_layout(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    const int cell_width = bounds.size.w / CALENDAR_WIDTH;
    const int cell_height = 20;
    const int left_offset = (bounds.size.w - cell_width * CALENDAR_WIDTH) / 2;
    int top_offset = 3;


    // draw month and year text layer
    month_year_layer = text_layer_create(GRect(0, top_offset, bounds.size.w, 26));
    text_layer_set_font(month_year_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(month_year_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(month_year_layer));

    caption = calloc(CAPTION_LENGTH, sizeof(char));

    top_offset += 30;

    // draw week days
    for (int i = 0; i < CALENDAR_WIDTH; i++) {
        weekday_name_layers[i] = text_layer_create(GRect(left_offset + i * cell_width, top_offset, cell_width, cell_height));
        text_layer_set_text(weekday_name_layers[i], weekday_names[i]);
        text_layer_set_text_alignment(weekday_name_layers[i], GTextAlignmentCenter);
        layer_add_child(window_layer, text_layer_get_layer(weekday_name_layers[i]));
        text_layer_set_text_color(weekday_name_layers[i], GColorWhite);
        text_layer_set_background_color(weekday_name_layers[i], GColorBlack);
    }

    top_offset += cell_height + 1;


    // draw calendar dates
    for (int i = 0; i < CALENDAR_HEIGHT; i++) {
        for (int j = 0; j < CALENDAR_WIDTH; j++) {

            date_txt[i][j] = calloc(DATE_LENGTH, sizeof(char));

            date_layers[i][j] = text_layer_create(GRect(left_offset + j * cell_width, top_offset, cell_width, cell_height));
            text_layer_set_text_alignment(date_layers[i][j], GTextAlignmentCenter);
            layer_add_child(window_layer, text_layer_get_layer(date_layers[i][j]));

            // regular font for working days
            if (j != 5 && j != 6)
                text_layer_set_font(date_layers[i][j], fonts_get_system_font(FONT_KEY_GOTHIC_14));

        }
        top_offset += cell_height;
    }
}

static void update_calendar(struct tm *tick_time, TimeUnits units_changed) {
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    int date = current_time->tm_mday;
    int month = current_time->tm_mon;
    int year = current_time->tm_year + 1900;
    int weekday = current_time->tm_wday - 1;
    if (weekday == -1)
        weekday = 6;

    time_t first = now - (60 * 60 * 24) * (current_time->tm_mday - 1);
    struct tm *first_day = localtime(&first);

    int first_weekday = first_day->tm_wday - 1;
    if (first_weekday == -1)
        first_weekday = 6;

    snprintf(caption , CAPTION_LENGTH, "%s %d", month_names[month], year);
    text_layer_set_text(month_year_layer, caption);


    int date_to_draw = 1;
    int days_in_month = get_days_in_month(current_time->tm_mon, current_time->tm_year);

    for (int i = 0; i < CALENDAR_HEIGHT; i++) {
        for (int j = 0; j < CALENDAR_WIDTH; j++) {

            if ((first_weekday <= i * CALENDAR_WIDTH + j) && (date_to_draw <= days_in_month)) {
                snprintf(date_txt[i][j], DATE_LENGTH, "%d", date_to_draw);
                text_layer_set_text(date_layers[i][j], date_txt[i][j]);

                // mark today
                if (date_to_draw == date){
                    text_layer_set_text_color(date_layers[i][j], inverted ? GColorBlack : GColorWhite);
                    text_layer_set_background_color(date_layers[i][j], inverted ? GColorWhite: GColorBlack);
                } else {
                    text_layer_set_text_color(date_layers[i][j], inverted ? GColorWhite: GColorBlack);
                    text_layer_set_background_color(date_layers[i][j], inverted ? GColorBlack : GColorWhite);
                }

                date_to_draw++;
            } else {
                date_txt[i][j][0] = 0; // delete text if there is any
            }
        }
    }
}

static void window_load(Window *window) {

    setup_layout(window);
    update_calendar(0, DAY_UNIT);

}

static void window_unload(Window *window) {
    free(caption);
    for (int i = 0; i < CALENDAR_HEIGHT; i++)
        for (int j = 0; j < CALENDAR_WIDTH; j++) {
            free(date_txt[i][j]);
            text_layer_destroy(date_layers[i][j]);
        }
    for (int j = 0; j < CALENDAR_WIDTH; j++)
        text_layer_destroy(weekday_name_layers[j]);
    text_layer_destroy(month_year_layer);
}

static void init(void) {
    window = window_create();

    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_load,
            .unload = window_unload,
    });

    tick_timer_service_subscribe(DAY_UNIT, update_calendar);

    const bool animated = true;
    window_stack_push(window, animated);
}

static void deinit(void) {
    window_destroy(window);
}

int main(void) {

    invert_colors();

    init();

    app_event_loop();
    deinit();

    return 0;
}
