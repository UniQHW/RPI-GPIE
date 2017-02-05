
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

#ifndef SYSFS_PATHS_H
#define SYSFS_PATHS_H

#include <string>

/* This file contains paths for gpio sysfs descriptors */
#ifdef  EMULATOR		// Compiling for Emulator
#define GPIO_SYSFS_PATH		"/tmp/gpie-emu/gpio"
#else				// Compiling for RPI
#define GPIO_SYSFS_PATH		"/sys/class/gpio"
#endif

/* "Stringify" paths */
const std::string sysfs_path    = std::string(GPIO_SYSFS_PATH);
const std::string export_path   = sysfs_path + "/export";
const std::string unexport_path = sysfs_path + "/unexport";


#endif
