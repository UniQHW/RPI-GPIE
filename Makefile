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

# TODO: Do not re-build targets

CC		         := g++
CFLAGS		         := -g -Wall

TARGET_OUT		 := out

# GPIE API library
GPIE_SRC		 := gpie/gpie.cpp
TARGET_OUT_GPIE		 := libgpie
TARGET_OUT_GPIE_EMU	 := libgpie_emu

# Emulator core
EMULATOR_CORE_SRC        := emulator/init.cpp \
			    emulator/fs.cpp   \
			    emulator/sysfs.cpp\

TARGET_OUT_EMULATOR_CORE := out/emulator/init.o  \
		            out/emulator/fs.o    \
			    out/emulator/sysfs.o

# Emulator CLI
EMULATOR_CLI_SRC         := emulator/cli/emulation.cpp
TARGET_OUT_EMULATOR_CLI  := emucli

# Blink example
BLINK_EXAMPLE_SRC        := examples/blink.cpp
TARGET_OUT_BLINK_EXAMPLE := blink

# Installation targets
TARGET_INSTALL_GPIE	 := install-gpie
TARGET_INSTALL_CLI_EMU   := install-emucli

# Build GPIE API library
$(TARGET_OUT_GPIE): $(TARGET_OUT) $(GPIE_SRC)
	$(CC) $(CFLAGS) -fPIC -shared -Iinclude $(GPIE_SRC) -o out/$(TARGET_OUT_GPIE).so

$(TARGET_OUT_GPIE_EMU): $(TARGET_OUT) $(GPIE_SRC)
	$(CC) $(CFLAGS) -fPIC -shared -Iinclude -DEMULATOR $(GPIE_SRC) -o out/$(TARGET_OUT_GPIE).so

# API Emulator CLI
$(TARGET_OUT_EMULATOR_CLI): $(TARGET_OUT_EMULATOR_CORE) $(EMULATOR_CLI_SRC)
	$(CC) $(CFLAGS) -DEMULATOR -Iinclude -Iemulator/include $(TARGET_OUT_EMULATOR_CORE) $(EMULATOR_CLI_SRC) -o out/$(TARGET_OUT_EMULATOR_CLI)

# Core sources for API emulator
$(TARGET_OUT_EMULATOR_CORE): $(TARGET_OUT) $(EMULATOR_CORE_SRC)
	@mkdir -p out/emulator
	$(CC) $(CFLAGS) -DEMULATOR -Iinclude -Iemulator/include -c $(EMULATOR_CORE_SRC)
	@mv *.o out/emulator

# Blink example
$(TARGET_OUT_BLINK_EXAMPLE): $(TARGET_OUT) $(BLINK_EXAMPLE_SRC)
	@mkdir -p out/examples
	$(CC) $(CFLAGS) -Wl,-rpath,/usr/local/lib $(BLINK_EXAMPLE_SRC) -lgpie -o out/examples/$(TARGET_OUT_BLINK_EXAMPLE)

# Install GPIE API Library
$(TARGET_INSTALL_GPIE): $(TARGET_OUT_GPIE)
	@ if [[ $(USER) == "root" ]]; then \
		echo "Installing gpie header to /usr/local/include"; \
		cp gpie/gpie.h /usr/local/include; \
		echo "Installing gpie library to /usr/local/lib"; \
		cp out/libgpie.so /usr/local/lib; \
		echo done; \
	  else \
  		echo "Please execute as root"; \
	  fi

$(TARGET_INSTALL_CLI_EMU): $(TARGET_OUT_EMULATOR_CLI)
	@ if [[ $(USER) == "root" ]]; then \
		echo "Installing emulator to /usr/local/bin"; \
		cp out/emucli /usr/local/bin; \
		echo done; \
	  else \
  		echo "Please execute as root"; \
	  fi

$(TARGET_OUT):
	@mkdir -p out

clean: out
	@rm -R out
