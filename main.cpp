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

#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include "utils.hpp"
#include "configs.hpp"
#include "arguments.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    int result = 0;
    struct termios newtio;
    char buffer[BUFSIZE];
    string line, device;

    /* This class object defines the initial configuration parameters */
    arguments arguments_list;
    result = arguments_list.initialize(argc, argv);
    if (result != 0) {
        return 1;
    }

    device = arguments_list.get_device();
    /* If the 'device' is empty, it means it must be searched since the user has not provided a device. If the USB device cannot be found, then exit */
    if (device.empty()) {
        device = find_usb_device(arguments_list.get_debug());
        if (device.empty()) {
            return 2;
        }
    }

    /* Open the Davis weather station device for read and write. Writing is needed to send commands to the Davis,
       that will then return the required information */
    int modem_filedesc = open(device.c_str(),  O_RDWR | O_NOCTTY );
    if (modem_filedesc < 0) {
        perror(device.c_str());
        cout << "Error opening Davis serial line" << endl;
        return 3;
    }

    memset(&newtio, '\0', sizeof(newtio));
    /* These are the modem control signals:
    CS8 = 8 data bits
    CLOCAL - Ignore modem control lines.
    CREAD - Enable receiver.
    ICANON - Input is made available line by line. ***DON'T WANT THIS***.... so set newtio.c_lflag = 0;
    CRTSCTS - (not in POSIX) Enable RTS/CTS (hardware) flow control.
    IGNPAR - Ignore framing errors and parity errors.  */

    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;
    newtio.c_cc[VTIME] = TIMEOUT;
    /* If (say) 97 chars are received, this is not a complete loop
    If 99 chars are received (with an ACK at the start (so 100 chars)
    Then wait for a maximum of 200 chars before returning
    Which means about 3 LOOPS maximum, before it returns */
    newtio.c_cc[VMIN] = MINCHARS;
    /* clean the modem line and activate the settings for the port */
    tcflush(modem_filedesc, TCIFLUSH);
    tcsetattr(modem_filedesc, TCSANOW, &newtio);

    /* This will wake up the Davis console and get it to send 30 LPS (over a 50 sec or so time)
    The LF (linefeeds) are to wakeup the Davis */
    for (int i = 0; i < 2; i++) {
        result = write(modem_filedesc, "\n", 1);
        if (arguments_list.get_debug()) cout << "LF WRITTEN" << endl;
        sleep(1);
    }
    result = write(modem_filedesc, "LPS 0 30\r", 9);
    if (arguments_list.get_debug()) cout << "LPS 0 30 WRITTEN" << endl;

    /* Only 2 goes at the loop */
    for (int loop = 2; loop > 0; loop--) {
        memset(buffer, '\0', sizeof(buffer));
        result = read(modem_filedesc, buffer, sizeof(buffer));
        /* 100 is the length of a LOOP command (99 chars) plus an ACK */
        if (result > LOOP_LENGTH) {
            line = extract_results(buffer, result, arguments_list.get_debug());
            break;
        }
        /* This else is just for debugging */
        else {
            if (arguments_list.get_debug()) cout << "Too few characters received or read timeout. Chars read: " << result << endl;
        }
    }

    string filename;
    if (arguments_list.get_filename_date()) {
        string current_date = get_current_date();
        filename = "davis_" + current_date + ".log";
    }
    else {
        filename = "davis.log";
    }
    /* Write the line to the log file */
    log_line(arguments_list.get_debug(), arguments_list.get_directory(), filename, line);

    /* This is to cancel any remaining LPS events */
    result = write(modem_filedesc, "\r", 1);
    if (arguments_list.get_debug()) cout << "CR WRITTEN" << endl;
    close(modem_filedesc);

    return 0;
}
