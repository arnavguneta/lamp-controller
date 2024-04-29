#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include <Arduino.h>

bool sendPostRequest(const String& url, const String& payload);
bool sendGetRequest(const String& url);

#endif
