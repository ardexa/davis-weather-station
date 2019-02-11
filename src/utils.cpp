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

#include "utils.hpp"

/* Open the file where the log entry will be written, and write the line to it 
   When using this function, make sure 'line' and 'header' have a newline at end 
   If the 'rotate' is true, it will move thew old file and file instead of appending 
	If the 'log_to_latest' it will also log a line to 'latest.csv' in 'directory' 
*/
int log_line(string directory, string filename, string line, string header, bool log_to_latest)
{
	struct stat st_directory;
	string fullpath;
	bool write_header = false;
	/* if the log directory does not exist, or the log file does not exist, then declare a 'rotation'.
	   This is where the 'latest.csv' file is renamed and a new one created */
	bool rotate = false;

	/* Add an ending '/' to the directory path, if it doesn't exist */
	if (*directory.rbegin() != '/') {
		directory += "/";
	}

	/* Check and create the directory if necessary */
	if (stat(directory.c_str(), &st_directory) == -1) {
		if (g_debug > 0) cout << "Directory doesn't exist. Creating it: " << directory.c_str() << endl;
		rotate = true;
		bool result = create_directory(directory);
		if (!result) {
			return 2;
		}
	}

	fullpath = directory + filename;
	if (g_debug > 1) cout << "Full filename: " << fullpath << endl;

	/* Check the full path. If it doesn't exist, the header line will need to be written 
		If the file DOES exist AND if a rotation is called, then rename it and annotate a header is required 
      And the rotate will need to be set to true 
   */
	if (stat(fullpath.c_str(), &st_directory) == -1) {
		if (g_debug > 1) cout << "Fullpath doesn't exist. Path: " << fullpath.c_str() << endl;
		write_header = true;
		rotate = true;
	}

	/* Open it for appending data only */
	ofstream writer(fullpath.c_str(), ios::app);
	if(!writer) {
		if (g_debug > 0) cout << "Cannot open logging file: " << fullpath << endl;
		return 2;
	}
	if (write_header) {
		writer << header << endl;
	}

	writer << line << endl;
 	writer.close();

	write_header = false;
	/* If 'log_to_latest' is set, then write the line to this file as well */
	if (log_to_latest) {
		/* if file exists and rotate is declared, rename it and create a new one */
		fullpath = directory + "latest.csv";
		if (rotate) {
			string newpath = directory + "latest.csv.OLD";
			rename(fullpath.c_str(), newpath.c_str());
			write_header = true;
		}

		/* Open it for appending data only */
		ofstream latest(fullpath.c_str(), ios::app);
		if(!latest) {
			if (g_debug > 0) cout << "Cannot open logging file: " << fullpath << endl;
			return 3;
		}
		if (write_header) {
			latest << header << endl;
		}
		latest << line << endl;

		/* close it */
		latest.close();
	}	

	return 0;
}

/* Create a directory, including all parent paths if they don't exist */
bool create_directory(string directory)
{
	string delimiter = "/";
	size_t pos = 0, start = 0;
	int count = 0;
	string temp = "";

	/* Add a trailing '/' */
	directory = directory + '/';
	while ((pos = directory.find(delimiter, start)) != string::npos) {
		if (count == 0) {
			if (pos != 0) {
				cout << "Directory needs to start with " << delimiter << endl;
				return false;
			}
		}
		else {
			temp = directory.substr(0, pos); 
			if (check_directory(temp)) {
				if (g_debug > 1) cout << "The dir: " << temp << " exists." << endl;

			}
			else {
				if (g_debug > 0) cout << "Creating the dir: " << temp << endl;
				if (mkdir(temp.c_str(), 0744) != 0) {
					cout << "Could not create the directory: " << temp << endl;
					return false;
				}
			}
		}
		count++;
		/* start at one past the las pos */
		start = pos + 1; 
	} 

	return true;
}

/* Check if a direetory exists */
bool check_directory(string directory)
{
    struct stat sb;

    if (stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return true;
    }
    else {
        return false;
    }

}

