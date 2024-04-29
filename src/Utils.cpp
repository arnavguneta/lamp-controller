#include "Utils.h"
#include <iostream>
#include <ctime>

bool isDarkOutside() {
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    int hour = localTime->tm_hour;   
    int minute = localTime->tm_min;  

    // Define the start and end times for "dark outside"
    int darkStartHour = 20; // 8:00 PM
    int darkStartMinute = 30;
    int darkEndHour = 2;    // 2:00 AM

    // Check if the current time is within the defined period
    if ((hour > darkStartHour || (hour == darkStartHour && minute >= darkStartMinute)) ||
        (hour < darkEndHour || (hour == darkEndHour && minute < darkStartMinute))) {
        return true;
    } else {
        return false;
    }
}