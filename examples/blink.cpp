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


/* Blink.cpp
 * Blinking LED example for GPIE

 * Description : A simple programm that turns the LED on and off 10 times in an interval of 1 second

 * Setup       : Please attach a LED + Resistor configuration on GPIO 17 to successfully execute this example 
 * Compiling   : To successfully compile an executable on your RPI, please execute the following commands :

	g++ Blink.cpp -lgpie -o blink

   Executing   : To start the executable, run :

	./blink
 */

#include <unistd.h> // For sleep
#include <iostream> // For std::cout
#include <gpie.h>

gpio LED(17, OUT); // LED on pin 17 set to write access

int main() 
{
	if(LED.is_set())
	{
		/* 20 Times as ON takes 10 iterations and OFF takes 10 iterations */
		for(int i = 0; i < 20; i++) 
		{
			LED.toggleState(); // Toggles ON/OFF
			sleep(1);
		}
	} 
	else
	{
		std::cout << "Failed to initialize LED!" << std::endl;
	}

	return 0;
}
