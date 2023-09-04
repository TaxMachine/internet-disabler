//
// Created by TaxMachine on 2023-09-03.
//

#include "wifi.hpp"

#include <wlanapi.h>
#include <string>

#include "../WlanException.hpp"

#pragma comment(lib, "wlanapi.lib")


GUID Wifi::GetCurrentWifiGuid() {
    HANDLE hClient = nullptr;
    DWORD dwMaxClient = 2;
    DWORD dwCurVersion = 0;
    DWORD dwResult;

    dwResult = WlanOpenHandle(dwMaxClient, nullptr, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS)
    {
        std::string error = "WlanOpenHandle failed with error: " + std::to_string(dwResult);
        throw WlanException(error.c_str());
    }

    PWLAN_INTERFACE_INFO_LIST pIfList = nullptr;
    dwResult = WlanEnumInterfaces(hClient, nullptr, &pIfList);
    if (dwResult != ERROR_SUCCESS)
    {
        std::string error = "WlanEnumInterfaces failed with error: " + std::to_string(dwResult);
        throw WlanException(error.c_str());
    }

    GUID networkGuid = GUID();
    if (pIfList->dwNumberOfItems > 0)
    {
        PWLAN_INTERFACE_INFO pIfInfo = &(pIfList->InterfaceInfo[0]);
        networkGuid = pIfInfo->InterfaceGuid;
    }

    WlanFreeMemory(pIfList);
    WlanCloseHandle(hClient, nullptr);

    return networkGuid;
}


bool Wifi::DisableWifiConnection(const GUID& guid) {
    HANDLE hClient = nullptr;
    DWORD dwMaxClient = 2;
    DWORD dwCurVersion = 0;
    DWORD dwResult;

    dwResult = WlanOpenHandle(dwMaxClient, nullptr, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS)
    {
        std::string error = "WlanOpenHandle failed with error: " + std::to_string(dwResult);
        throw WlanException(error.c_str());
    }

    dwResult = WlanDisconnect(hClient, const_cast<GUID*>(&guid), nullptr);
    if (dwResult != ERROR_SUCCESS)
    {
        std::string error = "WlanDisconnect failed with error: " + std::to_string(dwResult);
        throw WlanException(error.c_str());
    }

    WlanCloseHandle(hClient, nullptr);
    return true;
}