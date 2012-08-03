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
TARGET_DIRS := src \
               lib

# include the common envionment variables
include ./common.mk

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
	@echo "===========================Make WLAN CGI Begin==========================="	
	@$(call build_obj)
	${CC} -Wall -o wlan.cgi ./obj/*.o
	@echo "===========================Make WLAN CGI end============================="
clean:
	-rm ./obj/*.o ./out ./obj/*.so wlan.cgi

