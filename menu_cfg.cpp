#include "menu.h"
// Start
volatile int running;
static const char menu_start_desc[] PROGMEM = "Start";
static const char menu_start_name_0[] PROGMEM = "On";
static const char menu_start_name_1[] PROGMEM = "Off";

static const char *menu_start_names[2] = {menu_start_name_0, menu_start_name_1};
static const int menu_start_values[2] = {1, 0};
static NamedListSelector menu_start(menu_start_desc, &running, 0, 2, menu_start_names, menu_start_values);

// Camera Orientation
volatile int orientation;
static const char menu_camera_orientation_desc[] PROGMEM = "Camera Orientation";
static const char menu_camera_orientation_name_0[] PROGMEM = "portrait";
static const char menu_camera_orientation_name_1[] PROGMEM = "landscape";
static const char menu_camera_orientation_name_2[] PROGMEM = "square";

static const char *menu_camera_orientation_names[3] = {menu_camera_orientation_name_0, menu_camera_orientation_name_1, menu_camera_orientation_name_2};
static const int menu_camera_orientation_values[3] = {0, 1, 2};
static NamedListSelector menu_camera_orientation(menu_camera_orientation_desc, &orientation, 0, 3, menu_camera_orientation_names, menu_camera_orientation_values);

// Aspect Ratio
volatile int aspect;
static const char menu_aspect_ratio_desc[] PROGMEM = "Aspect Ratio";
static const char menu_aspect_ratio_name_0[] PROGMEM = "3:2";
static const char menu_aspect_ratio_name_1[] PROGMEM = "16:9";
static const char menu_aspect_ratio_name_2[] PROGMEM = "4:3";
static const char menu_aspect_ratio_name_3[] PROGMEM = "1:1";

static const char *menu_aspect_ratio_names[4] = {menu_aspect_ratio_name_0, menu_aspect_ratio_name_1, menu_aspect_ratio_name_2, menu_aspect_ratio_name_3};
static const int menu_aspect_ratio_values[4] = {0, 1, 2, 3};
static NamedListSelector menu_aspect_ratio(menu_aspect_ratio_desc, &aspect, 0, 4, menu_aspect_ratio_names, menu_aspect_ratio_values);

// Focal Length
volatile int focal;
static const char menu_focal_length_desc[] PROGMEM = "Focal Length";

static const int menu_focal_length_values[12] = {12, 14, 16, 20, 24, 35, 50, 70, 105, 200, 300, 400};
static ListSelector menu_focal_length(menu_focal_length_desc, &focal, 35, 12, menu_focal_length_values);

// Shutter Speed
volatile int shutter;
static const char menu_shutter_speed_desc[] PROGMEM = "Shutter Speed";
static const char menu_shutter_speed_name_0[] PROGMEM = "1/100";
static const char menu_shutter_speed_name_1[] PROGMEM = "1/20";
static const char menu_shutter_speed_name_2[] PROGMEM = "1/10";
static const char menu_shutter_speed_name_3[] PROGMEM = "1/4";
static const char menu_shutter_speed_name_4[] PROGMEM = "0.5s";
static const char menu_shutter_speed_name_5[] PROGMEM = "1s";
static const char menu_shutter_speed_name_6[] PROGMEM = "2s";
static const char menu_shutter_speed_name_7[] PROGMEM = "4s";
static const char menu_shutter_speed_name_8[] PROGMEM = "8s";

static const char *menu_shutter_speed_names[9] = {menu_shutter_speed_name_0, menu_shutter_speed_name_1, menu_shutter_speed_name_2, menu_shutter_speed_name_3, menu_shutter_speed_name_4, menu_shutter_speed_name_5, menu_shutter_speed_name_6, menu_shutter_speed_name_7, menu_shutter_speed_name_8};
static const int menu_shutter_speed_values[9] = {10, 50, 100, 250, 500, 1000, 2000, 4000, 8000};
static NamedListSelector menu_shutter_speed(menu_shutter_speed_desc, &shutter, 10, 9, menu_shutter_speed_names, menu_shutter_speed_values);

// Pre-Shutter Delay
volatile int pre_shutter;
static const char menu_pre_shutter_delay_desc[] PROGMEM = "Pre-Shutter Delay";
static const char menu_pre_shutter_delay_name_0[] PROGMEM = "0.1s";
static const char menu_pre_shutter_delay_name_1[] PROGMEM = "0.5s";
static const char menu_pre_shutter_delay_name_2[] PROGMEM = "1s";
static const char menu_pre_shutter_delay_name_3[] PROGMEM = "2s";
static const char menu_pre_shutter_delay_name_4[] PROGMEM = "4s";

static const char *menu_pre_shutter_delay_names[5] = {menu_pre_shutter_delay_name_0, menu_pre_shutter_delay_name_1, menu_pre_shutter_delay_name_2, menu_pre_shutter_delay_name_3, menu_pre_shutter_delay_name_4};
static const int menu_pre_shutter_delay_values[5] = {100, 500, 1000, 2000, 4000};
static NamedListSelector menu_pre_shutter_delay(menu_pre_shutter_delay_desc, &pre_shutter, 100, 5, menu_pre_shutter_delay_names, menu_pre_shutter_delay_values);

// Shots per position
volatile int shots;
static const char menu_shots_per_position_desc[] PROGMEM = "Shots per position";

static RangeSelector menu_shots_per_position(menu_shots_per_position_desc, &shots, 1, 1, 5, 1);

// Horizontal FOV
volatile int horiz;
static const char menu_horizontal_fov_desc[] PROGMEM = "Horizontal FOV";

static RangeSelector menu_horizontal_fov(menu_horizontal_fov_desc, &horiz, 120, 10, 360, 10);

// Vertical FOV
volatile int vert;
static const char menu_vertical_fov_desc[] PROGMEM = "Vertical FOV";

static RangeSelector menu_vertical_fov(menu_vertical_fov_desc, &vert, 90, 10, 180, 10);

// Motor Control
volatile int motors_enable;
static const char menu_motor_control_desc[] PROGMEM = "Motor Control";
static const char menu_motor_control_name_0[] PROGMEM = "On";
static const char menu_motor_control_name_1[] PROGMEM = "Off";

static const char *menu_motor_control_names[2] = {menu_motor_control_name_0, menu_motor_control_name_1};
static const int menu_motor_control_values[2] = {1, 0};
static NamedListSelector menu_motor_control(menu_motor_control_desc, &motors_enable, 1, 2, menu_motor_control_names, menu_motor_control_values);


static union MenuItem menus[10] = {&menu_start, &menu_camera_orientation, &menu_aspect_ratio, &menu_focal_length, &menu_shutter_speed, &menu_pre_shutter_delay, &menu_shots_per_position, &menu_horizontal_fov, &menu_vertical_fov, &menu_motor_control};
static const int menu_types[10] = {NamedListSelector::class_id,NamedListSelector::class_id,NamedListSelector::class_id,ListSelector::class_id,NamedListSelector::class_id,NamedListSelector::class_id,RangeSelector::class_id,RangeSelector::class_id,RangeSelector::class_id,NamedListSelector::class_id};
Menu menu("Main Menu", 10, menus, menu_types);
