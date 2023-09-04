#include <iostream>
#include <windows.h>

#include "adapters/ethernet.hpp"
#include "adapters/wifi.hpp"
#include "adapters/detectInterface.hpp"

#include "WlanException.hpp"


void CustomNotification(const std::string& message, bool isError = false) {
    MessageBoxA(nullptr, message.c_str(), "Internet Disabler", MB_OK | (isError ? MB_ICONERROR : MB_ICONINFORMATION));
}


int main() {
    InterfaceType interfaceType = DetectInterface::Detect();
    switch (interfaceType) {
        case ETHERNET:
            try {
                Ethernet::DisableEthernetConnection();
                CustomNotification("Ethernet disabled");
            } catch (WlanException& e) {
                CustomNotification(e.what(), true);
            }
            break;
        case WIFI:
            try {
                GUID guid = Wifi::GetCurrentWifiGuid();
                Wifi::DisableWifiConnection(guid);
                CustomNotification("Wifi disabled");
            } catch (WlanException& e) {
                CustomNotification(e.what(), true);
            }
            break;
        default:
            CustomNotification("Unknown interface type", true);
            break;
    }
    return 0;
}
