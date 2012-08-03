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
TARGET_DIRS := cgi-bin-midea \
               src \
               lib \
               wifi

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
	@echo "===========================Make S2WIFI MAIN Begin==========================="
	@$(call build_obj)
	${CC} -Wall -o s2wifi ./obj/*.o
	@echo "===========================Make S2WIFI MAIN End============================="

install:
	@echo "===========================Install bin Begin==========================="
	mkdir ./bin/www/cgi-bin
	cp ./cgi-bin-midea/html-flush/s2wifi.cgi ./bin/www/cgi-bin/
	cp ./cgi-bin-midea/login/login.cgi ./bin/www/cgi-bin/
	cp ./cgi-bin-midea/serial/serial.cgi ./bin/www/cgi-bin/
	cp ./cgi-bin-midea/wifi/wifi.cgi ./bin/www/cgi-bin/
	cp ./cgi-bin-midea/wlan/wlan.cgi ./bin/www/cgi-bin/
	cp ./cgi-bin-midea/mswitch/mswitch.cgi ./bin/www/cgi-bin/
	cp -rf ./WEB-fs-midea/htmlfs/* ./bin/www/
	cp ./WEB-fs-midea/login.cfg ./bin/www/cgi-bin/
	cp ./WEB-fs-midea/s2wifi.cfg ./bin/www/cgi-bin/
	chmod 777 ./bin/www/cgi-bin/login.cfg
	chmod 777 ./bin/www/cgi-bin/s2wifi.cfg
	cp ./s2wifi ./bin/
	@echo "===========================Install bin End==========================="

clean:
#	-rm ./obj/*.o ./out ./obj/*.so s2wifi
	-rm -f */*.o */*/*.o */*/*/*.o s2wifi ./cgi-bin-midea/html-flush/s2wifi.cgi ./cgi-bin-midea/login/login.cgi ./cgi-bin-midea/serial/serial.cgi ./cgi-bin-midea/wifi/wifi.cgi ./cgi-bin-midea/wlan/wlan.cgi ./cgi-bin-midea/mswitch/mswitch.cgi
	-rm -rf ./bin/www/* ./bin/s2wifi
	@echo "Clean Done"