/* check if a file exists */
bool check_file(string file)
{
    struct stat sb;

    if (stat(file.c_str(), &sb) == 0 && S_ISREG(sb.st_mode)) {
        return true;
    }
    else {
        return false;
    }

}

/* Convert a string to a long */
bool convert_long(string incoming, long *outgoing)
{
    size_t idx;
    bool stol_success = true;
    *outgoing = 0;

    try {
        *outgoing = stol(incoming,&idx,10);
    }
    catch ( const std::exception& e ) {
        stol_success = false;
    }


    if (stol_success){
        if (idx == incoming.length()) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }

}

/* trim whitespace from the start and end of a string */
string trim_whitespace(string raw_string) {

    raw_string.erase(raw_string.begin(), find_if(raw_string.begin(), raw_string.end(), not1(ptr_fun<int, int>(isspace))));
    raw_string.erase(find_if(raw_string.rbegin(), raw_string.rend(), not1(ptr_fun<int, int>(isspace))).base(), raw_string.end());

    return raw_string;
}


/* Returns the current date as a string in the format "2017-01-30" */
string get_current_date()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[DATESIZE];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);

    string date(buffer);
    return date;
}

/* Returns the current time as a string, in the format "2017-01-30T15:30:45" */
string get_current_datetime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[DATESIZE];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    /* This includes the time zone at the end of the time */
    strftime(buffer, DATESIZE, "%H:%M:%S%z", timeinfo);

    string time(buffer);

    string datetime = get_current_date() + "T" + time;

    return datetime;
}

/*  This function checks for the existence of a PID file. If one is
    found, it checks if the process is alive and exits if so. Otherwise,
    it will (re)write the PID file. */
bool check_pid_file()
{
    /* Check to see if there is a pid file */
    bool pid_file_exists = true;
    struct stat pid_status;
    string pid_file = PID_FILE;
    string pid_str, pid_raw;
    long pid_long;

    /* if the file exists, then read in the pid */
    if (stat(pid_file.c_str(), &pid_status) == 0) {
        fstream read_pid(pid_file, ios::in);
        read_pid >> pid_raw;
        read_pid.close();
        /* trim leading and trailing whitespace */
        pid_str = trim_whitespace(pid_raw);

        if (!(convert_long(pid_str, &pid_long))) {
            cout << "The ardexa-sma application appears to be already running..." << endl;
            return false;
        }
    }
    else {
        /* Flag that the pid file doesn't exist */
        pid_file_exists = false;
    }

    /* Check that the file exists and pid is alive. Use a sig of 0 */
    if ((pid_file_exists) && (kill((pid_t) pid_long, 0) == 0)) {
        cout << "The ardexa-sma application appears to be already running..." << endl;
        return false;
    }

    /*  If routine gets to here, then the file exists and the pid is dead, or the pid file doesn't exist.
        Either condition is treated the same */

    ofstream write_pid(pid_file, ios::trunc);
    write_pid << getpid() << endl;
    write_pid.close();

    return true;
}

/* remove a PID file */
void remove_pid_file()
{
    struct stat pid_status;
    string pid_file = PID_FILE;

    /* if the file exists, then remove it */
    if (stat(pid_file.c_str(), &pid_status) == 0) {
        remove(pid_file.c_str());
    }
}


bool check_root()
{
	uid_t uid=getuid();
	if (uid == 0) {
		return true;
	} 
	else {
		return false;
	}
}

