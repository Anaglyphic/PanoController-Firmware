/*
 * Pano engine
 *
 * Copyright (C)2016,2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */

#include "pano.h"

//{ FIXME: temporary, make them static in .ino once resolved
#include <BasicStepperDriver.h>
extern BasicStepperDriver horiz_motor;
extern BasicStepperDriver vert_motor;
//}

PanoSetup::PanoSetup(Camera& camera)
:camera(camera)
{
}

void PanoSetup::setFOV(int horiz_angle, int vert_angle){
    if (horiz_angle && vert_angle && horiz_angle <= 360 && vert_angle <= 180){
        horiz_fov = horiz_angle;
        vert_fov = vert_angle;
    }
}

void PanoSetup::setShutter(unsigned speed, unsigned pre_delay, unsigned post_wait, bool long_pulse){
    shutter_delay = speed;
    pre_shutter_delay = pre_delay;
    post_shutter_delay = post_wait;
    shutter_long_pulse = long_pulse;
}

void PanoSetup::setShots(unsigned shots){
    shots_per_position = shots;
}

void PanoSetup::setMode(unsigned mode){

}

unsigned PanoSetup::getHorizShots(void){
    return horiz_count;
}

unsigned PanoSetup::getVertShots(void){
    return vert_count;
}

int PanoSetup::getCurRow(void){
    return position / horiz_count;
}

int PanoSetup::getCurCol(void){
    return position % horiz_count;
}

/*
 * Calculate time left to complete pano.
 */
unsigned Pano::getTimeLeft(void){
    int photos = getHorizShots() * getVertShots() - position + 1;
    int seconds = photos * shots_per_position * (pre_shutter_delay + steady_delay_avg + shots_per_position * (shutter_delay + post_shutter_delay)) / 1000 +
        // time needed to move the platform
        // each photo requires a horizontal move (except last one in each row)
        (photos - photos/horiz_count) * camera.getHorizFOV() * HORIZ_GEAR_RATIO * 60 / horiz_motor.getRPM() / 360 +
        // row-to-row movement
        photos / horiz_count * camera.getVertFOV() * VERT_GEAR_RATIO * 60 / vert_motor.getRPM() / 360 +
        // row return horizontal movement
        photos / horiz_count * horiz_fov * 60 / horiz_motor.getRPM() / 360;
    return seconds;
}

/*
 * Helper to calculate grid fit with overlap
 * @param total_size: entire grid size (1-360 degrees)
 * @param overlap: min required overlap in percent (1-99)
 * @param block_size: ref to initial (max) block size (will be updated)
 * @param count: ref to image count (will be updated)
 */
void PanoSetup::gridFit(int total_size, int overlap, float& block_size, int& count){
    if (block_size <= total_size){
        /*
         * For 360 pano, we need to cover entire circle plus overlap.
         * For smaller panos, we cover the requested size only.
         */
        if (total_size != 360){
            total_size = ceil(total_size - block_size * overlap/100);
        }
        block_size = block_size * (100-overlap) / 100;
        count = ceil(total_size / block_size);
        block_size = float(total_size) / count;
    } else {
        count = 1;
    }
}

/*
 * Calculate shot-to-shot horizontal/vertical head movement,
 * taking overlap into account
 * Must be called every time focal distance or panorama dimensions change.
 */
void PanoSetup::computeGrid(void){
    horiz_move = camera.getHorizFOV();
    gridFit(horiz_fov, MIN_OVERLAP, horiz_move, horiz_count);
    vert_move = camera.getVertFOV();
    gridFit(vert_fov, MIN_OVERLAP, vert_move, vert_count);
}

/*
 * Actual platform driver part
 */
Pano::Pano(MultiDriver& motors, Camera& camera, MPU& mpu)
:PanoSetup(camera),
 motors(motors),
 mpu(mpu)
{
    setFOV(360,180);
}

// begin() follows Arduino convention for setup code
void Pano::begin(void){
    motorsEnable(false);
}

void Pano::start(void){
    computeGrid();
    motorsEnable(true);
    // set start position
    setMotorsHomePosition();
    position = 0;
}

void Pano::shutter(void){
    int start = millis();
    delay(pre_shutter_delay);
    mpu.zeroMotionWait(STEADY_TARGET(camera.getVertFOV(), shutter_delay, CAMERA_RESOLUTION), STEADY_TIMEOUT);
    steady_delay_avg = (steady_delay_avg * position + millis() - start) / (position + 1);
    for (unsigned i=shots_per_position; i; i--){
        camera.shutter(shutter_delay, shutter_long_pulse);
        delay(post_shutter_delay);
    }
}
/*
 * Move to grid position by photo index (0-number of photos)
 */
bool Pano::moveTo(int new_position){
    int new_row = new_position / horiz_count;
    int new_col = new_position % horiz_count;
    return moveTo(new_row, new_col);
}
/*
 * Move to specified grid position
 * @param new_row: requested row position [0 - vert_count)
 * @param new_col: requested col position [0 - horiz_count)
 */
bool Pano::moveTo(int new_row, int new_col){
    int cur_row = getCurRow();
    int cur_col = getCurCol();
    float h_move = 0, v_move = 0;

    if (cur_row >= vert_count ||
        new_row >= vert_count ||
        new_col >= horiz_count ||
        new_col < 0 ||
        new_row < 0){
        // beyond last/first row or column, cannot move there.
        return false;
    }

    if (cur_col != new_col){
        // horizontal adjustment needed
        // figure out shortest path around the circle
        // good idea if on batteries, bad idea when power cable in use
        h_move = (new_col-cur_col) * horiz_move;
        if (abs(h_move) > 180){
            if (h_move < 0){
                h_move = 360 + h_move;
            } else {
                h_move = h_move - 360;
            }
        }
    }
    if (cur_row != new_row){
        // vertical adjustment needed
        v_move = -(new_row-cur_row)*vert_move;
    }

    moveMotors(h_move, v_move);
    position = new_row * horiz_count + new_col;
    return true;
}
bool Pano::next(void){
    return moveTo(position+1);
}
bool Pano::prev(void){
    return (position > 0) ? moveTo(position-1) : false;
}
void Pano::end(void){
    // move to home position
    moveTo(0, 0);
}
/*
 * Execute a full pano run.
 */
void Pano::run(void){
    start();
    do {
        shutter();
    } while(next());
    end();
}

/*
 * Remember current position as "home"
 * (start tracking platform movement to be able to return to it)
 */
void Pano::setMotorsHomePosition(void){
    horiz_home_offset = 0;
    vert_home_offset = 0;
}

/*
 * Move head requested number of degrees
 */
void Pano::moveMotors(float h, float v){
    motors.rotate(h * HORIZ_GEAR_RATIO, v * VERT_GEAR_RATIO);
    horiz_home_offset += h;
    vert_home_offset += v;
}

/*
 * Prepare moving motors requested number of degrees
 */

/*
 * Run async operations if needed
 */
unsigned long Pano::pollEvent(void){
    static unsigned long next_event_time = micros();
    return next_event_time;
}

/*
 * Move head back to home position
 */
void Pano::moveMotorsHome(void){
    moveMotors(-horiz_home_offset, -vert_home_offset);
}

void Pano::motorsEnable(bool on){
    (on) ? motors.enable() : motors.disable();
    delay(1);
}
