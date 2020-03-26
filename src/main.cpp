#include <Arduino.h>
#include <Wifi.h>
#include <NTPClient.h>


int HOURS_PINS[5] = {15, 2, 4, 5, 18};
int MINUTES_PINS[6] = {13, 12, 14, 27, 26, 25};

int HOURS = 0;
int MINUTES = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);



void connectToWiFi(){

    const char* WIFI_SSID = "************";
    const char* WIFI_PASS = "************";

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

}


void updateTime(int *hours, int *minutes){

    
    String formattedTime;
    String stringHours;
    String stringMinutes;

    //get timestamp
    while(!timeClient.update()) {
        timeClient.forceUpdate();
    }
    formattedTime = timeClient.getFormattedTime();

    //convert string timestamp to int
    stringHours = formattedTime.substring(0, 2);
    stringMinutes = formattedTime.substring(3, 5);

    *hours = atoi(stringHours.c_str());
    *minutes = atoi(stringMinutes.c_str());
}

void setup() {

	//start serial communication
	Serial.begin(9600);

    //connect to wifi 
    connectToWiFi();

    timeClient.begin();
	
    
	//set led pin's as output
	for(int i = 0; i < 5; i++)
    	pinMode(HOURS_PINS[i], OUTPUT);
	for(int i = 0; i < 6; i++)
    	pinMode(MINUTES_PINS[i], OUTPUT);	
	
    

}

void loop() {

    Serial.println("\n------------------------------------------\n");

    //update hours & seconds    
    updateTime(&HOURS, &MINUTES);
    
    Serial.print("Hours : ");
    Serial.println(HOURS);

    Serial.print("Binary : ");
    //convert hours to binary
    for(int i = 0; i < 5; i++){
        digitalWrite(HOURS_PINS[i], HOURS % 2);
        Serial.print(HOURS % 2);
        HOURS /= 2;
    }
    Serial.println();
    Serial.println();

    Serial.print("Minutes : ");
    Serial.println(MINUTES);
    Serial.print("Binary : ");
    //convert minutes to binary
    for(int i = 0; i < 6; i++){
        digitalWrite(MINUTES_PINS[i], MINUTES % 2);
        Serial.print(MINUTES % 2);
        MINUTES /= 2;
    }
    Serial.println();
    Serial.println();
	
	delay(1000);
}
