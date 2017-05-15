/* Copyright (c) 2013-2017 Ardexa Pty Ltd. All rights reserved.
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
 */

#ifndef ARGUMENTS_HPP_INCLUDED
#define ARGUMENTS_HPP_INCLUDED

#include <string>
#include "arguments.hpp"
#include "utils.hpp"
#include "configs.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

/* This class is to call in the command line arguments */
class arguments
{
    public:
        /* methods are public */
        arguments();
        int initialize(int argc, char * argv[]);
        void usage();
        bool get_debug();
        string get_log_directory();
        string get_device();

    private:
        /* members are private */
        bool debug;
        string log_directory;
        string device;
        string usage_string;
};

#endif /* ARGUMENTS_HPP_INCLUDED */
