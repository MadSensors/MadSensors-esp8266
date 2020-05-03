#include "MadSensors-esp8266.h"

Value::Value(float value, String context)
{
  this->value = value;
  this->context = context;
}

MadSensor::MadSensor(char* device_name, char* token)
{
  this->token = token;
  this->server = SERVER;
  this->deviceName = device_name;
  index = 0;
}

void MadSensor::connectWifi(char* ssid, char* password)
{
  this->ssid = ssid;
  this->password = password;

  const int ledPin = 13;
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  Serial.println();
  Serial.println("####### MadSensors.com #######");
  Serial.print("Device Name: ");
  Serial.println(this->deviceName);
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  digitalWrite(ledPin , HIGH);
  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP() );
}

HTTPClient* MadSensor::initHTTP()
{
  if(WiFi.status() == WL_CONNECTED)
  {
    char url[50];
    sprintf(url, "http://%s/v1.0/device/%s", server, deviceName);
    HTTPClient* httpClient = new HTTPClient;
    httpClient->begin(url);
    return httpClient;
  }
  else
  {
    Serial.println("HTTP Client is not created. No Wifi connection!");
    return NULL;
  }
}

void MadSensor::killHTTPClient(HTTPClient* httpClient)
{
  httpClient->end();
  delete httpClient;
}

void MadSensor::addNewValue(String context, float val)
{
  // Check JSON_MAX_VAR
  if (index == JSON_MAX_VAR) {
    Serial.print("Error: You can only send ");
    Serial.print(JSON_MAX_VAR);
    Serial.println(" variables per payload.");
  }
  else {
    Value* value = new Value(val, context);
    addJSONelement(value, index);
    ++index;
  }
}

void MadSensor::addJSONelement(Value* value, int index)
{
  JSON[index] = value;
}

String MadSensor::getPOSTrequest()
{
  String all = "{";
  for(int i = 0; i < index; i++)
  {
    if(i == index -1)
      {all += + "\"" + JSON[i]->context + "\"" + ": " + JSON[i]->value;}
    else
      {all += + "\"" + JSON[i]->context + "\"" + ": " + JSON[i]->value + ", ";}
  }
  all += "}";
  return all;
}

String MadSensor::sendAll()
{
  String POSTrequest = getPOSTrequest();
  HTTPClient* httpClient = this->initHTTP();
  httpClient->addHeader("Content-Type", "application/json");
  httpClient->addHeader("X-Auth-Token", token);
  int httpCode = httpClient->POST(POSTrequest);
  String payload = httpClient->getString();
  this->killHTTPClient(httpClient);
  cleanJSON();
  return String(httpCode) + ": " + payload;
}

void MadSensor::cleanJSON()
{
  for(int i = 0; i < index-1; i++)
  {
    delete JSON[i];
  }
  index = 0;
}

void MadSensor::viewJSON()
{
  for(int i = 0; i < index-1; i++)
  {
    Serial.println(JSON[i]->context + ":" + JSON[i]->value);
  }
}
