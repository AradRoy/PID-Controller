
#define LDR A1
#define L1 10
#define L2 11



void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Arduino Started");
  pinMode (LDR , INPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
}

void loop() {

  if (Serial.available() > 0) //whether a serial is recived
  {
    //SERIAL
    char message[200];
    int bytesRead;

    //WORK VALUES
    int state, target, error, interval, duty, interf;
    int LDR_val;
    int L1_val;
    int yArr[201], xArr[201];
    int prevError;
    int Ictrl, Dctrl;
    long int Pctrl;
    long int errorSum = 0, maxErrorSum;
    double dt = 0.001;

    int  kP, kI, kD;

    //TIMES
    unsigned long start_time;

    //COUNTERS
    int i, j;



    /*//////////////////////////KEY////////////////////////////
       TARGET       is the requested value of the LDR sensor - after running test case 2 > max target of our bulb is 583

       SENSOR       is the current value of the LDR sensor

       DUTY         is the value of brightness which the LED is lit

       ERROR        is the current difference between the requested target and the current sensor

       SAMPLE RATE  is the rate of which the time and sensor are recorded to graph later (usualy 100HZ = 10ms)

       CONTROL RATE is the rate of which the sensor value is tested and controler changes the duty of LED

      /////////////////////////////////////////////////////////////////*/


    bytesRead = Serial.readBytesUntil('\0', message, 100);  //reciving data from CVI
    message[bytesRead] = '\0';

    if (message[0] >= '0' && message[0] <= '9')
    {
      sscanf(message, "%d,", &state);   //state = program state
      Serial.print(">>");
      Serial.println(state);

      switch (state)
      {
        //_____________________CASE 0_____________________________
        //////////////////////////////////////////////////////////
        case 0:
          Serial.println(">Case 0 is running");
          digitalWrite(L1, LOW);
          digitalWrite(L2, LOW);
          break;
        //_____________________CASE 1_____________________________
        //////////////////////////////////////////////////////////
        case 1:
          Serial.println(">State 1 running");
          // 1 OFF, 2 OFF
          LDR_val = analogRead(LDR);
          sprintf(message, ">L1:0 L2:0 LDR: %d", LDR_val);
          Serial.println(message);

          // 1 ON, 2 OFF
          digitalWrite(L1, HIGH);
          delay(2000);
          LDR_val = analogRead(LDR);
          sprintf(message, ">L1:255 L2:0 LDR: %d", LDR_val);
          Serial.println(message);

          // 1 OFF, 2 ON
          digitalWrite(L1, LOW);
          digitalWrite(L2, HIGH);
          delay(2000);
          LDR_val = analogRead(LDR);
          sprintf(message, ">L1:0 L2:255 LDR: %d", LDR_val);
          Serial.println(message);

          // 1 ON, 2 ON
          digitalWrite(L1, HIGH);
          delay(2000);
          LDR_val = analogRead(LDR);
          sprintf(message, ">L1:255 L2:255 LDR: %d", LDR_val);
          Serial.println(message);

          // 1 OFF, 2 OF
          digitalWrite(L1, LOW);
          digitalWrite(L2, LOW);
          state = 0;
          Serial.println(">Returning to State 0");
          break;

        //_____________________CASE 2_____________________________
        //////////////////////////////////////////////////////////
        case 2:
          start_time = 0;
          i = 0;
          sscanf(message + 2, "%d,", &duty); //state = program state
          Serial.println(">Case 2 is running");
          //L1_val = map(duty, 0, 255, 0, 255);   //state = duty cycle (declared up top)


          if (duty >= 0 && duty <= 255)
          {
            L1_val = duty;
            analogWrite(L1, L1_val);
            start_time = millis();
            while ((millis() - start_time) <= 2005)
            {
              if ((millis() - start_time) >= i * 10)
              {
                xArr[i] = millis() - start_time;
                yArr[i] = analogRead(LDR);
                i++;
              }
            }

            for (i = 1; i < 201; i++) //send the arrays to PC
            {
              sprintf(message, "%d,%d", xArr[i], yArr[i]);
              Serial.println(message);
            }
            Serial.println("*");

            digitalWrite(L1, LOW);
            digitalWrite(L2, LOW);
            state = 0;
            Serial.println(">Returning to State 0");
          }
          else
          {
            Serial.println(">duty needs to be up to 255");
            state = 0;
          }
          break;

        //_____________________CASE 3_____________________________
        //////////////////////////////////////////////////////////
        case 3:
          Serial.println(">Case 3 is running");

          for (i = 0; i < 10; i++)
          {
            L1_val = 57 + 20 * i ;
            analogWrite(L1, L1_val);
            delay(1000);      //warmup time
            yArr[i] = L1_val;
            xArr[i] = analogRead(LDR);
            digitalWrite(L2, LOW);
            sprintf(message, "%d,%d", xArr[i], yArr[i]); //sending the array during the procces to show that its working
            Serial.println(message);
            delay(1000);              //cooling time
          }
          Serial.println("$");

          digitalWrite(L1, LOW);
          digitalWrite(L2, LOW);
          state = 0;
          Serial.println(">Returning to State 0");
          break;

        //_____________________CASE 4_____________________________
        //////////////////////////////////////////////////////////
        case 4:
          start_time = 0;
          i = 0;
          j = 0;
          // message format: "case , target , control(and sample) rate , duty(fixed) , "duty" for LED2 "

          sscanf(message + 2, "%d,%d,%d, %d", &target, &interval, &duty, &interf); //state = program state, interval = Hz of sample
          Serial.println(">Case 4 is running");

          start_time = millis();
          while ((millis() - start_time) <= 2005)
          {
            analogWrite(L1, interf);
            if ((millis() - start_time) >= i * interval) //for graph plot
            {
              if (analogRead(LDR) <= target) //ON
                analogWrite(L2, duty);

              if (analogRead(LDR) > target) //OFF
                analogWrite(L2, 0);
              i++;
            }

            //Take sample from LDR
            if ((millis() - start_time) >= j * 10) //sample to graph 100Hz
            {
              xArr[j] = millis() - start_time;
              yArr[j] = analogRead(LDR);
              j++;
            }
          }

          //Send data to CVI
          for (i = 1; i < 201; i++)
          {
            sprintf(message, "%d,%d", xArr[i], yArr[i]);
            Serial.println(message);
          }

          Serial.println("*");

          digitalWrite(L1, LOW);
          digitalWrite(L2, LOW);
          state = 0;
          Serial.println(">Returning to State 0");
          break;


        //_____________________CASE 5_____________________________
        //////////////////////////////////////////////////////////
        case 5:

          i = 0;
          j = 0;


          // message format: "case , target , kP , kI , kD "
          //control rate  1000Hz = 1ms = dt
          //sample rate   100Hz = 10ms
          //realistic Kp id about 2 to 10 - about 5 in PI controller
          //realistic Ki id about 8 to 10
          //target max is 1023

          sscanf(message + 2, "%d,%d,%d,%d, %d", &target, &kP, &kI, &kD, &interf);
          maxErrorSum = 255 / (double)(kI * dt); //calculate top boundary for error sum
          prevError = target;

          start_time = millis();
          Serial.println(">Case 5 is running");

          while ((millis() - start_time) <= 2005)
          {
            if ((millis() - start_time) == 1000) //light interference LED2 after 1 second (half of the experiment)
              analogWrite(L1, interf);

            if ((millis() - start_time) >= i) //condition for control rate
            {
              error = target - analogRead(LDR); //max error is: target(initial) , min error is: 0

              /*           P               */
              /*___________________________*/
              Pctrl = (long int)kP * error;

              /*           I               */
              /*___________________________*/
              errorSum += error;
              if (errorSum >= maxErrorSum)
                errorSum = maxErrorSum;
              Ictrl = kI * (dt * errorSum);

              /*           D               */
              /*___________________________*/
              Dctrl = (prevError - error)* kD *(-1);
              prevError = error;

              /*         PID duty          */
              /*___________________________*/
              duty = Pctrl + Ictrl + Dctrl;
              if (duty > 255)
                duty = 255; //full gas
              if (duty < 0)
                duty = 0;   //OFF

              analogWrite(L2, duty);
              i++;
            }

            //Take sample from LDR
            if ((millis() - start_time) >= j * 10) //sample to graph 100Hz
            {
              xArr[j] = millis() - start_time;
              yArr[j] = analogRead(LDR);
              j++;
            }
          }

          //Send data to CVI
          for (j = 1; j < 201; j++)
          {
            sprintf(message, "%d,%d", xArr[j], yArr[j]);
            Serial.println(message);
          }
          Serial.println("*");

          //shut down lights
          digitalWrite(L1, LOW);
          digitalWrite(L2, LOW);
          state = 0;
          Serial.println(">Returning to State 0");
          break;
      }
    }
  }
}
