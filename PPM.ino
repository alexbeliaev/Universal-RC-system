// The code reads potentiometers value and produces PPM signal

#define Dp 3       // Digital port to output PPM 
#define Tp 500     // length of separation pulse
	

volatile int delays[4]={1500,1500,1500,1500};          // initial values values 

void setup() {
  pinMode(Dp, OUTPUT);   //DDRD |= 1<<3;   = 00001000  Set D3 direction for output (Not recieving)
}
  
void loop() {
  //======== Generate pulses =================
  int Tall = 0;
  for (byte i = 0; i < 4; i++) {
      
      int t = delays[i];
      Tall += t;

      digitalWrite(Dp, HIGH);
      delayMicroseconds(Tp);
      digitalWrite(Dp, LOW);       
      delayMicroseconds(t - Tp);
  } 
  // Start End Frame pulse:
      digitalWrite(Dp, HIGH);
      delayMicroseconds(Tp);
      digitalWrite(Dp, LOW);


  //======== do stuff and time it ============

  unsigned long T1 = micros();

  //======== ADC and map it on servo pulse length in [us]
  delays[0]=map(analogRead(A0), 0, 1023, 1000, 2000);         
  delays[1]=map(analogRead(A1), 0, 1023, 1000, 2000);
  delays[2]=map(analogRead(A2), 0, 1023, 1000, 2000);
  delays[3]=map(analogRead(A3), 0, 1023, 1000, 2000);

  
  // RF ...
  // LCD ... 
  
  
  //======== End of frame pulse

  unsigned long T2 = micros();
  delayMicroseconds(20000 - Tall - (T2-T1) - Tp);      // End frame pulse, T = 20 ms

}
