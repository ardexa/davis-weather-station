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

#include "arguments.hpp"

using namespace std;

/* Constructor for the arguments class */
arguments::arguments()
{
    /* Initialize members */
    this->debug = DEFAULT_DEBUG_VALUE;
    this->log_directory = DEFAULT_LOG_DIRECTORY;
    this->barocal = 1.0;
    this->wdspd_kmh =  false;
    this->winddir_180 = false;

    /* Usage string */
    this->usage_string = "Usage: ardexa-davis [-t device] [-d directory] [-e] [-w] [-b barocal] \n";
}

/* This method is to initialize the member variables based on the command line arguments */
int arguments::initialize(int argc, char * argv[])
{
	int opt;
	bool ret_error = false;
    string barocal_raw = "";

    /**
     * -t <device> (optional) name of the /dev/ device
     * -d <directory> (optional) name of the logging directory
     * -e (optional) if specified, debug will be turned on
     * -w (optional) if specified, wind speed is in km/h, not m/s
     * -z (optional) if specified, wind direction will be shifted 180 degs (to cater for the anemometer arm pointing south)
     * -b (optional) if specified, will calibrate the barometer using this number as a multiplication to the raw value
     */
    while ((opt = getopt(argc, argv, "t:d:efb:wz")) != -1) {
        switch (opt) {
            case 't':
                /* The device will be verified later */
                this->device = optarg;
                break;
            case 'd':
                /* If it doesn't exist, it will be created later */
                this->log_directory = optarg;
                break;
            case 'b':
                barocal_raw = optarg;
                break;
            case 'w':
                this->wdspd_kmh =  true;
                break;
            case 'z':
                this->winddir_180 =  true;
                break;
            case 'e':
                this->debug = true;
                break;
            default:
                this->usage();
                return 1;
        }
    }


    // Convert "barocal_raw" to a float
    size_t idx;
    try {
        this->barocal = stof(barocal_raw, &idx);
    }
    catch ( const std::exception& e ) {
        cout << "Could not convert the baro calibration value to a float: " << barocal_raw << endl;
        ret_error = true;
    }

	if (not create_directory(this->log_directory)) {
		cout << "Could not create the logging directory: " << this->log_directory << endl;
		ret_error = true;
	}


	/* If any errors, then return as an error */
	if (ret_error) {
		this->usage();
		return 1;
	}
	else {
		return 0;
	}
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

/* Get the logging directory */
string arguments::get_log_directory()
{
    return this->log_directory;
}

/* Get the device name */
string arguments::get_device()
{
    return this->device;
}
