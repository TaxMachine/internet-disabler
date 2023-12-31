//
// Created by TaxMachine on 2023-09-03.
//

#include "ethernet.hpp"

#include <windows.h>
#include <iphlpapi.h>
#include <string>
#include "../WlanException.hpp"


bool Ethernet::DisableEthernetConnection()
{
    PIP_ADAPTER_INFO adapterInfo = nullptr;
    ULONG adapterInfoSize = 0;
    DWORD result = GetAdaptersInfo(adapterInfo, &adapterInfoSize);
    if (result == ERROR_BUFFER_OVERFLOW) {
        adapterInfo = (PIP_ADAPTER_INFO)malloc(adapterInfoSize);
        result = GetAdaptersInfo(adapterInfo, &adapterInfoSize);
    }
    if (result != NO_ERROR) {
        std::string error = "GetAdaptersInfo failed with error: " + std::to_string(result);
        throw WlanException(error.c_str());
    }
    std::string ifName = "Ethernet";
    for (PIP_ADAPTER_INFO adapter = adapterInfo; adapter != nullptr; adapter = adapter->Next) {
        std::string sIfName(adapter->AdapterName);
        if (sIfName.find(ifName) != std::string::npos) {
            ifName = sIfName;
            break;
        }
    }
    free(adapterInfo);

    MIB_IFTABLE* ifTable = nullptr;
    DWORD ifTableSize = 0;
    result = GetIfTable(ifTable, &ifTableSize, false);
    if (result == ERROR_INSUFFICIENT_BUFFER) {
        ifTable = (MIB_IFTABLE*)malloc(ifTableSize);
        result = GetIfTable(ifTable, &ifTableSize, false);
    }
    if (result != NO_ERROR) {
        std::string error = "GetIfTable failed with error: " + std::to_string(result);
        throw WlanException(error.c_str());
    }
    DWORD ifIndex = 0;
    for (DWORD i = 0; i < ifTable->dwNumEntries; i++) {
        MIB_IFROW ifRow = ifTable->table[i];
        std::wstring wIfName(ifRow.wszName);
        std::string sIfName(wIfName.begin(), wIfName.end());
        if (sIfName == ifName) {
            ifIndex = ifRow.dwIndex;
            break;
        }
    }
    free(ifTable);

    MIB_IFROW ifRow;
    ifRow.dwIndex = ifIndex;
    ifRow.dwAdminStatus = IF_ADMIN_STATUS_DOWN;
    result = SetIfEntry(&ifRow);
    if (result != NO_ERROR) {
        std::string error = "SetIfEntry failed with error: " + std::to_string(result);
        throw WlanException(error.c_str());
    }
    return true;
}