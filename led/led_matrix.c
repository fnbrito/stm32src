
#include "led_matrix.h"
#include <string.h>
#include <stdio.h>
#include "Seeed_Font.h"

// Low1 High1 Low2 High2 ... Low8 High8
static uint8_t _buffer[16];
static uint8_t _big_buffer[MAX_BIG_BUFFER_SIZE];
static uint16_t _cursor_start, _cursor_end, _cursor_steps;
static uint16_t _ms;
static orientation_type_t _orientation;
static int8_t _offset_x;
static int8_t _offset_y;


void ht16k33_init(uint8_t addr) {
    _addr = addr;
    // turn on oscillator
    writeBytes(_addr, 0x21, 0, (uint8_t*)NULL);
}

void ht16k33_setBrightness(uint8_t brightness) {
    writeBytes(_addr, (0xE0 | brightness), 0, (uint8_t*)NULL);
}


void ht16k33_setBlinkRate(blink_type_t blink_type) {
    writeBytes(_addr, (0x80 | 0x01 | (blink_type << 1)), 0, (uint8_t*)NULL);
}

void matrix_create() {
    _orientation = DISPLAY_ROTATE_0;
    _offset_x = 0;
    _offset_y = 0;
    _cursor_start = 0;
    _cursor_end = 0;
    _cursor_steps = 1;
    _ms = 100;
}

void matrix_setDisplayOffset(int8_t x, int8_t y) {
    if ((x < -8) || (x > 8)) {
        return;
    }
    if ((y < -8) || (y > 8)) {
        return;
    }
    _offset_x = x;
    _offset_y = y;
}

void matrix_setDisplayOrientation(uint8_t orientation) {
    _orientation = (orientation_type_t)orientation;
}

// _cursor_end - _cursor_start >= 1
void matrix_display() {
    // get data from _big_buffer[] and write to HT16K33
    uint8_t line;

    if (_cursor_start > _cursor_end) {
        uint16_t t;
        t = _cursor_start;
        _cursor_start = _cursor_end;
        _cursor_end = t;
    }

    for (int32_t i = _cursor_start; i <= _cursor_end; i = i + _cursor_steps) {
        for (uint8_t j = 0; j < 8; j++) {
            // test
            line = _big_buffer[(MAX_BIG_BUFFER_SIZE - 8 - i) + j];
            for (uint8_t k = 0; k < 8; k++) {
                if (line & 0x01) {
                    // set 1
                    _buffer[2 * j] |= (1 << k);
                } else {
                    // set 0
                    _buffer[2 * j] &= ~(1 << k);
                }
                line = line >> 1;
            }
        }
        // offset
        if (_offset_y > 0) {
            for (uint8_t i = 0; i < 8; i++) {
                _buffer[2 * i] = (_buffer[2 * i] >> _offset_y);
            }
        } else if (_offset_y < 0) {
            for (uint8_t i = 0; i < 8; i++) {
                _buffer[2 * i] = (_buffer[2 * i] << (-_offset_y));
            }
        }
        if (_offset_x > 0) {
            for (int8_t i = (7 - _offset_x); i >= 0; i--) {
                _buffer[2 * (i + _offset_x)] = _buffer[2 * i];
            }
            for (uint8_t i = 0; i < _offset_x; i++) {
                _buffer[2 * i] = 0;
            }
        } else if (_offset_x < 0) {
            for (uint8_t i = (-_offset_x); i < 8; i++) {
                _buffer[2 * (i + _offset_x)] = _buffer[2 * i];
            }

            for (uint8_t i = (8 + _offset_x); i < 8; i++) {
                _buffer[2 * i] = 0;
            }
        }


        writeBytes(_addr, 0x00, 16, _buffer);
/*        printf("_addr: %x Buffer: %x\n", _addr, _buffer[0]);
        printf("_addr: %x Buffer: %x\n", _addr, _buffer[1]);
        printf("_addr: %x Buffer: %x\n", _addr, _buffer[2]);
        printf("_addr: %x Buffer: %x\n", _addr, _buffer[3]);
        printf("_addr: %x Buffer: %x\n", _addr, _buffer[4]);
        printf("_addr: %x Buffer: %x\n", _addr, _buffer[5]);
        printf("_addr: %x Buffer: %x\n", _addr, _buffer[6]);
        */
        HAL_Delay(_ms);
    }
}

void matrix_clear() {
    memset(_big_buffer, 0, MAX_BIG_BUFFER_SIZE);
}

void matrix_writePixel(uint8_t x, uint16_t y, bool set_on) {
    if (x > 7) {
        x = 7;
    }
    if (y > (MAX_BIG_BUFFER_SIZE - 1)) {
        y = MAX_BIG_BUFFER_SIZE - 1;
    }

    uint8_t y_mod8 = y % 8;
    // first do spin
    switch (_orientation) {
        case DISPLAY_ROTATE_0: {
                uint8_t t;
                t = x;
                x = y_mod8;
                y_mod8 = t;
                x = 7 - x;
            }
            break;

        // NOT IMPLEMENTED
        case DISPLAY_ROTATE_90:
            break;

        case DISPLAY_ROTATE_180: {
                uint8_t t;
                t = x;
                x = y_mod8;
                y_mod8 = t;
                y_mod8 = 7 - y_mod8;
            }
            break;

        case DISPLAY_ROTATE_270:
            y_mod8 = 7 - y_mod8;
            x = 7 - x;
            break;
    }

    if (set_on) {
        _big_buffer[((MAX_BIG_BUFFER_SIZE / 8 - 1) - (y / 8)) * 8 + y_mod8] |= (1 << x);
    } else {
        _big_buffer[((MAX_BIG_BUFFER_SIZE / 8 - 1) - (y / 8)) * 8 + y_mod8] &= ~(1 << x);
    }

}

