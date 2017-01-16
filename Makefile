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

CC		   := g++
LIB_EXPORT_PATH	   := /usr/local/lib
HEADER_EXPORT_PATH := /usr/local/include
USER		   := $(shell whoami)

ALL      : src/gpie.cpp src/gpie.h
	@ if [ -d out ]; then \
		make clean; \
	  fi;

	@ make out
	@ echo
	$(CC) -fPIC -shared src/gpie.cpp -o out/libgpie.so

out      :
	@mkdir out

clean    : out
	@ rm -R out
	@ echo done

install  : out src/gpie.h out/libgpie.so
	@ if [[ $(USER) == "root" ]]; then \
		echo "Installing gpie header to $(HEADER_EXPORT_PATH)"; \
		cp src/gpie.h $(HEADER_EXPORT_PATH); \
		echo "Installing gpie library to $(LIB_EXPORT_PATH)"; \
		cp out/libgpie.so $(LIB_EXPORT_PATH); \
		echo done; \
	  else \
		echo "Please execute as root"; \
	  fi

uninstall: $(LIB_EXPORT_PATH)/libgpie.so $(HEADER_EXPORT_PATH)/gpie.h
	@ if [[ $(USER) == "root" ]]; then \
	       echo "Removing $(LIB_EXPORT_PATH)/libgpie.so and $(HEADER_EXPORT_PATH)/gpie.h"; \
	       rm $(LIB_EXPORT_PATH)/libgpie.so \
	       	  $(HEADER_EXPORT_PATH)/gpie.h; \
	       echo "done"; \
	 else \
	       echo "Please execute as root"; \
	 fi		

# Examples
examples : $(LIB_EXPORT_PATH)/libgpie.so $(HEADER_EXPORT_PATH)/gpie.h examples/ out \
	   blink

blink    : $(LIB_EXPORT_PATH)/libgpie.so examples/ out
	$(CC) -Wl,-rpath,$(LIB_EXPORT_PATH) examples/blink.cpp -lgpie -o out/blink
