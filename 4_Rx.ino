#include "U8glib.h"              // lib for OLED

#include <SPI.h>                 // lib for SPI bus
#include <nRF24L01.h>            // setup RF24
#include <RF24.h>                // lib for nRF24L01+


U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);      // Tested     I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Tested     Display which does not send AC


RF24 radio(7,5);                 // (CE=7, CSN=5) create object "radio" for RF24, 

#define N 8                      // number of bytes in Array = 32/4 = 8  (Создаём массив для передачи данных)
//byte A[N];                    
unsigned long A[N];              // 4 bytes per element

void setup(){
    delay(1000);                              //give it 2[s] to put things in order, otherwise unplug/plug
    Serial.begin(9600);
  
    radio.begin();                            // initialize nRF24L01+
    //radio.stopListening();                  // reviver OFF
    radio.setAutoAck(false);                  // No need for replay  (0 = false,  1 = true)
    
    radio.setChannel(4);                      // set RF channel (0 .. 127)
    radio.setDataRate(RF24_1MBPS);            // speed (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS). Note RF24_250KBPS only for NRF24 plus (nRF24L01+) units 
    radio.setPALevel(RF24_PA_HIGH);           // power of the Power Amplifier (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm) 
    //radio.setCRCLength(RF24_CRC_16);        // packets CRC: 8-bit or 16-bit                                               
    radio.openReadingPipe(1,123);             // open pipe to create packets ID "123" (on one channel can have 1 receiver and 6 transmitters)
    radio.startListening();                   // receiver ON
}



int Err;

void loop(){

    int j=0,i = 0;
    unsigned long d, T;
    Err=0;
    T = micros();
    do{  
      j++;                                   // we do 28k loops for each 1000 radio.available() == true
      if(radio.available()){                 // if we have received data in the buffer
          i++;
          d = A[0];
          radio.read(&A, 32);                // set number of bytes to read (does not depends on the type of "A" variable )
          d = A[0]-d; 
          if ( d > 1 and i > 5 ){ Err = Err + d-1;}   // i>4 as we need OLED, Serial ... to become quit, not interupt my code.
          if (Err > 1000) {i=1000; Err = 100; }
      }
    } while (i<1000);  
    
    T = micros()-T;



//============= Debug =================== 
    Serial.print(j);
    Serial.print(", ");
    Serial.println(i);
    
    Serial.println(A[0]);
    Serial.println(A[7]);
    //float p = Err/10.0;
    //Serial.println(p);
    
    Serial.print("T = ");
    Serial.println(T);

    Serial.print("Errors = ");
    Serial.println(Err);
    
            
 
     u8g.firstPage();                                      // picture loop
     do {draw();} while( u8g.nextPage() );
     
}


//============= OLED ===================
void draw(void) {

    // Print value
    u8g.setFont(u8g_font_unifont);
    
    u8g.drawStr(5, 12, "Lost");
    u8g.setPrintPos(55,12);
    float p = Err/10.0;               // divide by N = 1000 packets  x 100% 
    u8g.print(p);
    u8g.drawStr(100, 12, "%");

    u8g.drawStr(5, 30, "Total");
    u8g.setPrintPos(55,30);
    u8g.print(A[0]);
}
