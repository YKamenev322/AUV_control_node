#include <iostream>
#include <string>

#include "serial.h"

using namespace std;

int main()
{
    cout << "Program started" << endl;
    cout << "Enter name of the device file:" << endl;

    string file;
    cin >> file;

    Serial port(file, 9600, 8, PARITY_NONE, 1);

    string input;
    cout << "Enter message to the port \" " << file << " \" " << endl;
    cin >> input;
    cout << "Sending message: " << input << " // Size: " << input.size() << endl;
    port << input;

    string output;
    cout << " Waiting for the response... " << endl;
    while(!port.bytesAvailable()) {

    }

    cout << " Got response! Bytes: " << port.bytesAvailable() << endl;
    string answer;
    port >> output;
    cout << output << endl;
    return 0;
}
