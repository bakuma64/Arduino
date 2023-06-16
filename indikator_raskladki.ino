int inByte = 0;
int LED_R = 7;//красный пин
int LED_G = 6;//зеленый пин
int LED_B = 5;//синий пин
int RV = 255; // яркость
int GV = 255;// яркость
int BV = 255;// яркость
void setup() {
  Serial.begin(9600);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}
void loop() {
  if (Serial.available() > 0) {
    delay(100);
    inByte = Serial.read();
    if (inByte == '2') { // RU
      analogWrite(LED_R, 0);
      analogWrite(LED_G, 0);
      analogWrite(LED_B, GV);
    } else if (inByte == '1') { // EN
      analogWrite(LED_R, 0);
      analogWrite(LED_G, GV);
      analogWrite(LED_B, 0);
    
    }
  }
}
