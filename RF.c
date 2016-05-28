#define PAYLOAD_SIZE 48

#define PULSE_WIDTH_SMALL  500

long buttons[] = {
  859124533L,
  861090613L,
  892547893L,
  1395864373L,
  859124563L,
  861090643L,
  892547923L,
  1395864403L,
  859125043L,
  861091123L,
  892548403L,
  1395864883L,
  859132723L,
  861098803L,
  892556083L,
  1395872563L
};

void sendData(long payload1, long payload2)
{
  long mask;
  char state;
  int ii;
  int jj;
  long payload;
  char bitt;

  
  mask = 1;
  state = 1;
  ii = 0;
  jj = 0;
  payload = payload1;
  
  
  
  
  
  
  
  // Send a preamble of 13 ms low pulse
  
  GPIO.B2 = 0;
  
  for (ii = 0; ii < 26; ii++)
  {
    Delay_us(PULSE_WIDTH_SMALL);
  }
  
  // send sync pulse : high for 0.5 ms
  GPIO.B2 = 1;
  Delay_us(PULSE_WIDTH_SMALL);
  GPIO.B2 = 0;

  // Now send the digits.
  // We send a 1 as a state change for 1.5ms, and a 0 as a state change for 0.5ms

  for (jj = 0; jj < PAYLOAD_SIZE; jj++)
  {
    if (jj == 32)
    {
      payload = payload2;
      mask = 1;
    }

    bitt = (payload & mask) ? 1 : 0;
    mask <<= 1;

    state = !state;
    GPIO.B2 = state;

    Delay_us(PULSE_WIDTH_SMALL);
    
    if (bitt)
    {
      Delay_us(PULSE_WIDTH_SMALL);
      Delay_us(PULSE_WIDTH_SMALL);
    }
  }
}


void simulate_button(int channel, int button, int on)
{
  long payload1;
  long payload2;
  int ii;
  
  payload1 = buttons[(channel - 1) * 4 + (button - 1)];
  payload2 = on? 13107L : 21299L;
  ii = 0;

  // Send the data 6 times
  for (ii = 0; ii < 6; ii++)
  {
    sendData(payload1, payload2);
  }
}



void main() {
      TRISIO = 0b00000000;  //IO  GP2=out
      
      while(1)
      {
              GPIO.B2 = 1;
              Delay_ms(1);
              GPIO.B2 = 0;
              Delay_ms(1);
              
                simulate_button(1, 1, 1);
                Delay_ms(5000);
                simulate_button(1, 1, 0);
                Delay_ms(5000);
      }
}