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

#include <fstream>
#include <string>
#include <stdint.h>

/* Low */
#define off 	false
#define low	false
#define OFF 	false
#define LOW	false

/* High */
#define ON	true
#define HIGH	true
#define on	true
#define high	true

typedef uint8_t pin_t;
typedef bool    bit_t;

/* Direction modes */
enum mode {
	IN	= 0,
	in 	= IN,

	INPUT	= 0,
	input	= IN,

	OUT	= 1,
	out 	= OUT, 

	OUTPUT 	= 1,
	output	= OUT,
};

/* GPIO structure */
struct gpio {
public:
	gpio(pin_t pin, mode io);
       ~gpio();
	mode getMode(); // Returns mode
	void setMode(mode io);
	mode toggleMode();
	bool write(bit_t state);
	bit_t toggleState();
	bit_t read();
	bool is_set();
private:
	std::fstream  	gpio_fd;
	std::ofstream	mode_fd;

	std::string	mode_fd_path;
	std::string 	gpio_fd_path;

	pin_t 		gpio_pin;
	mode  		gpio_mode;
	bit_t		gpio_state;

	bool set = false;
};	
