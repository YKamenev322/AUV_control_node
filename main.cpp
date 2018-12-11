#include <iostream>
#include <string>

#include "serial.h"
#include "messages.h"

using namespace std;

static uint64_t GetTickCountMs();

int main()
{
    cout << "Program started" << endl;
    //cout << "Enter name of the device file:" << endl;

    string file = "/dev/ttyUSB0";
    Serial port(file, 57600, 8, PARITY_NONE, 1);

    if(!port.isOpened()) {
        cout << "Failed to open the port!" << endl;
        //return 0;
    }

    RequestMessage request;
    request.march = 666;
    request.depth = 999;
    vector<uint8_t> output = request.formVector();

    cout << "Sending request message // Size: " << output.size() << endl;
    port << output;

    cout << " Waiting for the response... " << endl;
    ResponseMessage response;
    int counter = 0;
    uint64_t lasttick = GetTickCountMs();
    while(port.bytesAvailable() < response.length) {
        if(GetTickCountMs() - lasttick == 100) {
            port << output;
            cout << " Waiting for the response... " << counter << endl;
            counter++;
            lasttick = GetTickCountMs();
        }
    }

    vector<uint8_t> answer;
    cout << " Got response! Bytes: " << port.bytesAvailable() << endl;
    port >> answer;
    bool correct = response.parseVector(answer);

    if(correct) {
        cout << " Response correct! " << endl;
    }
    else {
        cout << " Response is broken =( " << endl;
    }

    int wait=0;
    cin >> wait;
    return 0;
}

/// Returns the number of ticks since an undefined time (usually system startup).
static uint64_t GetTickCountMs()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<uint64_t>(ts.tv_nsec / 1000000) + (static_cast<uint64_t>(ts.tv_sec) * 1000ull);
}
