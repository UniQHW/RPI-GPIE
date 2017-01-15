# Copyright 2017 Patrick Pedersen <ctx.xda@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LIB_EXPORT_PATH	   := /usr/local/lib/
HEADER_EXPORT_PATH := /usr/local/include/

ALL: src/gpie.cpp src/gpie.h
	@ if [ -d out ]; then \
		make clean; \
	  fi;

	@ make out
	@ echo
	g++ -fPIC -shared src/gpie.cpp -o out/libgpie.so

out :
	@mkdir out

clean : out
	@ rm -R out
	@ echo done

install : out src/gpie.h out/libgpie.so
	@cp src/gpie.h $(HEADER_EXPORT_PATH)
	@cp out/libgpie.so $(LIB_EXPORT_PATH)
	@echo done

# Examples
blink   : $(LIB_EXPORT_PATH)/libgpie.so examples/blink.cpp out
	g++ -Wl,-rpath,$(LIB_EXPORT_PATH) examples/blink.cpp -lgpie -o out/blink
