//
// Created by TaxMachine on 2023-09-03.
//

#ifndef INTERNET_DISABLER_WIFI_HPP
#define INTERNET_DISABLER_WIFI_HPP

#include <windows.h>

namespace Wifi {
    GUID GetCurrentWifiGuid();
    bool DisableWifiConnection(const GUID& guid);
}

#endif //INTERNET_DISABLER_WIFI_HPP
