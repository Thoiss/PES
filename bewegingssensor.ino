int led = 14;     // Led positive terminal to the digital pin 9.
int sensor = 12;  // signal pin of sensor to digital pin 5.
int state = LOW;
int val = 0;

void setup() {             // Void setup is ran only once after each powerup or reset of the Arduino  board.
  pinMode(led, OUTPUT);    // Led is determined as an output here.
  pinMode(sensor, INPUT);  // PIR motion sensor is determined is an input here.
  Serial.begin(9600);
}

void loop() {  // Void loop is ran over and  over and consists of the main program.
  val = digitalRead(sensor);
  //Serial.print("Sensor value: ");
  //Serial.println(val);  // Altijd printen om te zien of er iets gebeurt
  if (val == HIGH) {
    digitalWrite(led, HIGH);
    delay(100);  // Delay of led is 100

    if (state == LOW) {
      Serial.println("  Motion detected ");
      state = HIGH;
    }
  } else {
    digitalWrite(led, LOW);
    delay(100);

    if (state == HIGH) {
      Serial.println("The action/ motion has stopped");
      state = LOW;
    }
  }
}
