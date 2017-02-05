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

CC		         := g++
CFLAGS		         := -g -Wall

# GPIE API library
GPIE_SRC		 := gpie/gpie.cpp
TARGET_OUT_GPIE		 := out/libgpie.so

# Emulator core
EMULATOR_CORE_SRC        := emulator/init.cpp \
			    emulator/fs.cpp   \
			    emulator/sysfs.cpp\

TARGET_OUT_EMULATOR_CORE := out/emulator/init.o  \
		            out/emulator/fs.o    \
			    out/emulator/sysfs.o

# Emulator CLI
EMULATOR_CLI_SRC         := emulator/cli/emulation.cpp
TARGET_OUT_EMULATOR_CLI  := out/emucli

# Blink example
BLINK_EXAMPLE_SRC        := examples/blink.cpp
TARGET_OUT_BLINK_EXAMPLE := out/examples/blink

# API
libgpie: $(TARGET_OUT_GPIE)

# API  for emulator
libgpie_emu: CFLAGS += -DEMULATOR
libgpie_emu: $(TARGET_OUT_GPIE)

# Emulator client
emucli: $(TARGET_OUT_EMULATOR_CLI)

# Examples
blink: $(TARGET_OUT_BLINK_EXAMPLE)

# Build GPIE API library
$(TARGET_OUT_GPIE): $(GPIE_SRC)
	@mkdir -p out
	$(CC) $(CFLAGS) -fPIC -shared -Iinclude $(GPIE_SRC) -o $(TARGET_OUT_GPIE)

# API Emulator CLI
$(TARGET_OUT_EMULATOR_CLI): $(TARGET_OUT_EMULATOR_CORE) $(EMULATOR_CLI_SRC)
	$(CC) $(CFLAGS) -DEMULATOR -Iinclude -Iemulator/include $(TARGET_OUT_EMULATOR_CORE) $(EMULATOR_CLI_SRC) -o $(TARGET_OUT_EMULATOR_CLI)

# Core sources for API emulator
$(TARGET_OUT_EMULATOR_CORE): $(EMULATOR_CORE_SRC)
	@mkdir -p out/emulator
	$(CC) $(CFLAGS) -DEMULATOR -Iinclude -Iemulator/include -c $(EMULATOR_CORE_SRC)
	@mv *.o out/emulator

# Blink example
$(TARGET_OUT_BLINK_EXAMPLE): $(BLINK_EXAMPLE_SRC)
	@mkdir -p out/examples
	$(CC) $(CFLAGS) -Wl,-rpath,/usr/local/lib $(BLINK_EXAMPLE_SRC) -lgpie -o $(TARGET_OUT_BLINK_EXAMPLE)

# Install GPIE API Library
install-gpie: $(TARGET_OUT_GPIE)
	@ if [[ $(USER) == "root" ]]; then \
		echo "Installing gpie header to /usr/local/include"; \
		cp gpie/gpie.h /usr/local/include; \
		echo "Installing gpie library to /usr/local/lib"; \
		cp out/libgpie.so /usr/local/lib; \
		echo done; \
	  else \
  		echo "Please execute as root"; \
	  fi

install-emucli: $(TARGET_OUT_EMULATOR_CLI)
	@ if [[ $(USER) == "root" ]]; then \
		echo "Installing emulator to /usr/local/bin"; \
		cp out/emucli /usr/local/bin; \
		echo done; \
	  else \
  		echo "Please execute as root"; \
	  fi

clean: out
	@rm -R out
