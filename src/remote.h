/*
 * IR Remote interface
 *
 * Copyright (C)2016 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#ifndef REMOTE_H_
#define REMOTE_H_

#include "hid.h"

#if !defined(ARDUINO_SAMD_ZERO)

#include <Arduino.h>
#include <IRremote.h>

class Remote : public HID {
private:
    int pin;
    unsigned last_event;
    unsigned next_repeat_time;
    IRrecv* remote;
    decode_results remote_buffer;
public:
    Remote(int pin);
    unsigned read(void) override;
    int clear(int timeout);
};

#else /* defined(ARDUINO_SAMD_ZERO) */
#warning "IR remote not supported, see https://github.com/z3t0/Arduino-IRremote/issues/274"

class Remote : public HID {
public:
    Remote(int pin){ };
    unsigned read(void) override { return EVENT_NONE; };
    int clear(int timeout) override { return timeout; };
};

#endif /* defined(ARDUINO_SAMD_ZERO) */
#endif /* REMOTE_H_ */