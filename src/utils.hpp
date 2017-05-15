/* 
* Copyright (c) 2013-2017 Ardexa Pty Ltd
*
* This code is licensed under the MIT License (MIT).
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*
*/


#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <string>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <algorithm>   
#include <ctime>
#include "configs.hpp"
#include <stdio.h>
#include <libudev.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iomanip>

extern int g_debug;

using namespace std;

int log_line(string directory, string filename, string line, string header, bool log_to_latest);
string get_current_date();
string get_current_datetime();
string extract_results(char *input_buffer, int chars_received, bool debug);
string write_result_string(davis_data_t davis_data);
string find_usb_device(bool debug);
bool create_directory(string directory);
bool check_pid_file();
void remove_pid_file();
bool check_root();
bool check_directory(string directory);
bool check_file(string file);
bool convert_long(string incoming, long *outgoing);
string trim_whitespace(string raw_string);

#endif /* UTILS_HPP_INCLUDED */
