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

#include <iostream>
#include <string>
#include <sstream>

#include <sysfs_paths.h>

#include "gpie.h"

using namespace std; // TODO: Remove all std:: calls

/* In G++ 4.9.2, std::to_string is not defined in the standard libraries under ARM Linux */
template <typename int_type> std::string to_string(int_type val) {
	std::stringstream s;
	s << int(val);
	return s.str();
}

gpio::gpio(pin_t pin, mode io = OUT) : gpio_pin(pin), gpio_mode(io) {
	const std::string pin_str = to_string<pin_t>(pin);

#ifdef EMULATOR
	cout << "Waiting for export file to be spawned, make sure your emulator is running" << endl;
	while(!ifstream(export_path.c_str()).is_open()); // Wait till export file is spawned
	cout << "Export file found!" << endl;
#endif

	std::ofstream export_fd(export_path.c_str(), std::ofstream::out | std::ofstream::binary);

	if(export_fd) {
		export_fd << pin_str;
		export_fd.close();
		mode_fd_path = std::string(GPIO_SYSFS_PATH) + "/gpio" + pin_str + "/direction";

		while(!ifstream(mode_fd_path.c_str()).is_open()); // Wait till direction file is spawned TODO: Add timeout
		
		mode_fd.open(mode_fd_path.c_str(), std::ofstream::out);
	} else {
		std::cout << "Failed to open control interface for GPIO " << pin_str

#ifdef EMULATOR
		<< std::endl << "Is your GPIE emulator running?"
#endif	

		<< std::endl;
	}

	if(mode_fd) {
		if(io) mode_fd << "out" << std::flush;
		else   mode_fd << "in"  << std::flush;
		mode_fd.close();
	
		gpio_fd_path = std::string(GPIO_SYSFS_PATH) + "/gpio" + pin_str + "/value";
		set = true;
	} else {
		std::cout << "Failed to set mode for GPIO " << pin 
#ifdef EMULATOR
		<< std:: endl << "Is your GPIE emulator running?"
#endif	
		<< std::endl;
	}
}

gpio::~gpio() {
	const std::string pin_str = to_string<pin_t>(gpio_pin);
	std::ofstream unexport_fd(unexport_path.c_str());

	write(OFF);

	if(unexport_fd) {
		unexport_fd << pin_str;
		unexport_fd.close();
		mode_fd.close();
		gpio_fd.close();

	}
}

mode gpio::getMode() {
	return gpio_mode;
}

void gpio::setMode(mode io) {
	mode_fd.open(mode_fd_path.c_str(), std::ofstream::out);
		if(io) mode_fd << "out" << std::flush;
		else   mode_fd << "in"  << std::flush;
	mode_fd.close();

	gpio_mode = io;
}

mode gpio::toggleMode() {
	mode_fd.open(mode_fd_path.c_str(), std::ofstream::out);
		if(gpio_mode) {
			mode_fd << "in";
			gpio_mode = in;
		} else {
			mode_fd << "out";
			gpio_mode = out;	
		}
	mode_fd.close();

	return gpio_mode;
}

bool gpio::write(bit_t state) {
	bool ret;
	if(ret = (gpio_mode == OUTPUT)) {
		gpio_fd.open(gpio_fd_path.c_str(), std::ofstream::out | std::ios::binary);
		
		if(gpio_fd)
			gpio_fd << state << std::flush;
		else
			cout << "Failed to write on gpio " << gpio_pin << endl;
		

		gpio_fd.close();

		gpio_state = state;
	}
	return ret;
}

bit_t gpio::toggleState() {
	if(gpio_mode == OUTPUT) {
		gpio_state = !gpio_state;
		gpio_fd.open(gpio_fd_path.c_str(), std::ofstream::out | std::ios::binary);
		
		if(gpio_fd)
			gpio_fd << gpio_state << std::flush;
		else
			cout << "Failed to toggle state on gpio" << gpio_pin << endl;
		
		gpio_fd.close();

		return gpio_state;
	}
	return 0;
}

bit_t gpio::read() {
	gpio_fd.open(gpio_fd_path.c_str(), std::ifstream::in | std::ios::binary);
		gpio_fd >> gpio_state;
	gpio_fd.close();

	return gpio_state;
}

bool gpio::is_set() {
	return set;
}
