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

# I'm bad at makefiles, deal with it

CC		   := g++ -g
LIB_FLAGS	   := -fPIC -shared
LIB_EXPORT_PATH	   := /usr/local/lib
HEADER_EXPORT_PATH := /usr/local/include
LIB_LINK_FLAGS     := -Wl,-rpath,$(LIB_EXPORT_PATH)

GLOBAL_INCLUDE	   := src/include
EMU_INCLUDE	   := src/emulator/include

TARGET_GPIE 	   := src/gpie/gpie.cpp
TARGET_GPIE_OUT	   := libgpie.so

TARGET_EMU_CORE    := src/emulator/fs.cpp \
		      src/emulator/sysfs.cpp \
		      src/emulator/init.cpp

TARGET_EMU_CLI     := $(TARGET_EMU_CORE) \
		      src/emulator/cli/emulation.cpp

TARGET_EMU_CLI_OUT := gpiemucli

# GPIE build
ALL: $(TARGET_GPIE) out
	@ echo
	$(CC) $(LIB_FLAGS) -I$(GLOBAL_INCLUDE) $(TARGET_GPIE) -o out/$(TARGET_GPIE_OUT)

emubuild: emugpie emucli
	@echo done

# Explicit GPIE Emulator build
emugpie: $(TARGET_GPIE) out 
	$(CC) $(LIB_FLAGS) -DEMULATOR -I$(GLOBAL_INCLUDE) $(TARGET_GPIE) -o out/$(TARGET_GPIE_OUT)

# Emulator build (Commandline interface)
emucli: $(TARGET_EMU_CLI) out
	$(CC) $(LIB_LINK_FLAGS) -DEMULATOR -I$(GLOBAL_INCLUDE) -I$(EMU_INCLUDE) $(TARGET_EMU_CLI) -lgpie -o out/$(TARGET_EMU_CLI_OUT)
	@echo done

## Examples
examples: $(LIB_EXPORT_PATH)/libgpie.so $(HEADER_EXPORT_PATH)/gpie.h examples/ out \
	   blink

# Blink Example
blink: $(LIB_EXPORT_PATH)/libgpie.so examples/ out
	$(CC) -Wl,-rpath,$(LIB_EXPORT_PATH) examples/blink.cpp -lgpie -o out/blink

# Create output dir if necessary
out:
	@if [ ! -d out ]; then \
		mkdir out; \
	 fi

# Clean build env
clean: out
	@ echo cleaning...
	@ rm -R out
	@ echo done

# Install gpie lib
install-gpie: out/$(TARGET_GPIE_OUT)
	@ if [[ $(USER) == "root" ]]; then \
		echo "Installing gpie header to $(HEADER_EXPORT_PATH)"; \
		cp src/gpie/gpie.h $(HEADER_EXPORT_PATH); \
		echo "Installing gpie library to $(LIB_EXPORT_PATH)"; \
		cp out/libgpie.so $(LIB_EXPORT_PATH); \
		echo done; \
	  else \
		echo "Please execute as root"; \
	  fi

# Uninstall gpie lib
uninstall-gpie: $(LIB_EXPORT_PATH)/libgpie.so $(HEADER_EXPORT_PATH)/gpie.h
	@ if [[ $(USER) == "root" ]]; then \
	       echo "Removing $(LIB_EXPORT_PATH)/libgpie.so and $(HEADER_EXPORT_PATH)/gpie.h"; \
	       rm $(LIB_EXPORT_PATH)/libgpie.so \
	       	  $(HEADER_EXPORT_PATH)/gpie.h; \
	       echo "done"; \
	 else \
	       echo "Please execute as root"; \
	 fi
