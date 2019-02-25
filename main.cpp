#include <iostream>
#include <string>

#include "serial.h"
#include "messages.h"

using namespace std;

static uint64_t GetTickCountMs();

uint16_t getCheckSumm16b(char *pcBlock, uint16_t len);

int main()
{
    cout << "Program started" << endl;

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
    port.flush();
    port << output;

    cout << " Waiting for the response... " << endl;
    ResponseMessage response;
    uint64_t lasttick = GetTickCountMs();
    while(port.bytesAvailable() < response.length) {
        if(GetTickCountMs() - lasttick > 5000) {
            cout << " Timeout! " << endl;
            port.flush();
            port << output;
            cout << " Waiting for the response... " << endl;
            lasttick = GetTickCountMs();
        }
    }

    vector<uint8_t> answer;
    answer.clear();
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

/* CRC16-CCITT algorithm */
uint16_t getCheckSumm16b(char *pcBlock, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    uint8_t i;
    len = len-2;

    while (len--) {
        crc ^= *pcBlock++ << 8;

        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}
