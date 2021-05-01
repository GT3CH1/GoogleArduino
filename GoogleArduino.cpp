#include <Firebase_Arduino_WiFiNINA.h>
#include <Firebase_Arduino_WiFiNINA_HTTPClient.h>
#include <Arduino.h>
#include "GoogleArduino.h"

GoogleArduino::GoogleArduino(int resetPin)
{
    _resetPin = resetPin;
    pinMode(_resetPin,OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(resetPin,LOW);  
}

/**
 * Sets up the library to communicate to firebase.
 */
void GoogleArduino::setup(String firebaseHost, String firebaseAuth, char* ssid, char* wifiPass,
	int* invertedPins)
{
    _firebaseHost = firebaseHost;
    _firebaseAuth = firebaseAuth;
    _ssid = ssid;
    _wifiPass = wifiPass;
	_invertedPins = invertedPins;
    Firebase.reconnectWiFi(true);
    Firebase.begin(_firebaseHost,_firebaseAuth,_ssid,_wifiPass);
	delay(2500);
    Serial.println("My MAC");
	getMacAddr();
}

/**
 * Performs a dry-check of the given path, sets the pin to whatever value is there. Does not
 * perform a remote check.
 */
void GoogleArduino::firstRun(String root, int pin){
	setupPath(root,pin);
	delay(1000);
	String path = root + "/OnOff/on";
    if(Firebase.getBool(firebaseData,path)){
       Serial.println("First run for: " + root);
       bool newStatus = firebaseData.boolData();
       updateRelay(pin,!newStatus,root);
    }else
        onError();
}

void GoogleArduino::setupPath(String root, int pin){
	pinMode(pin,OUTPUT);
	String path = root + "/OnOff/on";
	Firebase.beginStream(firebaseData,path);
}


/**
 * Checks the status of the given path.  If it is on, set the given pin to on.
 */
void GoogleArduino::checkStatus(String root, int pin){
    Serial.println("Checking status for " + root); 
    bool remoteOn = checkRemote(root);
	String path = root + "/OnOff/on";
    if(remoteOn && Firebase.getBool(firebaseData,path)){
        bool newStatus = firebaseData.boolData();
        updateRelay(pin,newStatus,root);
    }
}


/**
 * Updates the relay on the pin with the given status.
 */
void GoogleArduino::updateRelay(int pin, bool status, String root){
    digitalWrite(LED_BUILTIN,LOW);
    if(!updateRemote(root))
		onError();
	if(pinInverted(pin))
		status = !status;	
    digitalWrite(pin,!status);
	Serial.println(root + " -> " + String(status));
	blinkLed(pin);
}

bool GoogleArduino::pinInverted(int pin){
	for(int i; i<sizeof(_invertedPins);i++)
		if(_invertedPins[i] == pin){
			Serial.println("Inverting pin: " + String(pin));
			return true;
		}
	return false;
}

bool GoogleArduino::updateRemote(String root){
	String path = root + "/OnOff/remote";
    bool success = false;
    Serial.print("Setting " + path + " to false... "); 
    if (! Firebase.getBool(firebaseData,path))
      onError();
	if(!firebaseData.boolData()){
		Serial.println("Remote is already off, ignoring.");
		return true;
	}
    success = Firebase.setBool(firebaseData,path,false);
    Serial.println(success ? "Success!" : "Failure!");
    return success; 
}

/**
 * Checks the path to see if the given on/off remote is enabled. 
 */
bool GoogleArduino::checkRemote(String root){
    String path = root+"/OnOff/remote";
	Serial.println("Checking remote -> " + path);
    if(Firebase.getBool(firebaseData,path)){
        bool status = firebaseData.boolData();
        Serial.println("Remote status is: " + String(status));
        return status;
    }
    else
        onError();
}

/**
 ;* What to do when an error occurs. 
 */
void GoogleArduino::onError(){
	Serial.println("Zoinks! An error occurred.");
	Serial.println(firebaseData.errorReason());
    digitalWrite(LED_BUILTIN,HIGH);
    digitalWrite(_resetPin,HIGH);
	delay(50);
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

void GoogleArduino::blinkLed(int blinks){
	for(int i = 0 ; i < blinks; i++){
		digitalWrite(LED_BUILTIN,HIGH);
		delay(100);
		digitalWrite(LED_BUILTIN,LOW);
		delay(100);
	}
}
