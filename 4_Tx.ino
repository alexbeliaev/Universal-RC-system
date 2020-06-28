#include <SPI.h>                 // lib for SPI bus
#include <nRF24L01.h>            // setup RF24
#include <RF24.h>                // lib for nRF24L01+

RF24  radio(7, 5);               // (CE=7, CSN=5) create object "radio" for RF24,

#define N 8                      // number of bytes in Array = 32/4 = 8  (Создаём массив для пере дачи данных)
//byte A[N];                     
unsigned long A[N];              // 4 bytes per element

void setup(){

    for (byte i = 0; i < N; i++) {            // populate A vector
      A[i] = i;                               
    }

   Serial.begin(9600);
 
   radio.begin();                             // create abject "radio"
    delay(2000);                              // give it 2[s] to put things in order, otherwise unplug/plug
    
    //radio.powerDown();
    radio.stopListening();                    // just in case, can ignore      
    radio.setAutoAck(false);                  // No need for replay  (0 = false,  1 = true)
    radio.setPayloadSize(32);                 // default is 32, it does affect the N bytes sent, but not the speed ...   
    radio.setChannel(4);                      // set RF channel (0 .. 127)
    radio.setDataRate(RF24_1MBPS);            // speed (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS). Note RF24_250KBPS only for NRF24 plus (nRF24L01+) units 
    radio.setPALevel(RF24_PA_HIGH);           // Power of the Power Amplifier (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    //radio.setCRCLength(RF24_CRC_16);        // packets CRC: 8-bit or 16-bit
    radio.openWritingPipe(123);               // open pipe to create packets ID "123" (on one channel can have 1 receiver and 6 transmitters)
    radio.powerUp();                          // powerUp включение или пониженное потребление powerDown                                                        
}

unsigned long n = 0;         // 4 bytes

void loop(){

    for (int i = 0; i < 1000; i++) {
      A[0] = n;
      radio.write(&A, 32);              // send 32 bytes of data, does not affect speed, if > 32 the bytes are ignored
      n+=1;
    }     

     Serial.println(A[0]);
     Serial.println(A[7]);
}
