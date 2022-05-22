// FAN Controller with USB serial interface based on ATtiny45/85
//
// This code implements a simple USB controlled FAN PWM Driver.
// It uses V-USB to build a USB communication device class (CDC) for serial
// communication via USB with the ATtiny.
//
//                           +-\/-+
//             A0 (D5) PB5  1|    |8  Vcc
// VUSB D- --- A3 (D3) PB3  2|    |7  PB2 (D2) A1 --- TACH
// VUSB D+ --- A2 (D4) PB4  3|    |6  PB1 (D1) ------ PWM
//                     GND  4|    |5  PB0 (D0) ------ SWITCH
//                           +----+
//
// Controller:  ATtiny45/85
// Core:        VUSB-AVR (https://github.com/wagiminator/VUSB-AVR)
// Clockspeed:  16.5 MHz internal
//
// FUSES:
// - LOW:  0xe1
// - HIGH: 0xdd
// - E:    0xff

// Libraries
#include <VUSB_CDC.h>     // part of USB-AVR core
#include <avr/pgmspace.h> // for using data in program space

// Pin definition
#define PWM PB1
#define SWITCH PB0
#define TACH PB2

// -----------------------------------------------------------------------------
// Main Function
// -----------------------------------------------------------------------------

// main setup
void setup()
{
  // prepare and start V-USB CDC
  Serial_VUSB.begin();

  pinMode(PWM, OUTPUT);
  pinMode(SWITCH, OUTPUT);
  pinMode(TACH, INPUT);

  // timer 1 for pwm
  // F_PWM = F / PRESCALER * TOP
  // F_PWM = 16.500.000Hz / 8 * 165
  // F_PWM = 25.000 Hz
  TCCR1 = 1 << CS10 | 1 << CS11 | 1 << PWM1A | 1 << COM1A1;
  OCR1C = 165 - 1; // Timer top value
}

// main loop
char inputMessage[8];
uint8_t counter = 0;
void loop()
{

  if (Serial_VUSB.available())
  {
    char input = Serial_VUSB.read();
    inputMessage[counter++] = input;
    if (input == '\r')
    {
      inputMessage[--counter] = '\0';
      counter = 0;
      setPWM(atoi(inputMessage));
    }
  }
  Serial_VUSB.delay(100);
}

void setPWM(uint8_t value)
{
  if (value <= 0)
    digitalWrite(SWITCH, LOW);
  else
    digitalWrite(SWITCH, HIGH);
  OCR1A = map(value, 0, 100, 0, 165 - 1);
}

//uint16_t getRPM(){
//  uint16_t = rpm = f * 30; // (f / 2) * 60;
//}