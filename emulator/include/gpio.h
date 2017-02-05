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

#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

#include <string>
#include <fstream>

#define GPIO_PIN_COUNT		0x1A

#define HIGH			0x1
#define LOW			0x0

typedef bool direction_t;

/* Directions */
enum {
	in  = false,
	out = true,
};

/* Primitive GPIO structure */
struct gpio {
public:
	/* Init */
	gpio(int p, string dp, string vp) : pin(p), direction_path(dp), value_path(vp) {};

	/* Get pin */
	int get_pin() { return pin; }

	/* Read direction file */
	direction_t read_direction() { 
		string ret;
		ifstream(direction_path) >> ret;
		return (ret == "out" ? out : in);
	}

	/* Read value file */
	uint8_t read_value() {
		bool ret;
		ifstream(value_path, ios::binary) >> ret;
		return ret;
	}

	/* Check for failure */
	bool check() {
		return (ifstream(direction_path).is_open(), ifstream(value_path).is_open());
	}

private:
	int pin;
	string direction_path;
	string value_path;

};

#endif // GPIO_H_INCLUDED
