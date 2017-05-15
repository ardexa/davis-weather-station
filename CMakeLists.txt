# Copyright (c) 2013-2017 Ardexa Pty Ltd. All rights reserved.
#
# This code is licensed under the MIT License (MIT).
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
#
# CMAKE file for the Ardexa plugin to read the Davis Weather station
#
#
# Note: Makesure the udev library is installed.... via: sudo apt-get install libudev-dev
#

cmake_minimum_required(VERSION 2.6)

project(ardexa-davis)

set(CMAKE_CXX_FLAGS "-Wall -fexceptions -std=c++11")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES /usr/local/lib ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})


# Source files
set(ARDEXA_DAVIS_SRC   src/main.cpp src/configs.hpp src/arguments.cpp src/arguments.hpp src/utils.cpp src/utils.hpp)

add_executable(ardexa-davis ${ARDEXA_DAVIS_SRC})
target_link_libraries(ardexa-davis udev )

# add the install targets
install (TARGETS ardexa-davis DESTINATION /usr/local/bin)