void matrix_writeBar(uint8_t bar) {
    if (bar > 32) {
        bar = 32;
    }
    matrix_writeOnePicture8((uint8_t*)&BARS[bar]);
}
void matrix_writeIcon(uint8_t num) {
    if (num > 28) {
        num = 28;
    }
    matrix_writeOnePicture8((uint8_t*)&ICONS[num]);
}

void matrix_writeNumber(int32_t number, uint16_t ms_per_digit) {
  char str[16];
  sprintf(str, "%ld", number);

    matrix_writeString(str, ms_per_digit, ACTION_SCROLLING);
}
void matrix_writeString(char* s, uint16_t ms_per_letter, action_type_t mode) {
    //uint8_t pic_number = s.length();
    uint8_t pic_number = strlen(s);
    if (pic_number > (MAX_BIG_BUFFER_SIZE / 8)) {
        pic_number = MAX_BIG_BUFFER_SIZE / 8;
    }
    if (pic_number == 0) {
        return;
    }


    const char* p_string = s; //s.c_str();
    _cursor_start = 0;
    if (mode == ACTION_SCROLLING) {
        _cursor_steps = 1;

        if (pic_number == 1) {
            _cursor_end = 0;
            _ms = ms_per_letter;
        } else {
            _cursor_end = pic_number * 8;
            _ms = ms_per_letter / 8;
        }
    } else {
        if (pic_number == 1) {
            _cursor_end = 0;
        } else {
            _cursor_end = pic_number * 8 - 1;
        }
        _cursor_steps = 8;
        _ms = ms_per_letter;
    }

    uint64_t frame;
    uint8_t line;

    memset(_big_buffer, 0, MAX_BIG_BUFFER_SIZE);

    for (uint8_t m = 0; m < pic_number; m++) {
        frame = ASCII[p_string[m] - 32];
        for (uint8_t i = 0; i < 8; i++) {
            line = (uint8_t)(frame & 0xff);
            for (uint8_t j = 7; j != 255; j--) {
                if (line & 0x01) {
                    matrix_writePixel(j, i + m * 8, true);
                }
                line = line >> 1;
            }
            frame = frame >> 8;
        }
    }
}

void matrix_writeOnePicture8(const uint8_t *pic) {
    matrix_writePictures8(pic, 1, 0, ACTION_SHIFT);
}

// uint64_t pic
void matrix_writeOnePicture64(uint64_t pic) {
    matrix_writePictures64((uint64_t*)&pic, 1, 0, ACTION_SHIFT);
}

void matrix_writePictures8(const uint8_t *pic, uint8_t pic_number, uint16_t ms_per_pic, action_type_t mode) {
    if (pic_number > (MAX_BIG_BUFFER_SIZE / 8)) {
        pic_number = MAX_BIG_BUFFER_SIZE / 8;
    }
    if (pic_number == 0) {
        return;
    }
    _cursor_start = 0;
    if (mode == ACTION_SCROLLING) {
        _cursor_steps = 1;
        _ms = ms_per_pic / 8;
        _cursor_end = pic_number * 8;
    } else {
        _cursor_end = pic_number * 8 - 1;
        _cursor_steps = 8;
        _ms = ms_per_pic;
    }

    uint8_t line;

    memset(_big_buffer, 0, MAX_BIG_BUFFER_SIZE);

    for (uint8_t i = 0; i < (8 * pic_number); i++) {
        line = pic[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (line & 0x01) {
                matrix_writePixel(j, i, true);
            }
            line = line >> 1;
        }
    }
}

void matrix_writePictures64(const uint64_t *pic, uint8_t pic_number, uint16_t ms_per_pic, action_type_t mode) {
    if (pic_number > (MAX_BIG_BUFFER_SIZE / 8)) {
        pic_number = MAX_BIG_BUFFER_SIZE / 8;
    }
    if (pic_number == 0) {
        return;
    }
    _cursor_start = 0;
    if (mode == ACTION_SCROLLING) {
        _cursor_steps = 1;
        _ms = ms_per_pic / 8;
        _cursor_end = pic_number * 8;
    } else {
        _cursor_end = pic_number * 8 - 1;
        _cursor_steps = 8;
        _ms = ms_per_pic;
    }

    uint64_t frame;
    uint8_t line;

    memset(_big_buffer, 0, MAX_BIG_BUFFER_SIZE);

    for (uint8_t m = 0; m < pic_number; m++) {
        frame = pic[m];
        for (uint8_t i = 0; i < 8; i++) {
            line = (uint8_t)(frame & 0xff);
            for (uint8_t j = 7; j != 255; j--) {
                if (line & 0x01) {
                    matrix_writePixel(j, i + m * 8, true);
                }
                line = line >> 1;
            }
            frame = frame >> 8;
        }
    }
}
