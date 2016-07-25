
/*
 * Do not edit this file, it is autogenerated.
 * Edit menu_cfg.json instead, then type "make".
 */
#include "menu.h"

// Pano
// New Pano
int onStart(int);

static OptionSelector menu_new_pano("New Pano", NULL, 0, 0 * sizeof(int), onStart);

// Repeat Last
int onRepeat(int);

static OptionSelector menu_repeat_last("Repeat Last", NULL, 0, 0 * sizeof(int), onRepeat);

// 360 Pano
int on360(int);

static OptionSelector menu_360_pano("360 Pano", NULL, 0, 0 * sizeof(int), on360);

// Last Pano Info
int onPanoInfo(int);

static OptionSelector menu_last_pano_info("Last Pano Info", NULL, 0, 0 * sizeof(int), onPanoInfo);

// Manual Horiz FOV
extern volatile int horiz;

static RangeSelector menu_manual_horiz_fov("Manual Horiz FOV", &horiz, 120, 1 * sizeof(int), NULL, 10, 360, 10);

// Manual Vert FOV
extern volatile int vert;

static RangeSelector menu_manual_vert_fov("Manual Vert FOV", &vert, 90, 2 * sizeof(int), NULL, 10, 180, 10);


static const PROGMEM BaseMenu* const menu_pano_opts[] = {&menu_new_pano, &menu_repeat_last, &menu_360_pano, &menu_last_pano_info, &menu_manual_horiz_fov, &menu_manual_vert_fov};
Menu menu_pano("Pano", 6, menu_pano_opts);

// Camera
// Focal Len
extern volatile int focal;

static ListSelector menu_focal_len("Focal Len", &focal, 35, 3 * sizeof(int), NULL, 16,
    (const PROGMEM int[]){12, 14, 16, 20, 24, 28, 35, 50, 75, 105, 200, 300, 400, 450, 500, 600});

// Shutter
extern volatile int shutter;

static NamedListSelector menu_shutter("Shutter", &shutter, 10, 4 * sizeof(int), NULL, 11,
    (const PROGMEM char * const[]){"1/100", "1/50", "1/25", "1/10", "1/4", "0.5s", "1s", "2s", "4s", "8s", "MANUAL"},
    (const PROGMEM int[]){10, 20, 40, 100, 250, 500, 1000, 2000, 4000, 8000, 0});

// Delay
extern volatile int pre_shutter;

static NamedListSelector menu_delay("Delay", &pre_shutter, 100, 5 * sizeof(int), NULL, 6,
    (const PROGMEM char * const[]){"0.1s", "0.5s", "1s", "2s", "4s", "8s"},
    (const PROGMEM int[]){100, 500, 1000, 2000, 4000, 8000});

// Processing Wait
extern volatile int post_wait;

static NamedListSelector menu_processing_wait("Processing Wait", &post_wait, 100, 6 * sizeof(int), NULL, 7,
    (const PROGMEM char * const[]){"0.1s", "0.25s", "0.5s", "1s", "2s", "4s", "8s"},
    (const PROGMEM int[]){100, 250, 500, 1000, 2000, 4000, 8000});

// Shutter Mode
extern volatile int long_pulse;

static NamedListSelector menu_shutter_mode("Shutter Mode", &long_pulse, 0, 7 * sizeof(int), NULL, 2,
    (const PROGMEM char * const[]){"Normal", "Cont Bkt"},
    (const PROGMEM int[]){0, 1});

// Shots #
extern volatile int shots;

static RangeSelector menu_shots__("Shots #", &shots, 1, 8 * sizeof(int), NULL, 1, 5, 1);

// Aspect
extern volatile int aspect;

static NamedListSelector menu_aspect("Aspect", &aspect, 23, 9 * sizeof(int), NULL, 2,
    (const PROGMEM char * const[]){"P 2:3", "L 3:2"},
    (const PROGMEM int[]){23, 32});

// Test Shutter
int onTestShutter(int);

static OptionSelector menu_test_shutter("Test Shutter", NULL, 0, 0 * sizeof(int), onTestShutter);


static const PROGMEM BaseMenu* const menu_camera_opts[] = {&menu_focal_len, &menu_shutter, &menu_delay, &menu_processing_wait, &menu_shutter_mode, &menu_shots__, &menu_aspect, &menu_test_shutter};
Menu menu_camera("Camera", 8, menu_camera_opts);

// Motors
extern volatile int motors_enable;

static NamedListSelector menu_motors("Motors", &motors_enable, 0, 0 * sizeof(int), NULL, 2,
    (const PROGMEM char * const[]){"On", "Off"},
    (const PROGMEM int[]){1, 0});

// Display
extern volatile int display_invert;

static NamedListSelector menu_display("Display", &display_invert, 0, 0 * sizeof(int), NULL, 2,
    (const PROGMEM char * const[]){"Dark", "Bright"},
    (const PROGMEM int[]){0, 1});


static const PROGMEM BaseMenu* const menu_opts[] = {&menu_pano, &menu_camera, &menu_motors, &menu_display};
Menu menu("Main Menu", 4, menu_opts);

