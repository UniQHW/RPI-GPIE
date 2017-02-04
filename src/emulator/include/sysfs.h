/*
 * Copyright 2017 Patrick Pedersen <ctx.xda@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SYSFS_H_INCLUDED
#define SYSFS_H_INCLUDED

#include "fs.h"
#include "gpio.h"

using namespace std;

/* Retrieve gpio metadata in string */
string get_gpio_pin(gpio *gpiobj);
string get_gpio_sysfs(gpio *gpiobj);
string get_gpio_value(gpio *gpiobj);
string get_gpio_direction(gpio *gpiobj);

/* GPIO file descriptors */
gpio*  check_export();
int8_t check_unexport();
 
/* (Un)Export GPIOs */
bool        check_value(gpio* gpiobj);
direction_t check_direction(gpio* gpiobj);

/* Spawn/Kill GPIO */
gpio* spawn_gpio(uint8_t pin);
bool  kill_gpio(gpio *gpiobj);

#endif // SYSFS_H_INCLUDED
