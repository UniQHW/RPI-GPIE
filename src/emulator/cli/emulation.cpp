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
#include "sysfs.h"
#include "gpio.h"

using namespace std;

/* Clears stdout fd */
void clearStdout() {
	cout << "\033c" << endl;
}

class GpioTable {
public:
	GpioTable(gpio *gpio_array[GPIO_PIN_COUNT]) {
		for (int i = 0; i < GPIO_PIN_COUNT; i++) {
			gpios[i] = &(gpio_array[i]);
			cout << gpios[i] << " " << gpio_array[i] << endl;
 		}
	}

	bool draw() {
		bool ret;
		if(ret = checkTable()) {
			clearStdout();
			cout << "|GPIO\t" << "|Status\t\t\t" << "|Mode\t" << "|Value\t|" << endl;
		
			for (int i = 0; i < GPIO_PIN_COUNT; i++) {
				cout << '|' << i 		     << '\t' << 
				        '|' << gpio_status_stamps[i] << '\t' << 
				        '|' << gpio_mode_stamps[i]   << '\t' << 
					'|' << gpio_value_stamps[i]  << '\t' <<
					'|' << endl;
			}
		}
		return ret;
	}
				

private:
	bool checkTable() {
		bool ret = false; 
		for (int i = 0; i < GPIO_PIN_COUNT; i++) {
			if (*gpios[i]) 
			{
				if (gpio_status_stamps[i] != "Initialized\t") {
					gpio_status_stamps[i]  = "Initialized\t";
					ret = true;
				}

				switch(check_direction(*gpios[i])) {

					case out :	if (gpio_mode_stamps[i] != "output") {
								gpio_mode_stamps[i]  = "output";
								ret = true;
							}
					break;

					default  :	if (gpio_mode_stamps[i] != "input") {
								gpio_mode_stamps[i]  = "input";
								ret = true;
							}  
					break;
				}

				switch(check_value(*gpios[i])) {
					case 0  :	if(gpio_value_stamps[i] != "low") {
								gpio_value_stamps[i] = "low";
								ret = true;
							}
					break;

					default :	if(gpio_value_stamps[i] != "high") {
								gpio_value_stamps[i] = "high";
								ret = true;
							}
					break;
				}
			}
			else 
			{
				if (gpio_status_stamps[i] != "Not initialized") {
					gpio_status_stamps[i]  = "Not initialized";
					gpio_mode_stamps[i] = "";
					gpio_value_stamps[i] = "";
					ret = true;
				}
			}			
		}

		return ret;
	}
	
	gpio   **gpios               [GPIO_PIN_COUNT];
	string  gpio_status_stamps   [GPIO_PIN_COUNT];
	string  gpio_value_stamps    [GPIO_PIN_COUNT];
	string  gpio_mode_stamps     [GPIO_PIN_COUNT];
};

bool emulate() {
	gpio* gpios[GPIO_PIN_COUNT];
	
	/* Point all gpios to null on init */
	for (int i = 0; i < GPIO_PIN_COUNT; i++) {
		gpios[i] = NULL;
	}

	GpioTable table(gpios);

	/* Run till exit */
	bool exit = false;
	while(!exit) {
		gpio *new_gpio = check_export();
		
		/* Assign new gpio */		
		if (new_gpio) {
			cout << "New GPIO " << new_gpio->get_pin() << endl;
			gpios[new_gpio->get_pin()] = new_gpio;
		}
		
		/* Kill gpio */
		int8_t killed_gpio_pin = check_unexport();
		if (killed_gpio_pin > 0) {
			kill_gpio(gpios[killed_gpio_pin]);
			gpios[killed_gpio_pin] = NULL;
		}

		table.draw();
	}
}
