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

#include <iostream> // DEBUGGING ONLY
#include "sysfs.h"

/* TODO: - Prevent overlapping sysfs 
 * 	 - Change error logs according to gpio->check() */

using namespace std;

/** GPIO metadata retrieval **/

/* Obtain gpio pin as string from gpio obj ptr */
string get_gpio_pin(gpio *gpiobj) {
	return to_string(gpiobj->get_pin());
}

/* Obtain gpio sysfs path from gpio obj ptr */
string get_gpio_sysfs(gpio *gpiobj) {
	return sysfs_path + "/gpio" + get_gpio_pin(gpiobj);
}

/* Obtain gpio value path from gpio obj ptr */
string get_gpio_value(gpio *gpiobj) {
	return sysfs_path + "/gpio" + get_gpio_pin(gpiobj) + "/value";
}

/* Obtain gpio direction path from gpio obj ptr */
string get_gpio_direction(gpio *gpiobj) {
	return sysfs_path + "/gpio" + get_gpio_pin(gpiobj) + "/direction";
}

/* Check with rpi ! */

/* Export/Spawn gpio sysfs and object 
 *
 * Returns
 * On failure : NULL
 * On success : Pointer to new and initialized gpio object */
gpio* spawn_gpio(uint8_t pin) {
	if (pin <= GPIO_PIN_COUNT) {
		fs_ret msg;
		string gpio_sysfs_path = sysfs_path + "/gpio" + to_string(pin);
		DIR *dstream = opendir(gpio_sysfs_path.c_str());

		/* Check if gpio sysfs has already been exported, if so, delete it */
		if (dstream && readdir(dstream)) {
			if ((msg = rm_r(gpio_sysfs_path)) != SUCCESS) {
				gpiemu::fatal("Failed to remove present sysfs path for gpio" + to_string(pin)
					    + "\nReason: " + get_fs_error(msg));
			}
		}
		
		/* Attempt to create new gpio sysfs */
		if (!mkdir_p(gpio_sysfs_path, 777)) {
			gpiemu::fatal("Failed to create sysfs path for gpio" + to_string(pin));
		}

		
		/* Spawn export and direction files */
		ofstream((gpio_sysfs_path + "/direction").c_str());
		ofstream((gpio_sysfs_path + "/value").c_str(), ios::binary);

		/* Spawn new gpio object */
		gpio *gpiobj = new gpio(pin, gpio_sysfs_path + "/direction", gpio_sysfs_path + "/value");	

		/* Check for failure */
		if (!gpiobj->check()) {
			gpiemu::fatal("Failed to open direction and value file descriptor gpio" + get_gpio_pin(gpiobj));
		}

		return gpiobj;
	}
}

// TODO: Kill terminate gpio safely
/* Terminate GPIO object and sysfs 
 *
 * Returns :
 * On failure : false
 * On success : true 		   */
bool kill_gpio(gpio *gpiobj) {
	bool ret = false;
	string gpio_sysfs = get_gpio_sysfs(gpiobj);
	string gpio_pin	  = get_gpio_pin(gpiobj);

	DIR *gpio_sysfs_dstream = opendir(gpio_sysfs.c_str());

	if(gpio_sysfs_dstream && readdir(gpio_sysfs_dstream)) {
		gpiobj = NULL;
		ret = !gpiobj;
		delete gpiobj; // Let destructor close the fd for us
	}

	if (rm_r(gpio_sysfs) != SUCCESS) {
		gpiemu::fatal("Failed to kill gpio" + gpio_pin);
	}

	return ret;
}

/* Check export file for new gpio 
 * Returns :
 * New gpio    : Pointer to new and initialized gpio object
 * No new gpio : NULL 					    */
gpio *check_export() {
	gpio *export_gpio = NULL; // Set default NULL, we're expecting that the file will be empty for the majority of its time

	ifstream export_fd(export_path.c_str());

	/* Read export file */
	if(export_fd) {
		string gpio_val_str;
		export_fd >> gpio_val_str;

		if (gpio_val_str != "") {
			unsigned int gpio_val = stoi(gpio_val_str);
			if (gpio_val <= GPIO_PIN_COUNT)
				export_gpio = spawn_gpio(gpio_val); // Retrieve pointer to new gpio object
		
			if (!gpiemu::trunc(export_path))
				gpiemu::fatal("Failed to clean export file descriptor");
		}
	} else {
		gpiemu::fatal("Failed to allocate export file descriptor");
	}

	return export_gpio;
}

/* Check unexport file for gpio termination.
 *
 * Return : 
 * Empty/Failure  : -1
 * FIlled         : Pin of terminated gpiobj */
int8_t check_unexport() {
	int8_t gpio_val;
	ifstream unexport_fd(unexport_path.c_str());

	/* Read unexport file */
	if (unexport_fd) {
		string s;
		unexport_fd >> s;

		if (s != "") {
			gpio_val = stoi(s);
			if (gpio_val < 0)
				gpio_val = -1; // Invalid gpio

			if(!gpiemu::trunc(unexport_path.c_str()))
					gpiemu::fatal("Failed to clean unexport file descriptor");
		}

	} else {
		gpiemu::fatal("Failed to allocate unexport file descriptor");
	}

	return gpio_val;
}

/* Obtain value of gpiopbject/device
 *
 * Returns :
 * GPIO Emmits or recieves HIGH : true
 * GPIO Emmits or recieves LOW  : false */
bool check_value(gpio* gpiobj) {
	if (!gpiobj->check())
		gpiemu::fatal(" Failed to read value for gpio" + get_gpio_pin(gpiobj));
	return gpiobj->read_value();
}

/* Obtain pin mode of gpiobjec/device
 * 
 * Returns :
 * direction_t aka bool on input  : in/false
 * direction_t aka bool on output : out      */
direction_t check_direction(gpio* gpiobj) {
	if(!gpiobj->check())
		gpiemu::fatal("Failed to read mode/direction for gpio" + get_gpio_pin(gpiobj));
	
	return gpiobj->read_direction();
}
