/* Copyright (c) 2013-2015 Ardexa Pty Ltd. All rights reserved.
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

#include "arguments.hpp"

using namespace std;

/* Constructor for the arguments class */
arguments::arguments()
{
    /* Initialize the private members */
    this->debug = false;
    this->directory = DIRECTORY;
    this->filename_date = false;

    /* Usage string */
    this->usage_string = "Usage: read_davis [-t device] [-d directory] [-f] [-e]\n";
}

/* This method is to initialize the member variables based on the command line arguments */
int arguments::initialize(int argc, char * argv[])
{
    int opt;

    /**
     * -t <device> (optional) name of the /dev/ device
     * -d <directory> (optional) name of the logging directory
     * -e (optional) if specified, debug will be turned on
     * -f (optional) if specified, filename will include the date
     */
    while ((opt = getopt(argc, argv, "t:d:ef")) != -1) {
        switch (opt) {
            case 't':
                /* The device will be verified later */
                this->device = optarg;
                break;
            case 'd':
                /* There is no verifying the directory. If it doesn't exist, it will be created later */
                this->directory = optarg;
                break;
            case 'e':
                this->debug = true;
                break;
            case 'f':
                this->filename_date = true;
                break;
            default:
                this->usage();
                return 1;
        }
    }

    return 0;
}

/* Print usage string */
void arguments::usage()
{
    cout << usage_string;
}

/* Get debug value */
bool arguments::get_debug()
{
    return this->debug;
}

/* Get whether the date should be included in the filename */
bool arguments::get_filename_date()
{
    return this->filename_date;
}

/* Get the logging directory */
string arguments::get_directory()
{
    return this->directory;
}

/* Get the device name */
string arguments::get_device()
{
    return this->device;
}
