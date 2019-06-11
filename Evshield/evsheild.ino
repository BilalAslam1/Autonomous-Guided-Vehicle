  // Mechbot#7
  // Evshield#9
  // Group 10
  // Bilal Aslam
  #include <Wire.h>
  #include <EVShield.h>
  #include <EVs_NXTCam.h>
  #include <USART.h>
  #define color =0;
  #define counter = 0;
  EVShield evshield(0x34, 0x36);
  EVs_NXTCam myCam;
  // global counters
  //int counter = 0;
  //int red =0;
  //int blue=0;
  //int color=0; // 2 is red, 3 is blue

  // declare varibales

  int alpha =1; // 1 = red blue, 2 = blue red

  void setup()
  {
    initUSART();

    // initilizize inputs and set ports low
    DDRB&=~((1<<PB1)|(1<<PB0));
    PORTB&=~((1<<PB1)|(1<<PB1));

    evshield.init( SH_HardwareI2C );
    myCam.init( &evshield, SH_BAS1 );

    myCam.disableTracking();
    myCam.selectObjectMode();
    myCam.sortSize();
    myCam.enableTracking();
    delay(1000);

    evsheild.bank.a.motorReset();
  }
  int Camera ()
  {
      int nblobs;
      uint8_t color[8];
      uint8_t left[8];
      uint8_t top[8];
      uint8_t bottom[8];
      uint8_t right[8];
      uint8_t i;


      myCam.issueCommand('J'); // lock buffer
      delay(500);
      myCam.getBlobs(&nblobs, color, left, top, right, bottom);
      delay(500);
      myCam.issueCommand('K'); // unlock buffer

      printString("number of blobs: ");
      printWord(nblobs);
      printString("\r\n");
      for (int i= 0; i< nblobs; i++)
      {
        if(color[i]==2) // red
        {
          //red = red + 1;
          return 2;
          evsheild.bank_a.ledSetRGB(255,0,0);
        }
        if(color[i]==3) //blue
        {
          //blue = blue + 1;
          return 3;
          evsheild.bank_a.ledSetRGB(0,0,255);
        }
      }

    /*  if (blue > red)
      {
        blue = 0;
        red = 0;
        return 3;

      }
      else if (red >blue)
      {
        blue=0;
        red=0;
        return 2;
      }
      */
  }
  void loop()
  {


  while (1)
  {
    // use !() condition for low ports
    // motor 1 grabbing can motor
    // motor 2 rotating arm motor
    if (PINB & (1<<PB0))
    {
              // can arrived at picking up Tjunction
              if (counter == 0 || counter == 2)
              {
                  counter = counter + 1;

                  if (counter > 3) // make sure counter does not exceed 3 incase of errors just in case
                  {
                  counter = 0;
                  }

                  if ((PINB & (1<<PB1)) && (PINB &(1<<PB0)))
                  {
                    // can on right side
                    //start arm in 120 degress position
                    //evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Reverse, int 10, 120,  SH_Completion_Wait_For, SH_Next_Action_BrakeHold);
                    evshield.bank_a.motorRunDegrees(SH_Motor_2, SH_Direction_Forward, 1, 90,  SH_Completion_Wait_For, SH_Next_Action_BrakeHold);
                    evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Forward, 1, 160,  SH_Completion_Dont_Wait, SH_Next_Action_BrakeHold);
                    //delay (1000);
                    //talha didn't allow it
                    //evshield.bank_a.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, 5);
                    // clamp can
                    //delay(500);
                    evshield.bank_a.motorRunDegrees(SH_Motor_2, SH_Direction_Reverse,  1, 90,  SH_Completion_Wait_For, SH_Next_Action_BrakeHold);
                    break;
                  }

                  else if(PINB &(1<<PB0))// can on left side
                  {
                    //start arm in 120 degress position
                    //  evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Reverse, int 10, 120,  SH_Completion_Wait_For, SH_Next_Action_BrakeHold);
                    evshield.bank_a.motorRunDegrees(SH_Motor_2, SH_Direction_Reverse, 1, 90,  SH_Completion_Wait_For, SH_Next_Action_BrakeHold);
                    evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Forward, 1, 160,  SH_Completion_Dont_Wait, SH_Next_Action_BrakeHold);
                    // clamp can
                    //delay(1000);
                    evshield.bank_a.motorRunDegrees(SH_Motor_2, SH_Direction_Forward, 1, 90,  SH_Completion_Wait_For, SH_Next_Action_BrakeHold);
                    break;
                  }

                  //delay (3000);


                  color = Camera(); // get can color
            }

            // dropping can
            if (counter==1 || counter == 3)
            {
              counter = counter + 1;
              if(alpha == 1) // red
              {
                // if holding red can
                evshield.bank_a.motorRunDegrees(SH_Motor_2, SH_Direction_Forward, 1, 90,  SH_Completion_Dont_Wait, SH_Next_Action_BrakeHold);
                evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Reverse, 1, 160,  SH_Completion_Dont_Wait, SH_Next_Action_BrakeHold);
                evshield.bank_a.motorRunDegrees(SH_Motor_2, SH_Direction_Reverse,  1, 90,  SH_Completion_Wait_For, SH_Next_Action_BrakeHold);
                alpha = 2;
                break;
              }

              if(alpha ==2)  // blue

              {
                evshield.bank_a.motorRunDegrees(SH_Motor_2, SH_Direction_Reverse, 1, 90,  SH_Completion_Dont_Wait, SH_Next_Action_BrakeHold);
                evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Reverse, 1, 160,  SH_Completion_Dont_Wait, SH_Next_Action_BrakeHold);
                evshield.bank_a.motorRunDegrees(SH_Motor_2, SH_Direction_Forward,  1, 90,  SH_Completion_Wait_For, SH_Next_Action_BrakeHold);
                alpha = 1;
                break;
              }

            }
      }
  }
}
