## Setup
### Credentials
Create a WifiCredentials.h file in the format of:

```cpp
// src/WifiCredentials.h

#ifndef WIFI_CREDENTIALS_H
#define WIFI_CREDENTIALS_H

#define WIFI_SSID "Your_SSD"
#define WIFI_PASSWORD "Your_Password"

#endif
```

### Dependencies
Add arduinoWebSockets manually to the projects dependencies at `.pio/libdeps/x/arduinoWebSockets`

Found here: https://github.com/Links2004/arduinoWebSockets/
