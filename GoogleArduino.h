#ifndef google-home-arduino_h
#define google-home-arduino_h

#include <Firebase_Arduino_WiFiNINA.h>
#include <Firebase_Arduino_WiFiNINA_HTTPClient.h>
#include <Arduino.h>

class GoogleArduino
{
  public:
    GoogleArduino(int resetPin);
    void setup(String firebaseHost, String authCode, char* ssid, char* wifiPass);
	void setupPin(int _pin,bool status);
    void checkStatus(String path);
	void getMacAddr();
	void checkRemote(String path);
  private:
    int _resetPin;
    String _firebaseHost;
	String _firebaseAuth;
    char* _authCode;
    char* _path;
    FirebaseData firebaseData;
    char* _ssid;
    char* _wifiPass;
    bool resetRemote(String path);
	void onError();

};

#endif
