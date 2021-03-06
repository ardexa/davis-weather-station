
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

#ifndef CONFIGS_HPP_INCLUDED
#define CONFIGS_HPP_INCLUDED

#define BAUDRATE B19200    /* Baud rate for the davis weather station is 19200 by default */
#define TIMEOUT 150        /* In deciseconds...so 150 = 15 seconds */
#define DATESIZE 20
#define DEFAULT_LOG_DIRECTORY "/opt/ardexa/davis/" /* Default logging directory */
#define ERROR_VALUE_FLOAT -9999.9   /* If an error is encountered, this number will be written to the log */
#define BUFSIZE 255
#define MINCHARS 200
#define LOOP_LENGTH 100
#define MS_TO_KMH 3.6

#define HEADER_LINE "# DateTime,Inside Temperature (celsius),Outside Temperature (celsius),Inside Humidity (percent),Outside Humidity (percent),Wind Speed (m/s),Wind Direction (degrees),Barometer (hectopascals),Solar Radiation (w/m^2),UV Index,Rain (mm),Console Battery (volts), Soil Temperature 1 (C), Soil Moisture 1 (centibar), Soil Temperature 2 (C), Soil Moisture 2 (centibar), Soil Temperature 3 (C), Soil Moisture 3 (centibar), Soil Temperature 4 (C), Soil Moisture 4 (centibar)"


#define HEADER_LINE_KMH "# DateTime,Inside Temperature (celsius),Outside Temperature (celsius),Inside Humidity (percent),Outside Humidity (percent),Wind Speed (km/h),Wind Direction (degrees),Barometer (hectopascals),Solar Radiation (w/m^2),UV Index,Rain (mm),Console Battery (volts), Soil Temperature 1 (C), Soil Moisture 1 (centibar), Soil Temperature 2 (C), Soil Moisture 2 (centibar), Soil Temperature 3 (C), Soil Moisture 3 (centibar), Soil Temperature 4 (C), Soil Moisture 4 (centibar)"



/* Davis weather station Vendor ID: 10c4 and Product ID: ea61 ... or .... Bus 001 Device 009: ID 10c4:ea60
   Executing "lsusb" should see the line "...Bus 002 Device 002: ID 10c4:ea61 Cygnal Integrated Products, Inc...." 
   Bus 001 Device 009: ID 10c4:ea60 Cygnal Integrated Products, Inc. CP2102/CP2109 UART Bridge Controller [CP210x family] */
#define DAVIS_USBSERIAL_VENDOR "10c4"
#define DAVIS_USBSERIAL_PRODUCT "ea61"
#define DAVIS_USBSERIAL_PRODUCT2 "ea60"
#define PID_FILE "/run/ardexa-davis.pid"
#define DEFAULT_DEBUG_VALUE 0


typedef struct davis_data_s {
    float inside_humidity;
    float outside_humidity;
    float wind_speed;
    float barometer;
    float outside_temperature;
    float inside_temperature;
    float rain;
    float UV;
    float solar_radiation;
    float console_battery;
    float wind_direction;
    float soil_temp1;
    float soil_moist1;
    float soil_temp2;
    float soil_moist2;
    float soil_temp3;
    float soil_moist3;
    float soil_temp4;
    float soil_moist4;
} davis_data_t;

#endif /* CONFIGS_HPP_INCLUDED */
