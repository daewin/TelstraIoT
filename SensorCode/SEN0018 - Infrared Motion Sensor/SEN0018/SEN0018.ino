 const int buttonPin = 8;     
  void setup() { 
    Serial.begin(115200);    
    pinMode(buttonPin, INPUT);     
  }
  void loop()
  {
     if (digitalRead(buttonPin) == HIGH) 
     {     
       Serial.println(F("HIGH!"));
     } 
     else {
       Serial.println(F("low..."));
     }

     delay(5000);
  }
