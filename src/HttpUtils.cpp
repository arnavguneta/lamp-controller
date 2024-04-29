#include "HttpUtils.h"
#include <HTTPClient.h>

HTTPClient http;

bool sendPostRequest(const String& url, const String& payload) {
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(payload);
    http.end();
    return httpResponseCode;
}

bool sendGetRequest(const String& url) {
    http.begin(url);
    int httpResponseCode = http.GET();
    http.end();
    return httpResponseCode;
}
