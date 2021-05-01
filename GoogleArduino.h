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
    void checkStatus(String path, int pin);
    void firstRun(String path,int pin);
  private:
    int _resetPin;
    String _firebaseHost;
	String _firebaseAuth;
    char* _authCode;
    char* _path;
    char* _ssid;
    char* _wifiPass;

    bool checkRemote(String path);
    bool resetRemote(String path);

	void onError();
    void updateRemote(String path);
    FirebaseData firebaseData;

};

#endif
