#ifndef google-home-arduino_h
#define google-home-arduino_h

#include <Firebase_Arduino_WiFiNINA.h>
#include <Firebase_Arduino_WiFiNINA_HTTPClient.h>
#include <Arduino.h>

class GoogleArduino
{
  public:
    GoogleArduino(int resetPin);
    void setup(String firebaseHost, String authCode, char* ssid, char* wifiPass,int* invertedPins);
    void checkStatus(String path);
	void getMacAddr();
    void checkStatus(String root, int pin);
    void firstRun(String path,int pin);
  private:
    int _resetPin;

    String _firebaseHost;
	String _firebaseAuth;
    
    char* _authCode;
    char* _path;
    char* _ssid;
    char* _wifiPass;
    int* _invertedPins;
    
    bool checkRemote(String path);
    bool resetRemote(String path);
    bool updateRemote(String root);
    bool pinInverted(int pin);	
    
    void onError();
    void updateRelay(int pin, bool status, String root);
    void blinkLed(int blinks);
    void setupPath(String root, int pin);
    FirebaseData firebaseData;

};

#endif
