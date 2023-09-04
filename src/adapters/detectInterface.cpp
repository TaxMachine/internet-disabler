//
// Created by TaxMachine on 2023-09-03.
//

#include "detectInterface.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <WinSock2.h>
#include <iphlpapi.h>

#include "../WlanException.hpp"

#pragma comment(lib, "iphlpapi.lib")


InterfaceType DetectInterface::Detect() {
    ULONG bufferSize = 0;
    DWORD result = GetIpForwardTable(nullptr, &bufferSize, false);
    if (result != ERROR_INSUFFICIENT_BUFFER) {
        std::string error = "GetIpForwardTable failed with error: " + std::to_string(result);
        throw WlanException(error.c_str());
    }
    auto* ipForwardTable = (MIB_IPFORWARDTABLE*)malloc(bufferSize);
    result = GetIpForwardTable(ipForwardTable, &bufferSize, false);
    if (result != NO_ERROR) {
        std::string error = "GetIpForwardTable failed with error: " + std::to_string(result);
        throw WlanException(error.c_str());
    }
    DWORD defaultGateway = 0;
    for (DWORD i = 0; i < ipForwardTable->dwNumEntries; i++) {
        MIB_IPFORWARDROW ipForwardRow = ipForwardTable->table[i];
        if (ipForwardRow.dwForwardDest == 0 && ipForwardRow.dwForwardMask == 0) {
            defaultGateway = ipForwardRow.dwForwardNextHop;
            break;
        }
    }
    free(ipForwardTable);

    if ((defaultGateway & 0xFF000000) == 0x0A000000 ||
        (defaultGateway & 0xFFFF0000) == 0xC0A80000 ||
        (defaultGateway & 0xFFF00000) == 0xAC100000) {
        return ETHERNET;
    } else if ((defaultGateway & 0xFF000000) == 0x80000000 ||
               (defaultGateway & 0xFF000000) == 0xC0000000 ||
               (defaultGateway & 0xFF000000) == 0xE0000000) {
        return UNKNOWN;
    } else {
        return WIFI;
    }
}