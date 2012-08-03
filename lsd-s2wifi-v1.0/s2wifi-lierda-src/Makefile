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
TARGET_DIRS := cgi-bin \
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
	cp ./cgi-bin/cgi-bin/s2wifi.cgi ./bin/www/cgi-bin/
	cp ./cgi-bin/cgi-bin/login.cgi ./bin/www/cgi-bin/
	cp ./cgi-bin/cgi-bin/serial.cgi ./bin/www/cgi-bin/
	cp ./cgi-bin/cgi-bin/wifi.cgi ./bin/www/cgi-bin/
	cp ./cgi-bin/cgi-bin/wlan.cgi ./bin/www/cgi-bin/
	cp ./cgi-bin/cgi-bin/mswitch.cgi ./bin/www/cgi-bin/
	cp -rf ./WEB-fs/htmlfs/* ./bin/www/
	rm ./bin/www/s2wifi.html
	cp ./WEB-fs/login.cfg ./bin/www/cgi-bin/
	cp ./WEB-fs/s2wifi.cfg ./bin/www/cgi-bin/
	chmod 777 ./bin/www/cgi-bin/login.cfg
	chmod 777 ./bin/www/cgi-bin/s2wifi.cfg
	cp ./s2wifi ./bin/
	@echo "===========================Install bin End==========================="

clean:
#	-rm ./obj/*.o ./out ./obj/*.so s2wifi
	-rm -f */*.o */*/*.o */*/*/*.o s2wifi ./cgi-bin/cgi-bin/s2wifi.cgi ./cgi-bin/cgi-bin/login.cgi ./cgi-bin/cgi-bin/serial.cgi ./cgi-bin/cgi-bin/wifi.cgi ./cgi-bin/cgi-bin/wlan.cgi ./cgi-bin/cgi-bin/mswitch.cgi
	-rm -rf ./bin/www/* ./bin/s2wifi
	@echo "Clean Done"

