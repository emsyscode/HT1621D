/*Begining of Auto generated code by Atmel studio */
/*
This code is not clean and also is not perfect, this is only 
a reference to extract ideas and adapte to your solution.
*/
#include<avr/io.h>
#include<util/delay.h>

void send_char(uint8_t cs, unsigned int a);
void send_cmd(uint8_t cs, unsigned int a);
#define LCD_in 8  // This is the pin number 8 on Arduino UNO
#define LCD_wr 9 // This is the pin number 9 on Arduino UNO
#define LCD_cs0 10 // This is the pin number 10 on Arduino UNO
#define LCD_cs1 11 // This is the pin number 10 on Arduino UNO
uint8_t chipSelect = HIGH;
bool flag = true;
uint8_t cs = LCD_cs1;

#define BUTTON_PIN 2 //Att check wich pins accept interrupts... Uno is 2 & 3
volatile byte buttonReleased = false;


void send_char(uint8_t cs, unsigned int a){
  unsigned int pinChipSelect = 0x00;
  pinChipSelect = cs;
 unsigned int data = 0; 
 unsigned int mask = 1; //our bitmask
  data=a;
  //digitalWrite(LCD_cs0, LOW); // When strobe is low, all output is enable. If high, all output will be set to low.
  digitalWrite(pinChipSelect, LOW);
  delayMicroseconds(2);
  // I define the size of mask to send only 12 bits
  for (mask = 0B000000000001; mask<4096; mask <<= 1) { //iterate through bit mask
   // Serial.println(mask, BIN);
  digitalWrite(LCD_wr,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(2);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(LCD_in, HIGH);
     // Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(LCD_in, LOW);
     // Serial.print(0);
    }
    digitalWrite(LCD_wr, HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(2);
  }
  digitalWrite(pinChipSelect, HIGH); // When strobe is low, all output is enable. If high, all output will be set to low.
    delayMicroseconds(2);
}
void send_cmd(uint8_t cs, unsigned int a){
  unsigned int pinChipSelect = 0x00;
  pinChipSelect = cs;
 // Attention: The "cmd" use 13 bits, the send_char use only 12
 unsigned int data = 0; 
 unsigned int mask = 1; //our bitmask
  data=a;
  digitalWrite(pinChipSelect, LOW); // When strobe is low, all output is enable. If high, all output will be set to low.
  delayMicroseconds(2);
  // I define the size of mask to send only 12 bits
  for (mask = 0B0000000000001; mask<8192; mask <<= 1) { //iterate through bit mask
    //Serial.println(mask, BIN);
  //digitalWrite(LCD_wr,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  digitalWrite(LCD_wr, LOW);
  delayMicroseconds(2);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(LCD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(LCD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(LCD_wr, HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(2);
  }
  digitalWrite(pinChipSelect, HIGH); // When strobe is low, all output is enable. If high, all output will be set to low.
    delayMicroseconds(2);
}
//READ 110 A5A4A3A2A1A0D0D1D2D3 D Read data from the RAM 
//WRITE 101 A5A4A3A2A1A0D0D1D2D3 D Write data to the RAM 
//READMODIFYWRITE 101 A5A4A3A2A1A0D0D1D2D3 D READ and WRITE to the RAM
//SYS DIS 100 0000-0000-X C Turn off both system oscillator and LCD bias generator Yes
//SYS EN 100 0000-0001-X C Turn on system oscillator LCD, OFF 100 0000-0010-X C 
//Turn off LCD bias generator Yes LCD ON 100 0000-0011-X C Turn on LCD bias generator
void initLCD(){
  //*****************************************************************//
  //  LCD OFF 10000000010X Turn off LCD outputs 
  //  LCD ON  10000000011X Turn on LCD outputs
  //
  //  BIAS & COM 1000010abXcX
  //  c=0: 1/2 bias option 
  //  c=1: 1/3 bias option 
  //  ab=00: 2 commons option 
  //  ab=01: 3 commons option 
  //  ab=10: 4 commons option
  //****************************************************************//
  //            X0Xba0100001 
  //send_char(cs, 0b000100100001); //100 0010-abX0-X BIAS 1/2 LCD 1/2 bias option ab=00: 2 commons option ab=01: 3 commons option ab=10: 4 commons option
  //send_char(cs, 0b010100100001); //100 0010-abX0-X BIAS 1/2 LCD 1/2 bias option ab=00: 2 commons option ab=01: 3 commons option ab=10: 4 commons option
  // The LCD I'm use, have 4 common pins. I let the position of "a" with "1" to select this option!
  send_char(cs, 0b000010100001); //100 0010-abX1-X BIAS 1/3 LCD 1/3 bias option ab=00: 2 commons option ab=01: 3 commons option ab=10: 4 commons option 
  send_char(cs, 0b001100000001);// TIMER EN 100 0000-0110-X  Enable time base output
  //send_char(cs, 0b000000000001); //SYS DIS Turn off both system oscillator and LCD bias generator, stay off
  //delay(2000);
  //send_char(cs, 0B100000000010); // SYS EN
  send_char(cs, 0B010000000001); // SYS EN
  //send_char(cs, 0B100000000110); // LCD ON
  send_char(cs, 0B011000000001); // LCD ON
  //send_char(cs, 0B001000000001); // LCD OFF
}
void testLCD(void){
  send_char(cs, 0b000000111001); //Mode Test
  delay(1000);
  send_char(cs, 0b011000111001); //Normal Mode Test
  delay(1000);
  segmentClear();
}
void fourBitsZero(void){
  // All 4 bits with 0000;
  //The last 4 bits of command are directment related with segments to be on or off!
  //The 6 bits after "101" are the address of RAM where you go write the 4 bits!
  send_cmd(cs, 0B0000000000101);
  send_cmd(cs, 0B0000100000101);
  send_cmd(cs, 0B0000010000101);
  send_cmd(cs, 0B0000110000101);
  send_cmd(cs, 0B0000001000101);
  send_cmd(cs, 0B0000101000101);
  send_cmd(cs, 0B0000011000101);
  send_cmd(cs, 0B0000111000101);
  send_cmd(cs, 0B0000000100101);
  send_cmd(cs, 0B0000100100101);
  send_cmd(cs, 0B0000010100101);
  send_cmd(cs, 0B0000110100101);
  send_cmd(cs, 0B0000001100101);
  send_cmd(cs, 0B0000101100101);
  send_cmd(cs, 0B0000011100101);
  send_cmd(cs, 0B0000111100101);
  //a0-a15 ... d0-d15
  send_cmd(cs, 0B0000000010101);
  send_cmd(cs, 0B0000100010101);
  send_cmd(cs, 0B0000010010101);
  send_cmd(cs, 0B0000110010101);
  send_cmd(cs, 0B0000001010101);
  send_cmd(cs, 0B0000101010101);
  send_cmd(cs, 0B0000011010101);
  send_cmd(cs, 0B0000111010101);
  send_cmd(cs, 0B0000000110101);
  send_cmd(cs, 0B0000100110101);
  send_cmd(cs, 0B0000010110101);
  send_cmd(cs, 0B0000110110101);
  send_cmd(cs, 0B0000001110101);
  send_cmd(cs, 0B0000101110101);
  send_cmd(cs, 0B0000011110101);
  send_cmd(cs, 0B0000111110101);
}
void allSegmentsOn(){
  send_cmd(cs, 0B1111000000101);// 3:0
  send_cmd(cs, 0B1111100000101);// 7:4
  send_cmd(cs, 0B1111010000101);//11:8
  send_cmd(cs, 0B1111110000101);//15:12
  send_cmd(cs, 0B1111001000101);//19:16
  send_cmd(cs, 0B1111101000101);//23:20
  send_cmd(cs, 0B1111011000101);//27:24
  send_cmd(cs, 0B1111111000101);//31:28
  //
  send_cmd(cs, 0B1111000100101);//35:32
  send_cmd(cs, 0B1111100100101);//39:36
  send_cmd(cs, 0B1111010100101);//43:40
  send_cmd(cs, 0B1111110100101);//47:44
  send_cmd(cs, 0B1111001100101);//51:48
  send_cmd(cs, 0B1111101100101);//55:52
  send_cmd(cs, 0B1111011100101);//59:56
  send_cmd(cs, 0B1111111100101);//63:60
  //a0-a15 ... d0-d15
  send_cmd(cs, 0B1111000010101);//67:64
  send_cmd(cs, 0B1111100010101);//71:68
  send_cmd(cs, 0B1111010010101);//75:72
  send_cmd(cs, 0B1111110010101);//79:76
  send_cmd(cs, 0B1111001010101);//83:80
  send_cmd(cs, 0B1111101010101);//87:84
  send_cmd(cs, 0B1111011010101);//91:88
  send_cmd(cs, 0B1111111010101);//95:92
  //
  send_cmd(cs, 0B1111000110101);// 99:96
  send_cmd(cs, 0B1111100110101);//103:100
  send_cmd(cs, 0B1111010110101);//107:104
  send_cmd(cs, 0B1111110110101);//111:108
  send_cmd(cs, 0B1111001110101);//115:112
  send_cmd(cs, 0B1111101110101);//119:116
  send_cmd(cs, 0B1111011110101);//123:120
  send_cmd(cs, 0B1111111110101);//127:124
}
void allSegmentsOff(){
  send_cmd(cs, 0B0000000000101);// 3:0
  send_cmd(cs, 0B0000100000101);// 7:4
  send_cmd(cs, 0B0000010000101);//11:8
  send_cmd(cs, 0B0000110000101);//15:12
  send_cmd(cs, 0B0000001000101);//19:16
  send_cmd(cs, 0B0000101000101);//23:20
  send_cmd(cs, 0B0000011000101);//27:24
  send_cmd(cs, 0B0000111000101);//31:28
  //
  send_cmd(cs, 0B0000000100101);//35:32
  send_cmd(cs, 0B0000100100101);//39:36
  send_cmd(cs, 0B0000010100101);//43:40
  send_cmd(cs, 0B0000110100101);//47:44
  send_cmd(cs, 0B0000001100101);//51:48
  send_cmd(cs, 0B0000101100101);//55:52
  send_cmd(cs, 0B0000011100101);//59:56
  send_cmd(cs, 0B0000111100101);//63:60
  //a0-a15 ... d0-d15
  send_cmd(cs, 0B0000000010101);//67:64
  send_cmd(cs, 0B0000100010101);//71:68
  send_cmd(cs, 0B0000010010101);//75:72
  send_cmd(cs, 0B0000110010101);//79:76
  send_cmd(cs, 0B0000001010101);//83:80
  send_cmd(cs, 0B0000101010101);//87:84
  send_cmd(cs, 0B0000011010101);//91:88
  send_cmd(cs, 0B0000111010101);//95:92
  //
  send_cmd(cs, 0B0000000110101);// 99:96
  send_cmd(cs, 0B0000100110101);//103:100
  send_cmd(cs, 0B0000010110101);//107:104
  send_cmd(cs, 0B0000110110101);//111:108
  send_cmd(cs, 0B0000001110101);//115:112
  send_cmd(cs, 0B0000101110101);//119:116
  send_cmd(cs, 0B0000011110101);//123:120
  send_cmd(cs, 0B0000111110101);//127:124
}
void anime0(){
  //WRITE 101 A5A4A3A2A1A0D0D1D2D3 D Write data to the RAM
  //.............DDDDAAAAAA101
  //.............3210012345WWW Last 3 bits as 101 is to write
  send_cmd(cs, 0B0000000000101);// 3:0
  send_cmd(cs, 0B0000100000101);// 7:4
  send_cmd(cs, 0B1000010000101);//11:8
  send_cmd(cs, 0B0000110000101);//15:12
  send_cmd(cs, 0B0000001000101);//19:16
  send_cmd(cs, 0B0000101000101);//23:20
  send_cmd(cs, 0B0000011000101);//27:24
  send_cmd(cs, 0B0000111000101);//31:28
  //
  send_cmd(cs, 0B0000000100101);//35:32
  send_cmd(cs, 0B0000100100101);//39:36
  send_cmd(cs, 0B0000010100101);//43:40
  send_cmd(cs, 0B0000110100101);//47:44
  send_cmd(cs, 0B0000001100101);//51:48
  send_cmd(cs, 0B0000101100101);//55:52
  send_cmd(cs, 0B0000011100101);//59:56
  send_cmd(cs, 0B0000111100101);//63:60
  //a0-a15 ... d0-d15
  send_cmd(cs, 0B0000000010101);//67:64
  send_cmd(cs, 0B0000100010101);//71:68
  send_cmd(cs, 0B0000010010101);//75:72
  send_cmd(cs, 0B0000110010101);//79:76
  send_cmd(cs, 0B0000001010101);//83:80
  send_cmd(cs, 0B0000101010101);//87:84
  send_cmd(cs, 0B0000011010101);//91:88
  send_cmd(cs, 0B0000111010101);//95:92
  //
  send_cmd(cs, 0B0000000110101);// 99:96
  send_cmd(cs, 0B0000100110101);//103:100
  send_cmd(cs, 0B0000010110101);//107:104
  send_cmd(cs, 0B0000110110101);//111:108
  send_cmd(cs, 0B0000001110101);//115:112
  send_cmd(cs, 0B0000101110101);//119:116
  send_cmd(cs, 0B0000011110101);//123:120
  send_cmd(cs, 0B0000111110101);//127:124
}
void anime1(){
  //WRITE 101 A5A4A3A2A1A0D0D1D2D3 D Write data to the RAM
  //.............DDDDAAAAAA101
  //.............3210012345WWW Last 3 bits as 101 is to write
  send_cmd(cs, 0B0000000000101);// 3:0
  send_cmd(cs, 0B0000100000101);// 7:4
  send_cmd(cs, 0B0000010000101);//11:8
  send_cmd(cs, 0B0000110000101);//15:12
  send_cmd(cs, 0B0000001000101);//19:16
  send_cmd(cs, 0B0000101000101);//23:20
  send_cmd(cs, 0B0000011000101);//27:24
  send_cmd(cs, 0B0000111000101);//31:28
  //
  send_cmd(cs, 0B0000000100101);//35:32
  send_cmd(cs, 0B0000100100101);//39:36
  send_cmd(cs, 0B0000010100101);//43:40
  send_cmd(cs, 0B0000110100101);//47:44
  send_cmd(cs, 0B0000001100101);//51:48
  send_cmd(cs, 0B0000101100101);//55:52
  send_cmd(cs, 0B0000011100101);//59:56
  send_cmd(cs, 0B0000111100101);//63:60
  //a0-a15 ... d0-d15
  send_cmd(cs, 0B0000000010101);//67:64
  send_cmd(cs, 0B0000100010101);//71:68
  send_cmd(cs, 0B0000010010101);//75:72
  send_cmd(cs, 0B0000110010101);//79:76
  send_cmd(cs, 0B0000001010101);//83:80
  send_cmd(cs, 0B0000101010101);//87:84
  send_cmd(cs, 0B0000011010101);//91:88
  send_cmd(cs, 0B0000111010101);//95:92
  //
  send_cmd(cs, 0B0000000110101);// 99:96
  send_cmd(cs, 0B0000100110101);//103:100
  send_cmd(cs, 0B0000010110101);//107:104
  send_cmd(cs, 0B0000110110101);//111:108
  send_cmd(cs, 0B0000001110101);//115:112
  send_cmd(cs, 0B0000101110101);//119:116
  send_cmd(cs, 0B1000011110101);//123:120
  send_cmd(cs, 0B0000111110101);//127:124
}
void anime2(){
  //WRITE 101 A5A4A3A2A1A0D0D1D2D3 D Write data to the RAM
  //.............DDDDAAAAAA101
  //.............3210012345WWW Last 3 bits as 101 is to write
  send_cmd(cs, 0B0000000000101);// 3:0
  send_cmd(cs, 0B1000100000101);// 7:4
  send_cmd(cs, 0B0000010000101);//11:8
  send_cmd(cs, 0B0000110000101);//15:12
  send_cmd(cs, 0B0000001000101);//19:16
  send_cmd(cs, 0B0000101000101);//23:20
  send_cmd(cs, 0B0000011000101);//27:24
  send_cmd(cs, 0B0000111000101);//31:28
  //
  send_cmd(cs, 0B0000000100101);//35:32
  send_cmd(cs, 0B0000100100101);//39:36
  send_cmd(cs, 0B0000010100101);//43:40
  send_cmd(cs, 0B0000110100101);//47:44
  send_cmd(cs, 0B0000001100101);//51:48
  send_cmd(cs, 0B0000101100101);//55:52
  send_cmd(cs, 0B0000011100101);//59:56
  send_cmd(cs, 0B0000111100101);//63:60
  //a0-a15 ... d0-d15
  send_cmd(cs, 0B0000000010101);//67:64
  send_cmd(cs, 0B0000100010101);//71:68
  send_cmd(cs, 0B0000010010101);//75:72
  send_cmd(cs, 0B0000110010101);//79:76
  send_cmd(cs, 0B0000001010101);//83:80
  send_cmd(cs, 0B0000101010101);//87:84
  send_cmd(cs, 0B0000011010101);//91:88
  send_cmd(cs, 0B0000111010101);//95:92
  //
  send_cmd(cs, 0B0000000110101);// 99:96
  send_cmd(cs, 0B0000100110101);//103:100
  send_cmd(cs, 0B0000010110101);//107:104
  send_cmd(cs, 0B0000110110101);//111:108
  send_cmd(cs, 0B0000001110101);//115:112
  send_cmd(cs, 0B0000101110101);//119:116
  send_cmd(cs, 0B0000011110101);//123:120
  send_cmd(cs, 0B0000111110101);//127:124
}
void anime3(){
  //WRITE 101 A5A4A3A2A1A0D0D1D2D3 D Write data to the RAM
  //.............DDDDAAAAAA101
  //.............3210012345WWW Last 3 bits as 101 is to write
  send_cmd(cs, 0B0000000000101);// 3:0
  send_cmd(cs, 0B0100100000101);// 7:4
  send_cmd(cs, 0B0000010000101);//11:8
  send_cmd(cs, 0B0000110000101);//15:12
  send_cmd(cs, 0B0000001000101);//19:16
  send_cmd(cs, 0B0000101000101);//23:20
  send_cmd(cs, 0B0000011000101);//27:24
  send_cmd(cs, 0B0000111000101);//31:28
  //
  send_cmd(cs, 0B0000000100101);//35:32
  send_cmd(cs, 0B0000100100101);//39:36
  send_cmd(cs, 0B0000010100101);//43:40
  send_cmd(cs, 0B0000110100101);//47:44
  send_cmd(cs, 0B0000001100101);//51:48
  send_cmd(cs, 0B0000101100101);//55:52
  send_cmd(cs, 0B0000011100101);//59:56
  send_cmd(cs, 0B0000111100101);//63:60
  //a0-a15 ... d0-d15
  send_cmd(cs, 0B0000000010101);//67:64
  send_cmd(cs, 0B0000100010101);//71:68
  send_cmd(cs, 0B0000010010101);//75:72
  send_cmd(cs, 0B0000110010101);//79:76
  send_cmd(cs, 0B0000001010101);//83:80
  send_cmd(cs, 0B0000101010101);//87:84
  send_cmd(cs, 0B0000011010101);//91:88
  send_cmd(cs, 0B0000111010101);//95:92
  //
  send_cmd(cs, 0B0000000110101);// 99:96
  send_cmd(cs, 0B0000100110101);//103:100
  send_cmd(cs, 0B0000010110101);//107:104
  send_cmd(cs, 0B0000110110101);//111:108
  send_cmd(cs, 0B0000001110101);//115:112
  send_cmd(cs, 0B0000101110101);//119:116
  send_cmd(cs, 0B0000011110101);//123:120
  send_cmd(cs, 0B0000111110101);//127:124
}
void anime4(){
 //WRITE 101 A5A4A3A2A1A0D0D1D2D3 D Write data to the RAM
  //.............DDDDAAAAAA101
  //.............3210012345WWW Last 3 bits as 101 is to write
  send_cmd(cs, 0B0000000000101);// 3:0
  send_cmd(cs, 0B0010100000101);// 7:4
  send_cmd(cs, 0B0000010000101);//11:8
  send_cmd(cs, 0B0000110000101);//15:12
  send_cmd(cs, 0B0000001000101);//19:16
  send_cmd(cs, 0B0000101000101);//23:20
  send_cmd(cs, 0B0000011000101);//27:24
  send_cmd(cs, 0B0000111000101);//31:28
  //
  send_cmd(cs, 0B0000000100101);//35:32
  send_cmd(cs, 0B0000100100101);//39:36
  send_cmd(cs, 0B0000010100101);//43:40
  send_cmd(cs, 0B0000110100101);//47:44
  send_cmd(cs, 0B0000001100101);//51:48
  send_cmd(cs, 0B0000101100101);//55:52
  send_cmd(cs, 0B0000011100101);//59:56
  send_cmd(cs, 0B0000111100101);//63:60
  //a0-a15 ... d0-d15
  send_cmd(cs, 0B0000000010101);//67:64
  send_cmd(cs, 0B0000100010101);//71:68
  send_cmd(cs, 0B0000010010101);//75:72
  send_cmd(cs, 0B0000110010101);//79:76
  send_cmd(cs, 0B0000001010101);//83:80
  send_cmd(cs, 0B0000101010101);//87:84
  send_cmd(cs, 0B0000011010101);//91:88
  send_cmd(cs, 0B0000111010101);//95:92
  //
  send_cmd(cs, 0B0000000110101);// 99:96
  send_cmd(cs, 0B0000100110101);//103:100
  send_cmd(cs, 0B0000010110101);//107:104
  send_cmd(cs, 0B0000110110101);//111:108
  send_cmd(cs, 0B0000001110101);//115:112
  send_cmd(cs, 0B0000101110101);//119:116
  send_cmd(cs, 0B0000011110101);//123:120
  send_cmd(cs, 0B0000111110101);//127:124
}
void anime5(){
  send_cmd(cs, 0B0000000000101);// 3:0
  send_cmd(cs, 0B0001100000101);// 7:4
  send_cmd(cs, 0B0000010000101);//11:8
  send_cmd(cs, 0B0000110000101);//15:12
  send_cmd(cs, 0B0000001000101);//19:16
  send_cmd(cs, 0B0000101000101);//23:20
  send_cmd(cs, 0B0000011000101);//27:24
  send_cmd(cs, 0B0000111000101);//31:28
  //
  send_cmd(cs, 0B0000000100101);//35:32
  send_cmd(cs, 0B0000100100101);//39:36
  send_cmd(cs, 0B0000010100101);//43:40
  send_cmd(cs, 0B0000110100101);//47:44
  send_cmd(cs, 0B0000001100101);//51:48
  send_cmd(cs, 0B0000101100101);//55:52
  send_cmd(cs, 0B0000011100101);//59:56
  send_cmd(cs, 0B0000111100101);//63:60
  //a0-a15 ... d0-d15
  send_cmd(cs, 0B0000000010101);//67:64
  send_cmd(cs, 0B0000100010101);//71:68
  send_cmd(cs, 0B0000010010101);//75:72
  send_cmd(cs, 0B0000110010101);//79:76
  send_cmd(cs, 0B0000001010101);//83:80
  send_cmd(cs, 0B0000101010101);//87:84
  send_cmd(cs, 0B0000011010101);//91:88
  send_cmd(cs, 0B0000111010101);//95:92
  //
  send_cmd(cs, 0B0000000110101);// 99:96
  send_cmd(cs, 0B0000100110101);//103:100
  send_cmd(cs, 0B0000010110101);//107:104
  send_cmd(cs, 0B0000110110101);//111:108
  send_cmd(cs, 0B0000001110101);//115:112
  send_cmd(cs, 0B0000101110101);//119:116
  send_cmd(cs, 0B0000011110101);//123:120
  send_cmd(cs, 0B0000111110101);//127:124
}
void anime6(){
  send_cmd(cs, 0B0000000000101);// 3:0
  send_cmd(cs, 0B0000100000101);// 7:4
  send_cmd(cs, 0B0000010000101);//11:8
  send_cmd(cs, 0B0000110000101);//15:12
  send_cmd(cs, 0B0000001000101);//19:16
  send_cmd(cs, 0B0000101000101);//23:20
  send_cmd(cs, 0B0000011000101);//27:24
  send_cmd(cs, 0B0000111000101);//31:28
  //
  send_cmd(cs, 0B0000000100101);//35:32
  send_cmd(cs, 0B0000100100101);//39:36
  send_cmd(cs, 0B0000010100101);//43:40
  send_cmd(cs, 0B0000110100101);//47:44
  send_cmd(cs, 0B0000001100101);//51:48
  send_cmd(cs, 0B0000101100101);//55:52
  send_cmd(cs, 0B0000011100101);//59:56
  send_cmd(cs, 0B0000111100101);//63:60
  //a0-a15 ... d0-d15
  send_cmd(cs, 0B0000000010101);//67:64
  send_cmd(cs, 0B0000100010101);//71:68
  send_cmd(cs, 0B0000010010101);//75:72
  send_cmd(cs, 0B0000110010101);//79:76
  send_cmd(cs, 0B0000001010101);//83:80
  send_cmd(cs, 0B0000101010101);//87:84
  send_cmd(cs, 0B0000011010101);//91:88
  send_cmd(cs, 0B0000111010101);//95:92
  //
  send_cmd(cs, 0B0000000110101);// 99:96
  send_cmd(cs, 0B0000100110101);//103:100
  send_cmd(cs, 0B0000010110101);//107:104
  send_cmd(cs, 0B0000110110101);//111:108
  send_cmd(cs, 0B0000001110101);//115:112
  send_cmd(cs, 0B0000101110101);//119:116
  send_cmd(cs, 0B0000011110101);//123:120
  send_cmd(cs, 0B0000111110101);//127:124
}
void segmentClear(void){
  unsigned int common=0;
  unsigned int address=0;
  unsigned int cmd=0b101;
  unsigned int mem=0;
  unsigned int aux=0;
  unsigned int var=0;
  unsigned int data=0;
  unsigned int temp=0;
  //
  for(common=0; common<16; common++){
    temp=0;
    var=(common << 9);      
              for (mem=0; mem< 64; mem++){
                data=0;
                data=(data | cmd);
                //Serial.print("Data 1: "); Serial.println(data, BIN); // Only for Debug
                aux=mem << 3;
                //Serial.print("aux: "); Serial.print(aux, BIN); Serial.print("; mem="); Serial.println(mem, DEC); // Only for Debug
                data=(data | aux);
                //Serial.print("Data 2: "); Serial.println(data, BIN); // Only for Debug
                temp=(data | var);
                send_cmd(cs, temp);
                  //Serial.println(temp, BIN);
                  delayMicroseconds(2);
                }
    }
}
void segmentTest(void){
  unsigned int common=0;
  unsigned int address=0;
  unsigned int cmd=0b101;
  unsigned int mem=0;
  unsigned int aux=0;
  unsigned int var=0;
  unsigned int data=0;
  unsigned int temp=0;
  //
      for(common=0; common<4; common++){
          temp=0;
          var=(common << 9);
          //Serial.print("var: "); Serial.print(var, BIN); Serial.print("; var=");Serial.println(var, DEC);// Only for Debug
          //
              for (mem=0; mem< 64; mem++){
                data=0;
                data=(data | cmd);
                //Serial.print("Data 1: "); Serial.println(data, BIN); // Only for Debug
                aux=mem << 3;
                //Serial.print("aux: "); Serial.print(aux, BIN); Serial.print("; mem="); Serial.println(mem, DEC); // Only for Debug
                data=(data | aux);
                //Serial.print("Data 2: "); Serial.println(data, BIN); // Only for Debug
                temp=(data | var);
                send_cmd(cs, temp);
                //Serial.println(temp, BIN); // Only for Debug
                delay(30);
                }
      }
}
void switchAllsegmentsON(){
      segmentClear();
      //send_cmd(cs, 0b1000010abX0X); //Please revert the sequence of bit's MSB to LSB on command.
      //send_cmd(cs, 0b000000100001); //Lowers segements active...
      //send_cmd(cs, 0b000100100001); //Partial segements active...
      send_cmd(cs, 0b000010100001);  //All segements active...because we select here the 4 common of LCD!
      //
      //send_cmd(cs, 0b100000000110);
      send_cmd(cs, 0b011000000001); //Segments ON
      delay(2000);
      send_cmd(cs, 0b001000000001); //Segments OFF
      delay(2000);
  }
  byte flipByte(byte c){
  char r=0;
  for(byte i = 0; i < 8; i++){
    r <<= 1;
    r |= c & 1;
    c >>= 1;
  }
  return r;
}
void segmentsSearch(){
  unsigned int segments=0b0001; //This is a length of 4 bits, they are the last 4 on 13 of command!
  uint8_t address=0;  //This is the 6 bits after defenition as command "101"
  unsigned int cmd=0b101;
  unsigned int bits=0b0000;
  unsigned int aux=0;
  unsigned int var=0;
  unsigned int data=0;
  unsigned int temp=0x0000;
  unsigned int numberSeg = 0;
  uint8_t reverted=0x00;
  // Ther I do a test to find segments from 0 until to 255!!! You can adjust the range of search!
    for (uint8_t mem=0; mem < 64; mem++){
      segmentClear();
      address=mem;
      reverted=flipByte(address);
      reverted = reverted & 0xFF;
      Serial.println(reverted, HEX);
      Serial.print("RAM addr: ");Serial.println(mem, DEC);
      //aux = (reverted << 3); //This move 3 to left, 
      aux = reverted; 
          for(segments = 0b1000; segments >0; segments = segments >> 1){
              temp=0;
              bits = ((~segments) & 0xF);
              var=(bits << 9);
  //              Serial.print("var: "); Serial.print(var, BIN); Serial.print("; var=");Serial.println(var, DEC);// Only for Debug
                    data=0;
                    data=(data | aux<<1);
                    data=(data | cmd);
  //                  Serial.print("Data & cmd: "); Serial.println(data, BIN); // Only for Debug
  //                  Serial.print("aux: "); Serial.print(aux, BIN); Serial.print("; mem="); Serial.println(mem, BIN); // Only for Debug
                    
                  Serial.print("Data with cmd & addr: "); Serial.println(data, BIN); // Only for Debug
                   temp=(data | var);
                   //Serial.print("Data & cmd & addr: "); Serial.println(temp, BIN); // Only for Debug
                  send_cmd(cs, temp);
                    Serial.println(temp, BIN); // Only for Debug
                    numberSeg++;
                    Serial.println(numberSeg, DEC); // Only for Debug
                  //delay(2000);
                              //This start the control of button to allow continue teste!
                              while (1) {
                                if (!buttonReleased) {
                                  delay(200);
                                }
                                else {
                                  delay(15);
                                  buttonReleased = false;
                                  break;
                                }
                              }                    
          }
    }
}
void allBlink(){
  for (unsigned int n = 0; n < 6; n++){
    allSegmentsOn();
    delay(1000);
    allSegmentsOff();
    delay(500);
  }
}
void animation(){
  unsigned int t1, t2, t3;
  t1=500;
  t2=100;
  t3=200;
  for(unsigned int m = 0; m < 2; m++){
        if (flag == true){
          flag = false;
          cs = LCD_cs0;
          initLCD();
        }
          else{
            flag = true;
            cs = LCD_cs1;
            initLCD();
          }
  //
  anime0();
  delay(t1); 
  anime1();
  delay(t1); 
  anime2();
  delay(t1); 
  anime3();
  delay(t1); 
  anime4();
  delay(t1); 
  anime5();
  delay(t1); 
  anime6();
  delay(t1); 
   }
}
void lcd0(){
 cs = LCD_cs0;
  initLCD();
  allSegmentsOn();
  delay(1000);
  allSegmentsOff();
  delay(1000);
  testLCD();
  delay(1000);
  segmentClear();
  delay(500);
  segmentTest();
  //delay(2000);
 animation();
}
void lcd1(){
  cs = LCD_cs1;
  initLCD();
  allSegmentsOn();
  delay(1000);
  allSegmentsOff();
  delay(1000);
  testLCD();
  delay(1000);
  segmentClear();
  delay(500);
  segmentTest();
  //  delay(2000);
  animation();
}
void setup() {
  //cli();
  pinMode(LCD_wr, OUTPUT);
  pinMode(LCD_in, OUTPUT);
  pinMode(LCD_cs0, OUTPUT);
  pinMode(LCD_cs1, OUTPUT);
  Serial.begin(115200);
  
  delayMicroseconds(5);
  //
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  //Next line is the attach of interruption to pin 2
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                  buttonReleasedInterrupt,
                  FALLING);
  //
    digitalWrite(LCD_cs1, HIGH);
    digitalWrite(LCD_cs0, HIGH);

      cs = LCD_cs0;
      initLCD();
      delay(100);
      cs = LCD_cs1;
      initLCD();
      delay(100);
  //testLCD();
  //delay(1000)
  //sei();
}
void loop() {
  cs = LCD_cs0;
  allSegmentsOn();
  delay(500);
  //segmentTest();
  //segmentClear();
  allSegmentsOff();
  delay(500);
  //
  cs = LCD_cs1;
  allSegmentsOn();
  delay(500);
  //segmentTest();
  //segmentClear();
  allSegmentsOff();
  delay(500);
  //
  animation();
  //segmentsSearch();//Uncomment this line if you want run the search of segments!!!
  }
//ISR zone!
void buttonReleasedInterrupt() {
  buttonReleased = true; // This is the line of interrupt button to advance one step on the search of segments!
}
