#include <iostream>

#include <windows.h>
#include <wlanapi.h>
#include <cstdlib>

#pragma comment(lib, "wlanapi.lib")

GUID GetWlanInterfaceGuid() {
    HANDLE hClient = nullptr;
    DWORD dwMaxClient = 2;
    DWORD dwCurVersion = 0;
    DWORD dwResult;

    dwResult = WlanOpenHandle(dwMaxClient, nullptr, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS)
    {
        std::cout << "WlanOpenHandle failed with error: " << dwResult << std::endl;
        return {};
    }

    PWLAN_INTERFACE_INFO_LIST pIfList = nullptr;
    dwResult = WlanEnumInterfaces(hClient, nullptr, &pIfList);
    if (dwResult != ERROR_SUCCESS)
    {
        std::cout << "WlanEnumInterfaces failed with error: " << dwResult << std::endl;
        WlanCloseHandle(hClient, nullptr);
        return {};
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


bool DisableWifiConnection(const GUID& guid) {
    HANDLE hClient = nullptr;
    DWORD dwMaxClient = 2;
    DWORD dwCurVersion = 0;
    DWORD dwResult;

    dwResult = WlanOpenHandle(dwMaxClient, nullptr, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS)
    {
        std::cout << "WlanOpenHandle failed with error: " << dwResult << std::endl;
        return false;
    }

    dwResult = WlanDisconnect(hClient, const_cast<GUID*>(&guid), nullptr);
    if (dwResult != ERROR_SUCCESS)
    {
        std::cout << "WlanDisconnect failed with error: " << dwResult << std::endl;
        WlanCloseHandle(hClient, nullptr);
        return false;
    }

    WlanCloseHandle(hClient, nullptr);
    return true;
}


int main() {
    GUID guid = GetWlanInterfaceGuid();
    DisableWifiConnection(guid);
}
