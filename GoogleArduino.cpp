#include <Firebase_Arduino_WiFiNINA.h>
#include <Firebase_Arduino_WiFiNINA_HTTPClient.h>
#include <Arduino.h>
#include "GoogleArduino.h"

GoogleArduino::GoogleArduino(int resetPin)
{
	_resetPin = resetPin;
	setupPin(_resetPin,OUTPUT);
}

void GoogleArduino::setup(String firebaseHost, String firebaseAuth, char* ssid, char* wifiPass)
{
    _firebaseHost = firebaseHost;
    _firebaseAuth = firebaseAuth;
    _ssid = ssid;
    _wifiPass = wifiPass;
    Firebase.reconnectWiFi(true);
    Firebase.begin(_firebaseHost,_firebaseAuth,_ssid,_wifiPass);
	Serial.println("My MAC");
}

void GoogleArduino::setupPin(int _pin, bool status){
	pinMode(_pin,status);
}

bool GoogleArduino::checkRemote(String path){
    path = path+"/OnOff/remote";

    if(Firebase.getBool(firebaseData,path))
        return firebaseData.boolData();
    else
        onError();
}

void GoogleArduino::onError(){
	digitalWrite(LED_BUILTIN,HIGH);
	digitalWrite(_resetPin,HIGH);
	digitalWrite(_resetPin,LOW);
}

void GoogleArduino::getMacAddr(){
	byte mac[6];
	WiFi.macAddress(mac);
    Serial.print("MAC: ");
    Serial.print(mac[5],HEX);
    Serial.print(":");
    Serial.print(mac[4],HEX);
    Serial.print(":");
    Serial.print(mac[3],HEX);
    Serial.print(":");
    Serial.print(mac[2],HEX);
    Serial.print(":");
    Serial.print(mac[1],HEX);
    Serial.print(":");
    Serial.println(mac[0],HEX);
}

