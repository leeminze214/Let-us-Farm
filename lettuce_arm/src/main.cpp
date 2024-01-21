#include <Arduino.h>
#include <ESP32Servo.h>

// put function declarations here:
// int myFunction(int, int);
Servo s1;
Servo s2;

void setup() {
  // put your setup code here, to run once:
  //   int result = myFunction(2, 3);
  s1.attach(18, 500, 2500);
  s2.attach(19, 500, 2500);

  //   s1.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i <= 270; i += 1) {
    s1.write(i);
    s2.write(i);
    delay(15);
  }
  for (int i = 270; i > 0; i -= 1) {
    s1.write(i);
    s2.write(i);
    delay(15);
  }

}

// put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }