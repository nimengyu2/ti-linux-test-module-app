#
# @file Makefile
# @brief main makefile
#
# Copyright (C) 2006-2011, toby
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# @author toby.zhang <toby.chs@gmail.com>
# @date 2011-6-23 22:10:36
# @version 0.01
#
# This file is pary of makefile project.
#

# global variable
export MAKEROOT := $(shell pwd)

# the directory to be make
TARGET_DIRS := html-flush \
               login \
               serial \
               wifi \
               wlan \
               mswitch 

# include the common envionment variables
# Do not include comman.mk here
#include ../common.mk

# build shell
define build_obj
for SubDir in $(TARGET_DIRS); do \
    if ! [ -d $$SubDir ]; then \
        echo "The $$SubDir is not exist !"; \
        exit 11; \
    fi; \
    echo "Building $$SubDir ..."; \
    make -C $$SubDir ; \
    if [ $$? -ne 0 ]; then \
echo "Building $$SubDir is failed !"; \
        exit 12; \
    fi; \
done
endef

all:
	@echo "===========================Make CGI-BIN Begin==========================="	
	@$(call build_obj)
	@echo "===========================Make CGI-BIN end============================="

clean:
	-rm ./html-flush/obj/*.o ./html-flush/obj/*.so ./html-flush/s2wifi.cgi ./login/obj/*.o ./login/obj/*.so ./login/login.cgi ./serial/obj/*.o ./serial/obj/*.so ./serial/serial.cgi ./wifi/obj/*.o ./wifi/obj/*.so ./wifi/wifi.cgi ./wlan/obj/*.o ./wlan/obj/*.so ./wlan/wlan.cgi  ./mswitch/obj/*.o ./mswitch/obj/*.so ./mswitch/mswitch.cgi

