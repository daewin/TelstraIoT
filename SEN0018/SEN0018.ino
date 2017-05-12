 const int buttonPin = 8;     
 const int heckpin = 9;
  void setup() { 
    Serial.begin(115200);    
    pinMode(buttonPin, INPUT);   
    pinMode (heckpin, INPUT);  
  }
  void loop()
  {
     if ((digitalRead(buttonPin) == LOW)&& (digitalRead(heckpin) == LOW))
     {     
       Serial.println(F("no sheep!"));
     } 
     else if ((digitalRead(buttonPin) == HIGH)&& (digitalRead(heckpin) == LOW))
     {
        // ID set to Sheep One (14738)
       
       Serial.println(F("sheep numer uno"));
     }
     else if ((digitalRead(buttonPin) == LOW)&& (digitalRead(heckpin) == HIGH))
     {
        // ID set to Sheep Two (14739)
        
        Serial.println(F("Sheep numer dos"));
     }
     else
     {
        Serial.println("nada");
     }

     delay(500);
  }
