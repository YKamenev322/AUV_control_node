#include <iostream>
#include <string>

#include "serial.h"
#include "messages.h"

using namespace std;

int main()
{
    cout << "Program started" << endl;
    //cout << "Enter name of the device file:" << endl;

    string file = "/dev/ttyUSB1";
    Serial port(file, 9600, 8, PARITY_NONE, 1);

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

    vector<uint8_t> answer;
    cout << " Waiting for the response... " << endl;
    ResponseMessage response;
    while(port.bytesAvailable() < response.length) {

    }

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
