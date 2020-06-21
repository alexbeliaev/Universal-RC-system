#include "U8glib.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);	// Fast I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);	// Display which does not send AC

//U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);  // I added few options, no difference
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 


RF24 radio(7,5);      // CE 7, CSN 5

#define N 128             // number of channels
uint8_t values[N];                 // N-vector to store measurments


void setup(void) {
  // u8g.setRot180();       // flip screen, if required

  // Setup and configure rf radio
    radio.begin();
    radio.setAutoAck(false);    
}

void loop(void) {
    memset(values,0,sizeof(values));       // Clear measurement "values" by setting to "0"

    int n = 50;                          // number of repetitions                      
    while (n--) {
        int i = N;
        while (i--) {
            radio.setChannel(i);
            radio.startListening();
            delayMicroseconds(128);           // can set 256, ...  not sure what is better ...        
            radio.stopListening();
            if(radio.testCarrier()){++values[i];};
        }
    }
    
  
  
    // picture loop
      u8g.firstPage();  
      do {
        draw();
      } while( u8g.nextPage() );
      
}



void draw(void) {

  int i = 0;
   while ( i < N ) {
      int v = 31-values[i]*3;
      if(v<0){v=0;}; 
      u8g.drawLine(i,31,i,v);
      ++i;
    }
}
