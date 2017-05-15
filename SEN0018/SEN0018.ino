const int readerPin = 9;
const int heckpin = 10;
const int kpin = 11;

void setup() {
  Serial.begin(115200);
  pinMode(readerPin, INPUT);
  pinMode (heckpin, INPUT);
  pinMode(kpin, INPUT);
}
void loop()
{
  if ((digitalRead(readerPin) == LOW) && (digitalRead(heckpin) == LOW) && (digitalRead(kpin) == LOW))
  {
    Serial.println(F("no sheep!"));
  }
  else if ((digitalRead(readerPin) == HIGH) && (digitalRead(heckpin) == LOW) && (digitalRead(kpin) == LOW))
  {
    // Sheep One (14738)
    return 1;
  }
  else if ((digitalRead(readerPin) == LOW) && (digitalRead(heckpin) == HIGH) && (digitalRead(kpin) == LOW))
  {
    // Sheep Two (14739)
    return 2;
  }
  else if ((digitalRead(readerPin) == HIGH) && (digitalRead(heckpin) == HIGH) && (digitalRead(kpin) == LOW))
  {
    // Sheep Three (15702)
    return 3;
  }
  else if ((digitalRead(readerPin) == LOW) && (digitalRead(heckpin) == LOW) && (digitalRead(kpin) == HIGH))
  {
    // Sheep Four (15704)
    return 4;
  }
  else if ((digitalRead(readerPin) == HIGH) && (digitalRead(heckpin) == LOW) && (digitalRead(kpin) == HIGH))
  {
    // Sheep Five (15706)
    return 5;
  }
  else if ((digitalRead(readerPin) == LOW) && (digitalRead(heckpin) == HIGH) && (digitalRead(kpin) == HIGH))
  {
    // Sheep Six (15643)
    return 6;
  }
  else
  {
    Serial.println("nada");
  }

  delay(500);
}