/* This function extracts the results, and returns a string */
string extract_results(char *input_buffer, int chars_received, bool debug, bool wdspd_kmh, float barocal, bool winddir_180)
{
    davis_data_t davis_data;

    /* Set all the Davis_data members to error values */
    davis_data.inside_humidity = ERROR_VALUE_FLOAT;
    davis_data.outside_humidity = ERROR_VALUE_FLOAT;
    davis_data.wind_speed = ERROR_VALUE_FLOAT;
    davis_data.barometer = ERROR_VALUE_FLOAT;
    davis_data.outside_temperature = ERROR_VALUE_FLOAT;
    davis_data.inside_temperature = ERROR_VALUE_FLOAT;
    davis_data.rain = ERROR_VALUE_FLOAT;
    davis_data.UV = ERROR_VALUE_FLOAT;
    davis_data.solar_radiation = ERROR_VALUE_FLOAT;
    davis_data.console_battery = ERROR_VALUE_FLOAT;
    davis_data.wind_direction = ERROR_VALUE_FLOAT;
    davis_data.soil_temp1 = ERROR_VALUE_FLOAT;
    davis_data.soil_moist1 = ERROR_VALUE_FLOAT;
    davis_data.soil_temp2 = ERROR_VALUE_FLOAT;
    davis_data.soil_moist2 = ERROR_VALUE_FLOAT;
    davis_data.soil_temp3 = ERROR_VALUE_FLOAT;
    davis_data.soil_moist3 = ERROR_VALUE_FLOAT;
    davis_data.soil_temp4 = ERROR_VALUE_FLOAT;
    davis_data.soil_moist4 = ERROR_VALUE_FLOAT;

    if (debug) cout << "Chars received = " << chars_received << endl;
    for (int i = 0; i < chars_received; i++) {
        /* The start of a valid line is 'LOO' */
        /* Check that there at least 88 chars after 'i' before proceeding */
        if ((input_buffer[i] == 'L') && (input_buffer[i+1] == 'O') && (input_buffer[i+2] == 'O') && (chars_received - i >= 89)) {
            if (debug) cout << "Found LOO at offset: " << i << " Val: " << input_buffer[i] << input_buffer[i+1] << input_buffer[i+2] << endl;

            /* Call in the data. For sanity checking this is the plan:
            If any of the values below are DUD, I don't want to invalidate the whole line.
            So any parameters below which *appear* to be obviously invalid, will be replaced with the value ERROR_VALUE_FLOAT
            An error condition will then flagged which will then be sent to the log */
            if (debug) cout << "Raw barometer offset 7 and 8: " << input_buffer[i+7] << input_buffer[i+8] << endl;
            /* convert inches of mercury to hectopascals */
            davis_data.barometer = (float) ((input_buffer[i+8] << 8) | input_buffer[i+7])/1000 * 33.86;
            if ((davis_data.barometer < 800.0) || (davis_data.barometer > 1100.0)) {
                davis_data.barometer = ERROR_VALUE_FLOAT;
            }
            /* calibrate the barometer */
            if (debug) {
                cout << "\t Barometer uncalibrated (hectopascals): " << davis_data.barometer << endl;
            }
            davis_data.barometer = davis_data.barometer * barocal;
            if (debug) {
                cout << "\t Barometer (hectopascals): " << davis_data.barometer << endl;
                cout << "\t Baro calibration value: " << barocal << endl;
            }

            if (debug) cout << "Raw outside temp offset 12 and 13: " << input_buffer[i+12] << input_buffer[i+13] << endl;
            /* convert Fahrenheit to Celsius */
            davis_data.outside_temperature =   (( (float) ((input_buffer[i+13] << 8) | input_buffer[i+12])/10) - 32.0) * 5/9;
            if ((davis_data.outside_temperature < -80.0) || (davis_data.outside_temperature > 100.0)) {
                davis_data.outside_temperature = ERROR_VALUE_FLOAT;
            }
            if (debug) cout << "\tOutside temperature (celsius): " << davis_data.outside_temperature << endl;

            if (debug) cout << "Raw inside temp offset 9 and 10: " << input_buffer[i+9] << input_buffer[i+10] << endl;
            /* convert Fahrenheit to Celsius */
            davis_data.inside_temperature =   (((float) ((input_buffer[i+10] << 8) | input_buffer[i+9])/10) - 32.0) * 5/9;
            if ((davis_data.inside_temperature < -80.0) || (davis_data.inside_temperature > 100.0)) {
                davis_data.inside_temperature = ERROR_VALUE_FLOAT;
            }
            if (debug) cout << "\tInside temperature (Celsius): " << davis_data.inside_temperature << endl;

            if (debug) cout << "Raw wind speed offset 14: " << input_buffer[i+14] << endl;
            /* convert mph to metres/s */
            davis_data.wind_speed = (float) (input_buffer[i+14] * 0.44704);
            if ( (davis_data.wind_speed < 0.0) || (davis_data.wind_speed > 50.0)) {
                davis_data.wind_speed = ERROR_VALUE_FLOAT;
            }
            else {
                /* Convert the wind speed from m/s to km/h if requested by the user */
                if (wdspd_kmh) {
                    davis_data.wind_speed = davis_data.wind_speed * MS_TO_KMH;
                    if (debug) cout << "\tWind speed (km/h): " << davis_data.wind_speed << endl;
                }
                else {
                    if (debug) cout << "\tWind speed (m/s): " << davis_data.wind_speed << endl;
                }
            }

            if (debug) cout << "Raw wind direction offset 16 and 17: " << input_buffer[i+16] << input_buffer[i+17] << endl;
            davis_data.wind_direction = (float) ((input_buffer[i+17] << 8) | input_buffer[i+16]);

            /* Alter the wind direction by 180 degs, if required */
            if (winddir_180) {
                davis_data.wind_direction = davis_data.wind_direction + 180.0;
                if (davis_data.wind_direction > 360.0) {
                    davis_data.wind_direction = davis_data.wind_direction - 360.0;
                }
            }
            if ( (davis_data.wind_direction < 0.0) || (davis_data.wind_direction > 360.0)) {
                davis_data.wind_direction = ERROR_VALUE_FLOAT;
            }
            if (debug) cout << "\tWind direction (degs): " << davis_data.wind_direction << endl;

            if (debug) cout << "Raw outside humidity offset 33: " << input_buffer[i+33] << endl;
            davis_data.outside_humidity = (float) (input_buffer[i+11]);
            if ((davis_data.outside_humidity < 0.0) || (davis_data.outside_humidity > 100.0)) {
                davis_data.outside_humidity = ERROR_VALUE_FLOAT;
            }
            if (debug) cout << "\tOutside humidity (%): " << davis_data.outside_humidity << endl;

            if (debug) cout << "Raw inside humidity offset 11: " << input_buffer[i+11] << endl;
            davis_data.inside_humidity = (float) (input_buffer[i+33]);
            if ((davis_data.inside_humidity < 0.0) || (davis_data.inside_humidity > 100.0)) {
                davis_data.inside_humidity = ERROR_VALUE_FLOAT;
            }
            if (debug) cout << "\tInside humidity (%): " << davis_data.inside_humidity << endl;

            /* Part of the DAVIS protocol doc says UV is read directly, and another section says to divide by 10
               Correct value seems to be if the raw index is divided by 10. */
            if (debug) cout << "Raw UV offset 43: "<< input_buffer[i+43]<< endl;
            davis_data.UV = (float) (input_buffer[i+43])/10;
            if ((davis_data.UV < 0.0) || (davis_data.UV > 50.0)) {
                davis_data.UV = ERROR_VALUE_FLOAT;
            }
            if (debug) cout << "\tUV index: " << davis_data.UV << endl;

            if (debug) cout << "Raw solar radiation offset 44 and 45: " << input_buffer[i+44] << input_buffer[i+45] << endl;
            davis_data.solar_radiation =  (float) ((input_buffer[i+45] << 8) | input_buffer[i+44]);
            if ((davis_data.solar_radiation < 0.0) || (davis_data.solar_radiation > 1800.0)) {
                davis_data.solar_radiation = ERROR_VALUE_FLOAT;
            }
            if (debug) cout << "\tSolar radiation (W/m): " << davis_data.solar_radiation << endl;

            if (debug) cout << "Raw console battery voltage offset 87 and 88: " << input_buffer[i+87] << input_buffer[i+88] << endl;
            /*  Convert to volts. The formula  is directly from the Davis 'Vantage Pro ® , Vantage Pro2 TM and Vantage Vue ®
                Serial Communication Reference Manual'
                Voltage = ((Data * 300)/512)/100.0 */
            davis_data.console_battery =  (((float) ((input_buffer[i+88] << 8) | input_buffer[i+87])*300)/512)/100.0;
            if ((davis_data.console_battery < -10.0) || (davis_data.console_battery > 50.0)) {
                davis_data.console_battery = ERROR_VALUE_FLOAT;
            }
            if (debug) cout << "\tConsole battery (volts): " << davis_data.console_battery << endl;

            if (debug) cout << "Raw rain offset 46 and 47: " << input_buffer[i+41] << input_buffer[i+42] << endl;
            /* Nothing in the documentation about reading this figure, but to get it to mm/hr, appears the figure needs to be divided by 4 */
            davis_data.rain = ((float) ((input_buffer[i+47] << 8) | input_buffer[i+46])*0.25);
            if ((davis_data.rain < 0.0) || (davis_data.rain > 300.0)) {
                davis_data.rain = ERROR_VALUE_FLOAT;
            }
            if (debug) cout << "\tRain (mm/hr): " << davis_data.rain << endl;

            /* Soil temperature 1 reading...NB A special Davis device is required to read this */
            if (debug) cout << "Soil temperature 1 offset 25: " << input_buffer[i+25] << endl;
            davis_data.soil_temp1 = (((float) (input_buffer[i+25]) + -90) - 32.0) * 5/9;
            if (debug) cout << "\tSoil temperature 1 (Celsius): " << davis_data.soil_temp1 << endl;

            /* Soil Moisture 1 */
            if (debug) cout << "Soil moisture 1 offset 62: " << input_buffer[i+62] << endl;
            davis_data.soil_moist1 = (float) (input_buffer[i+62]);
            if (debug) cout << "\tSoil moisture 1 (Centibar): " << davis_data.soil_moist1 << endl;

            /* Soil temperature 2 reading...NB A special Davis device is required to read this */
            if (debug) cout << "Soil temperature 2 offset 26: " << input_buffer[i+26] << endl;
            davis_data.soil_temp2 = (((float) (input_buffer[i+26]) + -90) - 32.0) * 5/9;
            if (debug) cout << "\tSoil temperature 2 (Celsius): " << davis_data.soil_temp2 << endl;

            /* Soil Moisture 2 */
            if (debug) cout << "Soil moisture 2 offset 63: " << input_buffer[i+63] << endl;
            davis_data.soil_moist2 = (float) (input_buffer[i+63]);
            if (debug) cout << "\tSoil moisture 2 (Centibar): " << davis_data.soil_moist2 << endl;

            /* Soil temperature 3 reading...NB A special Davis device is required to read this */
            if (debug) cout << "Soil temperature 3 offset 27: " << input_buffer[i+27] << endl;
            davis_data.soil_temp3 = (((float) (input_buffer[i+27]) + -90) - 32.0) * 5/9;
            if (debug) cout << "\tSoil temperature 3 (Celsius): " << davis_data.soil_temp3 << endl;

            /* Soil Moisture 3 */
            if (debug) cout << "Soil moisture 3 offset 64: " << input_buffer[i+64] << endl;
            davis_data.soil_moist3 = (float) (input_buffer[i+64]);
            if (debug) cout << "\tSoil moisture 3 (Centibar): " << davis_data.soil_moist3 << endl;

            /* Soil temperature 4 reading...NB A special Davis device is required to read this */
            if (debug) cout << "Soil temperature 4 offset 28: " << input_buffer[i+28] << endl;
            davis_data.soil_temp4 = (((float) (input_buffer[i+28]) + -90) - 32.0) * 5/9;
            if (debug) cout << "\tSoil temperature 4 (Celsius): " << davis_data.soil_temp3 << endl;

            /* Soil Moisture 4 */
            if (debug) cout << "Soil moisture 4 offset 65: " << input_buffer[i+65] << endl;
            davis_data.soil_moist4 = (float) (input_buffer[i+65]);
            if (debug) cout << "\tSoil moisture 4 (Centibar): " << davis_data.soil_moist4 << endl;

            /* The data has been read, exit the for loop */
            break;
        }
    }

    /* Send the struct to the write function */
    return write_result_string(davis_data);
}

