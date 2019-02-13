// Demo for 74LS673 16 Bit Shift Register using serial to parallel mode, only
// 20190213 mflg tested - works
// use MSB first and send HIGHbyte first to have lowest frequency on Y15 - see code below
// Shift Out function uses POSITIVE shift clock => receiving circuit should use SPI Mode 0 which DOES NOT match 74LS673 
// 74LS673 shifts on negative edge - shift out works because both edges are being produces (see code at the end)
// However, better select correct mode ( SPI Mode 2 ?) when using hardware SPI

//Pin connected to Mode/STRCLK (5)
int latchPin = 8;
//Pin connected to SH_CLK (2)
int clockPin = 12;
////Pin connected to SER (6)
int dataPin = 11;

// connect *CS  (1) to GND
// connect       (2)  - see above
// connect R/*W   (3) to low
// connect *STRCLR (4) to High 
// connect          (5)  - see above
// connect           (6)  - see above
// connect GND(12) to GND
// connect VCC(24) to 5V
// Outputs 0...4 on Pins 7...11 and Outputs 5...15 on Pins 13...23

void setup() 
{
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{

  for (unsigned int numberToDisplay = 0; numberToDisplay < 65536; numberToDisplay++) 
  {
  digitalWrite(LED_BUILTIN, HIGH);   // turn on builtin LED

    // shift out 16 bits:
    // set datapin then clock high clock low bitorder lsb first (like uart), valuetosend is clocked out
    // Out0 should have highest blinking frequency, Out 15 lowest

    shiftOut(dataPin, clockPin, MSBFIRST,highByte(numberToDisplay)); 
    shiftOut(dataPin, clockPin, MSBFIRST,lowByte(numberToDisplay)); 

   // now Y15 has highest frequency and Y0 lowest
   // remind to swap order of bytewrites if changing to LSB first ! 
    
   // shiftOut(dataPin, clockPin, LSBFIRST,lowByte(numberToDisplay)); 
   // shiftOut(dataPin, clockPin, LSBFIRST,highByte(numberToDisplay)); 

   // Demo with fixed output
 
   // shiftOut(dataPin, clockPin, MSBFIRST,highByte(0x8002)); //=> Y15        active, all others low
   // shiftOut(dataPin, clockPin, MSBFIRST,lowByte(0x8002));  //       and Y1

   // shiftOut(dataPin, clockPin, LSBFIRST,lowByte(0x8002)); // => Y0          active, all others low
   // shiftOut(dataPin, clockPin, LSBFIRST,highByte(0x8002));  //     and Y14
    
   //take the latch pin high so the LEDs will light up:
    delayMicroseconds(50);
    digitalWrite(latchPin, HIGH);
    delayMicroseconds(50);
    digitalWrite(latchPin, LOW);
    // pause before next value:
    delayMicroseconds(50);
    digitalWrite(LED_BUILTIN, LOW);  // turn off builtin LED   
    delayMicroseconds(50);
  }
}

/*
  
 For reference 
 
 copied from wiring_shift.c - shiftOut() function  
  
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
  uint8_t i;

  for (i = 0; i < 8; i++)  {
    if (bitOrder == LSBFIRST)
      digitalWrite(dataPin, !!(val & (1 << i)));
    else  
      digitalWrite(dataPin, !!(val & (1 << (7 - i))));
      
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);    
  }
} 
*/
