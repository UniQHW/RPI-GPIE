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

#ifndef FS_H_INCLUDED
#define FS_H_INCLUDED

#include <algorithm>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <sysfs_paths.h>

using namespace std;

namespace gpiemu {
	void fatal(string msg);
	bool trunc(string path);
}

/* Filesystem return */
enum fs_ret {
	SUCCESS		= 1,
	DIRSTREAM_ERR,
	RM_ERR,
	RMDIR_ERR
};

/* fs_ret wrapper */
inline string get_fs_error(fs_ret ret) {
	switch (ret) {
		case DIRSTREAM_ERR 	: return "Failed to allocate stream for directory";
		case RM_ERR	   		: return "Failed to remove file"		     ;
		case RMDIR_ERR	   	: return "Failed to remove directory"	     ;
		default		   		: return "No error"				     ;
	}
}

/* Path log */
struct path_trace {
	path_trace(string init_path) {
		path_log.push_back(init_path);
	}

	string cd() {
		return path_log.back();
	}

	void go_to(string sub) {
		path_log.push_back(path_log.back() + '/' + sub);
	}

	bool go_back() {
		bool ret;
		if(ret = path_log.size())
			path_log.pop_back();
		return ret;
	}

private:
	vector<string> path_log;
};

/* Recursive directory removing */
fs_ret rm_r(string path);

/* Recursive directory spawning */
bool mkdir_p(string path, mode_t permissions);

#endif // FS_H_INCLUDED
