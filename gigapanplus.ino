/*
 * GigapanPlus for Arduino project
 *
 * Copyright (C)2015 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DRV8834.h>
#include "pano.h"
#include "camera.h"
#include "joystick.h"
#include "menu.h"

// Address of I2C OLED display. If screen looks scaled edit Adafruit_SSD1306.h
// and pick SSD1306_128_64 or SSD1306_128_32 that matches display type.
#define DISPLAY_I2C_ADDRESS 0x3C
#define OLED_RESET 4

#define JOYSTICK_X A2
#define JOYSTICK_Y A3
#define JOYSTICK_SW 2

#define MOTOR_STEPS 200
#define VERT_EN 4
#define VERT_DIR 5
#define VERT_STEP 6
#define HORIZ_EN 7
#define HORIZ_DIR 8
#define HORIZ_STEP 9

#define DRV_M0 10
#define DRV_M1 11
#define CAMERA_FOCUS 12
#define CAMERA_SHUTTER 13

static DRV8834 horiz_motor(MOTOR_STEPS, HORIZ_DIR, HORIZ_STEP, DRV_M0, DRV_M1);
static DRV8834 vert_motor(MOTOR_STEPS, VERT_DIR, VERT_STEP, DRV_M0, DRV_M1);
static Adafruit_SSD1306 display(OLED_RESET);
static Camera camera(CAMERA_FOCUS, CAMERA_SHUTTER);
static Joystick joystick(JOYSTICK_SW, JOYSTICK_X, JOYSTICK_Y);
static Pano pano(horiz_motor, vert_motor, camera, HORIZ_EN, VERT_EN);

const int rows = 6;

void setup() {
    Serial.begin(38400);
    delay(1000); // wait for serial
    delay(100);  // give time for display to init; if display blank increase delay
    display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS);
    display.setRotation(0);
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    Serial.println(F("Ready\n"));
    menu.open();
    menu.render(&display, rows);
    display.display();
}

void handleEvent(int event) {

    if (isEventLeft(event)) menu.cancel();
    else if (isEventRight(event) || isEventClick(event)) menu.select();
    else if (isEventDown(event)) menu.next();
    else if (isEventUp(event)) menu.prev();

    Serial.println();
    display.clearDisplay();
    display.setCursor(0,0);

    menu.render(&display, rows);

    display.display();
}

void loop() {
    int event = joystick.read();
    if (event && !running){
        handleEvent(event);
        Serial.println(focal);
        if (running){   // pano was just started via Menu
            display.clearDisplay();
            display.setCursor(0,0);
            display.print(F("Start "));
            display.display();
            pano.setFocalLength(focal);
            pano.setFOV(horiz, vert);
            pano.setShutter(shutter, pre_shutter);
            pano.start();
        }
    }
    if (running){ // pano is in process
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(F("running\n"));
        display.display();
        display.print(F("pos = "));
        display.println(pano.position);
        display.print(F("horiz = "));
        display.println(pano.horiz_position);
        display.print(F("vert = "));
        display.println(pano.vert_position);
        display.display();
        running = pano.next();
        if (!running){
            pano.end();
            display.println(F("end"));
            display.display();
            delay(5000);
            menu.open();
            handleEvent(EVENT_NONE);
        }
    }
}
