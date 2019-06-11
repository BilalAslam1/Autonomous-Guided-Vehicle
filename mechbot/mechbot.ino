  // Mechbot#7
  // Evshield#9
  // Group 10
  // Bilal Aslam
  #include <mechbotShield.h>
  #include <avr/io.h>
  #define left analog(0)
  #define leftc analog(1)
  #define rightc analog(2)
  #define right analog(3)
  #define leftw analog(4)
  #define centerw analog(5)
  #define rightw analog(6)

  void linefollow ()
  {
    motor (500,500);
    while ((left > 800)||(leftc> 800)||(rightc > 800)||(right > 800))
    {

      //One of the sensors is touching the black line
      if ((left > 975) && (leftc > 950) && (rightc > 950) && (right > 965))
      { //All sensors are touching the blackline (cross)
        break; //break out of the while loop
      }
      else if (left < 920)
      {
        motor(700, -200); //Steer right
      }
      else if (right < 880)
      {
        motor(-250, 700); //Steer left
      }
      else
      {
        motor(400, 400); //Go straight
      }
      delay_ms(50);

    }
    motor(0,0);
    delay_ms(50);

  }

  void linefollow_Obstacle ()
  {
    motor (500,500);
    while ((left > 800)||(leftc> 800)||(rightc > 800)||(right > 800))
    {

      //One of the sensors is touching the black line
      if ((left > 975) && (leftc > 950) && (rightc > 950) && (right > 965))
      { //All sensors are touching the blackline (cross)
        break; //break out of the while loop
      }
      else if (left < 920)
      {
        motor(700, -200); //Steer right
      }
      else if (right < 880)
      {
        motor(-250, 700); //Steer left
      }
      else
      {
        motor(400, 400); //Go straight
      }
      delay_ms(50);

      if(((PIND&(1<<PD4))==0)||((PIND&(1<<PD5))==0))
      { //obstacle gets hit
        motor(0,0);
        delay_ms(100);
        motor(-400,-400);
        delay_ms(400);
        while(1)
        {
          beep(500, 1000000); // beep buzzer
          PORTB |= (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4)|(1<<PB5); // flash lights
          PORTD |= (1<<PD6)|(1<<PD7);
          motor(0,0);
          clrLCD();
         //front distance sensor
          lcdPrint("Distance = ");
          lcdPrintDec(centerw);
          delay_ms(500);
          if(distance<300)
          {
            break;
          }
        }
      }
    }
    motor(0,0);
    delay_ms(50);

  }
  }

  int main(void)
  {

    int distance;

    //Initialize and set ports as outputs and low
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB3) | (1 << PB4) | (1 <<PB5);
    PORTB &= ~ (1 << PB0) | (1 << PB1) | (1 << PB3) | (1 << PB4) | (1 <<PB5);

    //Initialize and set ports as outputs and low
    DDRC |= ((1 << PC4) | (1 << PC5));
    PORTC &=~ (1 << PC4) | (1 << PC5);

    //Initialize and set ports as inputs and high
    DDRD &= ~((1 << PD3) | (1 << PD4) | (1 << PD5));
    PORTD &=~ (1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);

    //PORTC &=~(1<<PC5);// Set low
    //PORTC |=(1<<PC5);// Set high,
    // DDRC |=(1<<PC4);//Set something as output.

    //initializing LCD
    initSoftSerial();
    setLCDBackLight(147);
    initADC();
    clrLCD();
    moveLCDCursor(0);

    initADC();
    initMotor();
    int timer=0;


    //*****************************************Getting to Tjunction 1********************************************************
     linefollow1 ();

    //Distance Sensor
    // PC4 indicates Tjunction, PC5 high indicates can on right side and low indicates left side
    if (analog(4) >250)
    {
      clrLCD();
      moveLCDCursor(0);
      lcdPrint("Can on left side");
      PORTC |=(1<<PC4); //set port high
      PORTC&=~ (1<<PC5); // set port low
      delay_ms(500);
      PORTC&=~ (1<<PC4);
    }
    else if (analog(6) > 250)
    {
      PORTC |=(1<<PC4); //set port high
      PORTC |=(1<<PC5); //set port high
      delay_ms(500);
      PORTC&=~ (1<<PC5);
      PORTC&=~ (1<<PC4);
      clrLCD();
     moveLCDCursor(0);
     lcdPrint("Can on right side");
    }

    // stop motor at Tjunction.
    //motor(0,0);
    delay_ms(400);
    clrLCD();
    moveLCDCursor(0);
    //delay_ms(1000);
    lcdPrint("T junction 1");

    //Clear bits/set them low for next time use
    PORTC&=~ (1<<PC4);
    PORTC&=~ (1<<PC5);

    //------------------------------------------------------Wait On Evsheild------------------------------------------------------------
    delay_ms(5000); // wait for EVShield to run
    motor(600,600);
    delay_ms(650);


  //*****************************************Getting to Tjunction 2********************************************************
  linefollow1();

  // tell evsheild arrived at 2 TJuction
  clrLCD();
  moveLCDCursor(0);
  lcdPrint("T junction 2");
  PORTC |=(1<<PC4); //set port high
  delay_ms(100);
  PORTC&=~ (1<<PC4);

    //------------------------------------------------------Wait On Evsheild------------------------------------------------------------
    delay_ms(6000); // wait for EVShield to run
    motor(600,600);
    delay_ms(650);

    //Clear bits/set them low for next time use
    PORTC&=~ (1<<PC4);
    PORTC&=~ (1<<PC5);

    //*****************************************Getting to Tjunction 3********************************************************
     linefollow1 ();

    //Distance Sensor
    // PC4 indicates Tjunction, PC5 high indicates can on right side and low indicates left side
    if (analog(4) >250)
    {
      clrLCD();
      moveLCDCursor(0);
      lcdPrint("Can on left side");
      PORTC |=(1<<PC4); //set port high
      PORTC&=~ (1<<PC5); // set port low
      delay_ms(500);
      PORTC&=~ (1<<PC4);
    }
    else if (analog(6) > 250)
    {
      PORTC |=(1<<PC4); //set port high
      PORTC |=(1<<PC5); //set port high
      delay_ms(500);
      PORTC&=~ (1<<PC5);
      PORTC&=~ (1<<PC4);
      clrLCD();
     moveLCDCursor(0);
     lcdPrint("Can on right side");
    }

    // stop motor at Tjunction.
    //motor(0,0);
    delay_ms(400);
    clrLCD();
    moveLCDCursor(0);
    //delay_ms(1000);
    lcdPrint("T junction 3");

    //Clear bits/set them low for next time use
    PORTC&=~ (1<<PC4);
    PORTC&=~ (1<<PC5);

    //------------------------------------------------------Wait On Evsheild------------------------------------------------------------
    delay_ms(5000); // wait for EVShield to run
    motor(600,600);
    delay_ms(650);

    //*****************************************Getting to Tjunction 4********************************************************
    // tell evsheild arrived at 4 TJuction
    clrLCD();
    moveLCDCursor(0);
    lcdPrint("T junction 4");
    PORTC |=(1<<PC4); //set port high
    delay_ms(100);
    PORTC&=~ (1<<PC4);

    //------------------------------------------------------Wait On Evsheild------------------------------------------------------------
      delay_ms(6000); // wait for EVShield to run
      motor(600,600);
      delay_ms(650);

    //Clear bits/set them low for next time use
      PORTC&=~ (1<<PC4);
      PORTC&=~ (1<<PC5);


  }
