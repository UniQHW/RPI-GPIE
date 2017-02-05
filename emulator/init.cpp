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
#include <fstream>

#include "fs.h"
#include "gpio.h"

using namespace std;

bool emulate();

int main() {
	DIR *d = opendir(GPIO_SYSFS_PATH);
	if (d && readdir(d)) {
		fs_ret msg = rm_r(GPIO_SYSFS_PATH);
		if (msg != SUCCESS) {
			std::cout << "Failed to remove emulated GPIO sysfs, reason : " << get_fs_error(msg) << std::endl;
			return -1;
		}
	}

	if (!mkdir_p(GPIO_SYSFS_PATH, 777)) {
		std::cout << "Failed to create emulated GPIO sysfs, are you running as root?" << std::endl;
		return -1;
	}

	/* Spawn export and unexport fd */
	fstream export_fd(export_path.c_str(), std::ifstream::out);
	fstream unexport_fd(unexport_path.c_str(), std::ifstream::out);

	/* Close files as they'll be re-opened in read-only */
	if (export_fd && unexport_fd) {
		export_fd.close();
		unexport_fd.close();
	} else {
		std::cout << "Failed to spawn GPIO export and unexport file descriptors" << std::endl;
		return -1;
	}

	return emulate(); // Start emulation
}
