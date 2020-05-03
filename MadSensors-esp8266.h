#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <string.h>

#ifndef SERVER
#define SERVER "api.madsensors.com"
#endif

#ifndef USER_AGENT
#define USER_AGENT "MadSensorsESP8266"
#endif

#ifndef VERSION
#define VERSION "1.0"
#endif

#ifndef JSON_MAX_VAR
#define JSON_MAX_VAR 3
#endif

struct Value
{
    Value(float value, String context);
    ~Value() {};
    float value;
    String context;
};

class MadSensor
{
public:
    MadSensor(char* device_name, char* token);
    ~MadSensor() {};
    void connectWifi(char* ssid, char* password);
    HTTPClient* initHTTP();
    void killHTTPClient(HTTPClient* httpClient);
    void addNewValue(String context, float val);
    void addJSONelement(Value* value, int index);
    String getPOSTrequest();
    String sendAll();
    void cleanJSON();
    void viewJSON();

private:
    char* token;
    char* server;
    char* deviceName;
    char* ssid;
    char* password;
    Value* JSON[JSON_MAX_VAR];
    int index;
};
