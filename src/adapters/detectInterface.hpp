//
// Created by TaxMachine on 2023-09-03.
//

#ifndef INTERNET_DISABLER_DETECTINTERFACE_HPP
#define INTERNET_DISABLER_DETECTINTERFACE_HPP

enum InterfaceType {
    ETHERNET,
    WIFI,
    UNKNOWN
};

namespace DetectInterface {
    InterfaceType Detect();
}

#endif //INTERNET_DISABLER_DETECTINTERFACE_HPP