/* This function writes a string based on the davis_data struct */
string write_result_string(davis_data_t davis_data)
{
    stringstream stream;

    /* Get the current date-time */
    string datetime = get_current_datetime();

    /* clear the stringstream and make the string to create the CSV entry */
    stream << datetime;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.inside_temperature;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.outside_temperature;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.inside_humidity;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.outside_humidity;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.wind_speed;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.wind_direction;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.barometer;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.solar_radiation;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.UV;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.rain;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.console_battery;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.soil_temp1;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.soil_moist1;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.soil_temp2;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.soil_moist2;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.soil_temp3;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.soil_moist3;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.soil_temp4;
    stream << ",";
    stream << fixed << setprecision(2) << davis_data.soil_moist4;
    return stream.str();
}


/* This function will search the USB devices to find the one that corresponds to a Davis weather station */
string find_usb_device(bool debug)
{
    struct udev *udev;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    struct udev_device *dev;
    const char *vendor_id, *product_id, *serial, *path, *full_syspath, *device_found;
    bool found = false;

    /* Create the udev object */
    udev = udev_new();
    if (!udev) {
        cout << "Can't create udev data type" << endl;
        return string();
    }

    /* Create a list of the devices in the 'tty' subsystem. */
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "tty");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    /* For loop that iterates through each tty device found */
    udev_list_entry_foreach(dev_list_entry, devices) {
        /* Get the file name of the /sys entry for the device */
        full_syspath = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, full_syspath);
        /* get the path to the device */
        path = udev_device_get_devnode(dev);

        /* Retrieve the USB device information */
        dev = udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
        if (!dev) {
            udev_device_unref(dev);
            continue;
        }
        /* Davis weather station Vendor ID: 10c4 and Product ID: ea61
           Executing "lsusb" should see the line "...Bus 002 Device 002: ID 10c4:ea61 Cygnal Integrated Products, Inc...." */
        vendor_id = udev_device_get_sysattr_value(dev, "idVendor");
        product_id = udev_device_get_sysattr_value(dev, "idProduct");
        serial = udev_device_get_sysattr_value(dev, "serial");
        if ((strcmp(vendor_id, DAVIS_USBSERIAL_VENDOR) == 0) and (strcmp(product_id, DAVIS_USBSERIAL_PRODUCT) == 0)) {
            if (found) {
                cout << "Duplicate \"Cygnal Integrated Products\" USB found. Check by running \'lsusb\'. If necessary specify the /dev device manually" << endl;
                return "";
            }
            found = true;
            device_found = path;
            if (debug) {
                cout << "Device node: " << path << endl;
                cout << "\tFull Path: " << full_syspath << endl;
                cout << "\tVendor ID: " << vendor_id << endl;
                cout << "\tProduct ID: " << product_id << endl;
                cout << "\tSerial: " << serial << endl;
            }
        }
        udev_device_unref(dev);
    }

    udev_enumerate_unref(enumerate);
    udev_unref(udev);

    string device(device_found, strlen(device_found));

    if (found) {
        if (debug) cout << "Device found: " << device << endl;
        return device;
    }
    else {
        cout << "Davis weather station USB Serial device not found. Check by running \'lsusb\'. If necessary specify the /dev device manually" << endl;
        return string();
    }
}
