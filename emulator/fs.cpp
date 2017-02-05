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
#include <iostream>

#include "fs.h"

using namespace std;

namespace gpiemu {
	/* Fatal error */
	void fatal(string msg) {
		cout << msg << endl;
		fs_ret fs_msg = rm_r(GPIO_SYSFS_PATH);
	
		if (fs_msg != SUCCESS) {
			cout << "Failed to remove emulated sysfs on error termination" << endl
			     << "Reason : " << get_fs_error(fs_msg) << endl;
		}

		exit(EXIT_FAILURE);
	}

	/* Truncate/Clean file *
	 * 
	 * Returns :
	 * On failure : false
	 * On success : true   */
	bool trunc(string path) {
		bool ret = false;
		string content;

		ofstream clean(path.c_str());
		clean.close();

		ifstream check(path.c_str());

		if(check) {
			ret = true;
			check >> content;
		}
		
		return (ret && content == "");

	}
}

/* Remove directory that contains files (Take care when using!) 
 *
 * Return :
 * Failed to initialize directory stream (of type DIR*) : DIRSTREAM_ERR 
 * Failed to remove empty directory                     : RM_ERR
 * Failed to remove file				: RMDIR_ERR
 * Successfully removed directory                       : SUCCESS */
fs_ret rm_r(string path) {

	path_trace p(path);

	dirent *entry;

	DIR *dirstream  = opendir(p.cd().c_str());

	if (!dirstream) {
		return DIRSTREAM_ERR;
	}

	while(true) {
		entry = readdir(dirstream);
		if (entry) {
			if (entry->d_type == DT_DIR) {
				if (string(entry->d_name) != "." &&
			 	    string(entry->d_name) != ".." ) {
					if (rmdir((p.cd() + '/' + string(entry->d_name)).c_str()) < 0) {
						p.go_to(string(entry->d_name));
						if(!(dirstream = opendir(p.cd().c_str())))
							return DIRSTREAM_ERR;
					}
				}
			} 
			
			else {
				if (remove((p.cd() + '/' + string(entry->d_name)).c_str()) < 0)
					return RM_ERR;
			}
		}

		else if (p.cd() == path) {
			return (rmdir(path.c_str()) == 0) ? SUCCESS : RMDIR_ERR;
		}

		else {
			p.go_back();
			if (!(dirstream = opendir(p.cd().c_str())))
				return DIRSTREAM_ERR;
		}
	}
}

/* Recursive directory spawning 
 *
 * Returns :
 * On failure : false
 * On success : true            */
bool mkdir_p(string path, mode_t permissions) {
	bool ret = false;
	
	/* Check wether path end with '/', if not, add it to make our algorithm functional */
	if (path[path.size() + 1] != '/')
		path += '/';

	/* Try mkdir on every path until end has been reached */
	for (int i = 0; i < path.size(); i++) {
		if (path[i] == '/' && i != 0) {
			ret = (mkdir (path.substr(0, i).c_str(), permissions) == 0); // Ignore if mkdir failed

		}
	}

	return ret; // False if failed
}
