#include <Firebase_Arduino_WiFiNINA.h>
#include <Firebase_Arduino_WiFiNINA_HTTPClient.h>
#include <Arduino.h>
#include "GoogleArduino.h"

GoogleArduino::GoogleArduino(int resetPin)
{
    _resetPin = resetPin;
    setupPin(_resetPin,OUTPUT);
}

/**
 * Sets up the library to communicate to firebase.
 */
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

/**
 *  Sets the pin to the given output status.
 */
void GoogleArduino::setupPin(int _pin, bool status){
    pinMode(_pin,status);
}

/**
 * Performs a dry-check of the given path, sets the pin to whatever value is there. Does not
 * perform a remote check.
 */
void GoogleArduino::firstRun(String root, int pin){
    String path = root + "/OnOff/on";
    if(Firebase.getBool(firebaseData,path){
       Serial.println("First run for: " + path);
       bool newStatus = firebase.boolData();
       updateRelay(pin,newStatus);
    }else
        onError();
}

/**
 * Checks the status of the given path.  If it is on, set the given pin to on.
 */
void GoogleArduino::checkStatus(String root, int pin){
    String path = root + "/OnOff/remote";
    bool remoteOn = checkRemote(path);
    if(remoteOn && Firebase.getBool(firebaseData,path){
        bool newStatus = firebaseData.boolData();
        updateRelay(pin,root);
    }
}


/**
 * Updates the relay on the pin with the given status.
 */
void GoogleArduino::updateRelay(int pin, bool status, String root){
    digitalWrite(LED_BUILTIN,LOW);
    updateRemote(root);    
}

/**
 * Checks the path to see if the given on/off remote is enabled. 
 */
bool GoogleArduino::checkRemote(String path){
    path = path+"/OnOff/remote";

    if(Firebase.getBool(firebaseData,path))
        return firebaseData.boolData();
    else
        onError();
}

/**
 * What to do when an error occurs. 
 */
void GoogleArduino::onError(){
    digitalWrite(LED_BUILTIN,HIGH);
    digitalWrite(_resetPin,HIGH);
    digitalWrite(_resetPin,LOW);
}

/**
 * Gets the arduino's mac addr. 
 */
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

