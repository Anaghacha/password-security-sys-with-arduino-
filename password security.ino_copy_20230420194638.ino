#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Servo.h>


SoftwareSerial BTserial(8,12 ); // SoftwareSerial BTserial(Rx, Tx)
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo sg90;
constexpr uint8_t servoPin = 9;
constexpr uint8_t buzzerPin = 13;
char initial_password[4] = {'1', '2', '3', '4'};  // Variable to store initial password
char password[4];   // Variable to store users password
boolean NormalMode = true; // boolean to change modes
char key_pressed = 0; // Variable to store incoming keys
uint8_t i = 0;  // Variable used for counter
const byte rows = 4;
const byte columns = 3;
char hexaKeys[rows][columns] = {
{'1', '2', '3'},
{'4', '5', '6'},
{'7', '8', '9'},
{'*', '0', '#'}
};
byte row_pins[rows] = {11, 7, 6, 5};
byte column_pins[columns] = {4, 3, 2};
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);
void setup() {
// Arduino Pin configuration
pinMode(buzzerPin, OUTPUT);
sg90.attach(servoPin);  //Declare pin 8 for servo
sg90.write(0); // Set initial position at 0 degrees
lcd.init();   // LCD screen
lcd.backlight();
// Arduino communicates with SIM900 GSM shield at a baud rate of 19200
// Make sure that corresponds to the baud rate of your module
BTserial.begin(9800);
}
void loop() {
if (NormalMode == false){
BTserial.print("Someone is trying to unlock the door");
}
else(NormalMode == true) ;{
key_pressed = keypad_key.getKey(); // Storing keys
if (key_pressed)
{
password[i++] = key_pressed; // Storing in password variable
lcd.print("*");
}
if (i == 4) // If 4 keys are completed
{
delay(200);
if (!(strncmp(password, initial_password, 4))) // If password is matched
{
lcd.clear();
lcd.print("Pass Accepted");
BTserial.print("\nDoor Opened \nIf it was't you, then there was an intrusion.");
sg90.write(90); // Door Opened
delay(5000);
sg90.write(0); // Door Closed
lcd.clear();
i = 0;
BTserial.print("\nDoor has been opened");
delay(2000);
}
else    // If password is not matched
{
lcd.clear();
lcd.print("\nWrong Password");
digitalWrite(buzzerPin, HIGH);
delay(3000);
digitalWrite(buzzerPin, LOW);
lcd.clear();
i = 0;
BTserial.print("\nSomeone is trying to open the door");
delay(2000);
lcd.print("\nClosed");
delay(5000);
lcd.clear();
}
}

}
}
