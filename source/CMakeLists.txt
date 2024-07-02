# Copyright (C) 2013 Max Oberberger <max@oberbergers.de>
#
# Last modified: 2013 February 23, 20:05:02 by max
#
# This file is part of ba-hla.
# 
# ba-hla is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# ba-hla is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ba-hla.  If not, see <http://www.gnu.org/licenses/>.
#
cmake_minimum_required(VERSION 3.6)
project(ba-hla)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## enable debugging
set(CMAKE_BUILD_TYPE Debug)

include_directories(/home/yonchicy/OpenRTI/include/rti1516)
#add_library(ba-hla-lib ExampleFedAmb.cpp ExampleCPPFederate.cpp)
#add_executable(ba-hla main.cpp)
#add_library(ba-hla-lib ExampleFedAmb.cpp ExampleCPPFederate.cpp)
file(GLOB SRC_FILES "*.cpp")
add_executable(ba-hla ${SRC_FILES})
target_link_libraries(ba-hla  /home/yonchicy/OpenRTI/build/lib64/librti1516.so /home/yonchicy/OpenRTI/build/lib64/libfedtime1516.so )

install(TARGETS ba-hla DESTINATION bin)
