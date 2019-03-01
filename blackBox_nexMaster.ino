#include "Nextion.h"
#include "NexNumber.h"
#include "NexText.h"
#include "declarations.h"
#include <EEPROMex.h>


/* *******************************************************************

                    buzzer

 *********************************************************************/

void speedRepPushCallBack(void *ptr)
{
  tone (5, 200, 10);
}
void starRepPushCallBack(void *ptr)
{
  tone (5, 200, 10);
}
void bullsEyeRepPushCallBack(void *ptr)
{
  tone (5, 200, 10);
}
void combiRepPushCallBack (void *ptr)
{
  tone (5, 200, 10);
}
void manualRepPushCallBack (void *ptr)
{
  tone (5, 200, 10);
}
void curingUVPushCallBack (void *ptr)
{
  tone (5, 200, 10);
}
void setUpPushCallBack (void *ptr)
{
  tone (5, 200, 10);
}
void langueBtnPushCallBack (void *ptr)
{
  tone (5, 200, 10);
}
void batBtnPushCallBack (void *ptr)
{
  tone (5, 200, 10);
}
void calibrationBtnPushCallBack (void *ptr)
{
  tone (5, 200, 10);
}
void infosBtnPushCallBack (void *ptr)
{
  tone (5, 200, 10);
}
void departAutoPushCallBack (void *ptr)
{
  tone (5, 200, 10);
}
void arretAutoPushCallBack (void *ptr)
{
  tone (5, 200, 10);
}
void sortieAutoPushCallBack (void *ptr)
{
  tone (5, 200, 10);
}

/* *******************************************************************

                      Infos
 *********************************************************************/


void infosPopCallBack(void *ptr)
{

  int number;
  char serialNumber[] = "     ";
  EEPROM.readBlock<char>(0, serialNumber, 5);
  NexText(10, 5, "serial").setText(serialNumber);
  Serial.println(serialNumber);
  
  number = EEPROM.readInt(20);
  NexNumber(10, 3, "repairs").setValue (number);
  Serial.println(number);


}

/* *******************************************************************

                      Autonomie batterie

 *********************************************************************/


void turnOffPopCallBack(void *ptr)
{

  uint32_t number;
  dbSerialPrintln("turnOffPopCallBack");
  offSlider.getValue(number);
  tone (5, 1200, 500);
  delay (500);
  digitalWrite (6, HIGH);


}


/* *******************************************************************

                      Autonomie batterie

 *********************************************************************/
unsigned int analogReadReference(void) {

  /* Elimine toutes charges résiduelles */
#if defined(__AVR_ATmega328P__)
  ADMUX = 0x4F;
#elif defined(__AVR_ATmega2560__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5F;
#elif defined(__AVR_ATmega32U4__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5F;
#endif
  delayMicroseconds(5);

  /* Sélectionne la référence interne à 1.1 volts comme point de mesure, avec comme limite haute VCC */
#if defined(__AVR_ATmega328P__)
  ADMUX = 0x4E;
#elif defined(__AVR_ATmega2560__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5E;
#elif defined(__AVR_ATmega32U4__)
  ADCSRB &= ~(1 << MUX5);
  ADMUX = 0x5E;
#endif
  delayMicroseconds(200);

  /* Active le convertisseur analogique -> numérique */
  ADCSRA |= (1 << ADEN);

  /* Lance une conversion analogique -> numérique */
  ADCSRA |= (1 << ADSC);

  /* Attend la fin de la conversion */
  while (ADCSRA & (1 << ADSC));

  /* Récupère le résultat de la conversion */
  return ADCL | (ADCH << 8);
}

void batteryLevelPopCallBack (void *ptr)
{
  checkBatteryLevel();
}
void checkBatteryLevel ()
{
  const byte BROCHE_CAPTEUR_VIN = A1;

  /* Coefficient diviseur du pont de résistance */
  const float COEFF_PONT_DIVISEUR_VIN = 3;
  unsigned int raw_vin = analogRead(BROCHE_CAPTEUR_VIN);
  unsigned int raw_ref = analogReadReference();

  /* Calcul de la tension réel avec un produit en croix */
  real_vin = ((raw_vin * 1.1) / raw_ref) * COEFF_PONT_DIVISEUR_VIN;

  /* Affichage */

  unsigned int pourcent = (real_vin - 12) / 0.02;
  if (real_vin >= 14) {
    pourcent = 100;
  } else if  (real_vin < 12) {
    pourcent = 0;
  }
  batProgressBar.setValue (pourcent);
  batPercent.setValue(pourcent);
  // return (batteryVoltage);
  delay(1000);

}

void batteryOk ()
{
  checkBatteryLevel ();

  if (real_vin < 12.4)
  {
    checkAlim();
    if  (alimState == 0)
    {
      batteryState = 0;
      NexPage (14, 0, "page0").show();
      //  return 0;
    } else {
      batteryState = 1;
    }
  } else if (real_vin >= 12.4)
  {
    batteryState = 1;
    //return 1;
  }
}

/* *******************************************************************

                      Check alimentation & charger

 *********************************************************************/

void checkAlim ()
{
  if (digitalRead (18) == LOW)
  {
    alimState = 1;
  } else {
    alimState = 0;
  }
}

/* *******************************************************************

                      Calibrage

 *********************************************************************/

void calibrationPopCallBack (void *ptr)
{

  // checkAlim ();
  dbSerialPrintln("calibrationPopCallBack");
  // currentMillis = millis();
  // previousMillis = 0;
  atm = 0;
  valnum = 0;
  int i = 1;
  while ( i <= 10 )
  {

    // MEASUREMENT
    int pressureSensorRaw = analogRead(A0); //Reads the sensor raw value on analog port 0
    float pressureSensorVoltage = pressureSensorRaw * (5.0 / 1023.0);  // convert the raw reading to voltage
    mBar = int (pressureSensorVoltage + 0.2) * 7000 / 4.5;
    valnum = mBar;
    itoa(valnum, val, 10);
    NexText(7, 8, "gaugeDigiCal").setText(val);
    NexGauge (7, 5, "calibrateGauge").setValue(map(mBar, -1000, 3000, 0, 216));
    NexProgressBar (7, 4, "progressBarCal").setValue(map((i), 0, 10, 0, 100));

    atm = atm + mBar;
    delay(1000);
    i += 1;
  }
  atm = atm / 10;
  EEPROM.updateInt(10, atm);
  Serial.println(atm);
  //atmMemory.setValue(atm);
  NexGauge (7, 5, "calibrateGauge").setValue(map((mBar - atm), -1000, 3000, 0, 216));
  valnum = mBar - atm;
  itoa(valnum, val, 10);
  NexText(7, 8, "gaugeDigiCal").setText(val);

}


/* *******************************************************************

                      Stop button

 *********************************************************************/
bool arretAutoPopCallBack (void *ptr)
{
  NexButton *btn = (NexButton *)ptr;
  dbSerialPrintln("arretAutoPopCallback");
  repStarted = 0;
  finRep();
  return repStarted;
  //  menuPage.show();
}

/* *******************************************************************

                    Speed Rep

 *********************************************************************/

void speedRepPopCallback(void *ptr)
{
  dbSerialPrintln("speedRepPopCallback");

  batteryOk();
  //  checkAlim ();
  if (batteryState = 1) {
    Cycle_1     = 60;
    Cycle_2     = 120;
    Cycle_3     = 60;
    Cycle_4     = 120;


    NexProgressBar (2, 5, "progressCycle").setValue (0);
    NexProgressBar (2, 6, "progressRep").setValue (0);


    valnum = pressureSensorRead ();
    itoa(valnum, val, 10);


    NexGauge (2, 2, "gaugeAnalog").setValue(map(pressureSensorRead (), -1000, 3000, 0, 216));

    NexNumber (2, 18, "n8").setValue(CycleTotal % 60);
    NexNumber (2, 17, "n7").setValue(CycleTotal / 60);
    NexText(2, 27, "gaugeDigiRep").setText(val);
  }
}

/* *******************************************************************

                     Star Rep

 *********************************************************************/

void starRepPopCallBack (void *ptr)
{
  //  dbSerialPrintln("starRepPopCallBack");
  // checkAlim ();
  batteryOk();
  if (batteryState = 1) {
    Cycle_1     = 120;
    Cycle_2     = 180;
    Cycle_3     = 120;
    Cycle_4     = 180;

    NexProgressBar (2, 5, "progressCycle").setValue (0);
    NexProgressBar (2, 6, "progressRep").setValue (0);


    valnum = pressureSensorRead ();
    itoa(valnum, val, 10);

    NexText(2, 27, "gaugeDigiRep").setText(val);
    NexGauge (2, 2, "gaugeAnalog").setValue(map(pressureSensorRead (), -1000, 3000, 0, 216));


    NexNumber (2, 18, "n8").setValue(CycleTotal % 60);
    NexNumber (2, 17, "n7").setValue(CycleTotal / 60);
    NexText(2, 27, "gaugeDigiRep").setText(val);
  } /*else {
    NexPage (14, 0, "page0").show();

  }*/


}

/* *******************************************************************

                      Bullseye Rep

 *********************************************************************/

void bullsEyeRepPopCallBack (void *ptr)
{
  dbSerialPrintln("bullsEyeRepPopCallBack");
  // checkAlim ();
  batteryOk();
  if (batteryState = 1) {
    Cycle_1     = 120;
    Cycle_2     = 120;
    Cycle_3     = 120;
    Cycle_4     = 120;

    NexProgressBar (2, 5, "progressCycle").setValue (0);
    NexProgressBar (2, 6, "progressRep").setValue (0);


    NexNumber (2, 18, "n8").setValue(CycleTotal % 60);
    NexNumber (2, 17, "n7").setValue(CycleTotal / 60);


    NexGauge (2, 2, "gaugeAnalog").setValue(map(pressureSensorRead (), -1000, 3000, 0, 216));
    valnum = pressureSensorRead ();
    itoa(valnum, val, 10);
    NexText(2, 27, "gaugeDigiRep").setText(val);
  }
}


/* *******************************************************************

                      combinatio  Rep

 *********************************************************************/

void combiRepPopCallBack (void *ptr)
{
  dbSerialPrintln("combiRepPopCallBack");
  // checkAlim ();
  batteryOk();
  if (batteryState = 1) {
    Cycle_1     = 180;
    Cycle_2     = 180;
    Cycle_3     = 120;
    Cycle_4     = 240;

    NexProgressBar (2, 5, "progressCycle").setValue (0);
    NexProgressBar (2, 6, "progressRep").setValue (0);


    valnum = pressureSensorRead ();
    itoa(valnum, val, 10);

    NexText(2, 27, "gaugeDigiRep").setText(val);
    NexGauge (2, 2, "gaugeAnalog").setValue(map(pressureSensorRead (), -1000, 3000, 0, 216));


    NexNumber (2, 18, "n8").setValue(CycleTotal % 60);
    NexNumber (2, 17, "n7").setValue(CycleTotal / 60);
    NexText(2, 27, "gaugeDigiRep").setText(val);
  }
}
/* *******************************************************************

                      manual Rep

 *********************************************************************/

void manualRepPopCallBack (void *ptr)
{
  dbSerialPrintln("manualRepPopCallBack");
  batteryOk();
  if (batteryState = 1) {
    valnum = pressureSensorRead ();
    itoa(valnum, val, 10);

    NexText(9, 14, "gaugeDigiMan").setText(val);
    NexGauge  (9, 2, "gaugeAnaMan").setValue(map(pressureSensorRead (), -1000, 3000, 0, 216));

    Pression_1    = -800;
    Pression_2  = 3100;

    // checkAlim ();
  }
}

/* *******************************************************************

                      manual rep pressure

 *********************************************************************/

bool pressureManuPopCallBack (void *ptr)
{
  int elapsedTime;
  dbSerialPrintln ("pressureManuPopCallBack");
  NexNumber (9, 6, "n1").setValue (0);
  NexNumber (9, 7, "n2").setValue (0);
  bool pressureOk = 0;
  leakTime = 0;

  while (pressureOk != 1)
  {
    pressureOk = checkPressure ( Pression_2);
    valnum = pressureSensorRead ();
    itoa(valnum, val, 10);
    NexText(9, 14, "gaugeDigiMan").setText(val);
    NexGauge  (9, 2, "gaugeAnaMan").setValue(map(pressureSensorRead (), -1000, 3000, 0, 216));


  }
  NexText(9, 14, "gaugeDigiMan").setText(val);
}

/* *******************************************************************

                     manual rep vacuum

 *********************************************************************/

void vacuumManuPopCallBack (void *ptr)
{
  dbSerialPrintln ("vacuumManuPopCallBack");
  int elapsedTime;
  NexNumber (9, 6, "n1").setValue (0);
  NexNumber (9, 7, "n2").setValue (0);
  pressureOk = 0;
  leakTime = 0;

  while (pressureOk != 1)
  {
    pressureOk = checkPressure ( Pression_1);
    valnum = pressureSensorRead ();
    itoa(valnum, val, 10);

    NexText(9, 14, "gaugeDigiMan").setText(val);
    NexGauge  (9, 2, "gaugeAnaMan").setValue(map(pressureSensorRead (), -1000, 3000, 0, 216));
  }
  NexText(9, 14, "gaugeDigiMan").setText(val);
}

//**********************************************************

void exhaustAutoPopCallBack (void *ptr)
{

}

/* *******************************************************************

                     UV curing time plus

 *********************************************************************/

void plusUVPopCallBack (void *ptr)
{
  curingTime += 30;

  NexNumber (8, 9, "n0").setValue ( curingTime % 60 );
  NexNumber (8, 9, "n1").setValue ( curingTime / 60 );

}

/* *******************************************************************

                      UV curing time minus

 *********************************************************************/

void minusUVPopCallBack (void *ptr)
{
  if ( curingTime >= 30)
  {
    curingTime -= 30;

    NexNumber (8, 9, "n0").setValue ( curingTime % 60 );
    NexNumber (8, 9, "n1").setValue ( curingTime / 60 );
  }
}

/* *******************************************************************

                      UV curing start

 *********************************************************************/
void startUVPopCallBack (void *ptr)

{

  flag = 1;
  //  unsigned long currentMillis = millis();
  // unsigned long previousMillis = millis();
  digitalWrite (7, HIGH);

  while ( curingTime > 0 )
  {
    currentMillis = millis();
    nexLoop(nex_listen_list);

    if ( (currentMillis - previousMillis) >= 1000 )
    {
      if ( flag == 0)
      {
        break;
      }
      previousMillis = currentMillis;
      curingTime -= 1;
      NexNumber (8, 9, "n0").setValue ( curingTime % 60 );
      NexNumber (8, 9, "n1").setValue ( curingTime / 60 );

    }
  }
  NexCrop(8, 7, "q2").setPic(35);
  digitalWrite (7, LOW);
  tone (5, 1200, 1500);
  nexLoop(nex_listen_list);
}

/* *******************************************************************

                      UV curing stop

 *********************************************************************/

void stopUVPopCallBack (void *ptr)
{
  flag = 0;
  digitalWrite (7, LOW);
}

/* *******************************************************************

                      UV curing page

 *********************************************************************/

void exitUVPopCallBack (void *ptr)
{
  flag = 0;
  digitalWrite (7, LOW);
}

/* *******************************************************************

                      UV curing exit

 *********************************************************************/

void curingUVPopCallBack (void *ptr)
{
  dbSerialPrintln("curingUVPopCallBack");
  batteryOk();
  if (batteryState = 1) {
    curingTime = 0;
    // checkAlim ();
  }
}

void setUpPopCallBack (void *ptr)
{
  dbSerialPrintln("setUpRepPopCallBack");

}



/* *******************************************************************

                     Read pressure

 *********************************************************************/

int pressureSensorRead ()
{
  atm = EEPROM.readInt(10);
  int pressureSensorRaw = analogRead(A0); //Reads the sensor raw value on analog port 0
  float pressureSensorVoltage = pressureSensorRaw * (5.0 / 1023.0);  // convert the raw reading to voltage
  int32_t mBar = (pressureSensorVoltage + 0.2) * 7000 / 4.5;

  mBar = mBar - atm;

  return  mBar;
  delay(1);
}

/* *******************************************************************

                     depart Auto

 *********************************************************************/

void departAutoPopCallBack (void *ptr)
{
  dbSerialPrintln("departAutoPopCallBack");
  repStarted = 1;
  CycleTotal  = Cycle_1 + Cycle_2 + Cycle_3 + Cycle_4;

  if (repStarted != 0)
  {
    leakTime = 0;
    MiseEnPression ( Pression_1, Cycle_1, 0 ); // -800
    leakTime = 0;
    MiseEnPression ( Pression_2, Cycle_2, Cycle_1 ); // 2500
    leakTime = 0;
    MiseEnPression ( Pression_3, Cycle_3, Cycle_1 + Cycle_2 ); // -800
    leakTime = 0;
    MiseEnPression ( Pression_4, Cycle_4, Cycle_1 + Cycle_2 + Cycle_3 ); // 3000
    repairNumberInc();
  }
  digitalWrite(valvePressure, LOW); // Electrovanne 1 Led verte Pression
  digitalWrite(valveVacuum, LOW); // Electrovanne 2 Led bleu Depression
  digitalWrite(pump, LOW);
  tone (5, 1200, 1500);



}

/* *******************************************************************

                     Automatic repair

 *********************************************************************/

bool MiseEnPression ( int Pressure, long Cycle,  long etape )
{
  // checkAlim ();
  //  unsigned long currentMillis = millis();
  //  unsigned long previousMillis = millis();
  //   int previousPressure = 0;
  int repTime = CycleTotal;
  int cycleCounter = 0;
  int repCounter = etape;
  String displaytime;

  cycleCounter      = 0;
  repCounter        = etape;
  //    previousPressure  = Pressure;
  currentMillis     = millis();
  valnum = pressureSensorRead ();
  itoa(valnum, val, 10);

  NexText(2, 27, "gaugeDigiRep").setText(val);

  if (repStarted != 0)
  {
    // On récupère la valeur du capteur de pression


    NexNumber (2, 14, "n4").setValue(Cycle % 60);
    NexNumber (2, 13, "n3").setValue(Cycle / 60);

    NexNumber (2, 18, "n8").setValue(CycleTotal % 60);
    NexNumber (2, 17, "n7").setValue(CycleTotal / 60);

    // On attend que la pression soit atteint avant de lancer la temporisation
    bool pressureOk = 0;
    while (pressureOk != 1)
    {
      pressureOk = checkPressure ( Pressure);
      valnum = pressureSensorRead ();
      itoa(valnum, val, 10);

      NexGauge (2, 2, "gaugeAnalog").setValue(map(pressureSensorRead (), -1000, 3000, 0, 216));

      NexText(2, 27, "gaugeDigiRep").setText(val);
    }
    // On commence le cycle de réparation

    while ( cycleCounter <= (Cycle - 1))
    {
      currentMillis = millis();
      nexLoop(nex_listen_list);

      //        repTime = CycleTotal;
      if ( (currentMillis - previousMillis) >= 1000 )
      {
        if ( repStarted == 0)
        {
          break;
        }
        previousMillis = currentMillis;
        cycleCounter += 1;
        repCounter += 1;

        NexNumber (2, 12, "n2").setValue (cycleCounter % 60);
        NexNumber (2, 16, "n6").setValue (repCounter % 60);
        NexNumber (2, 11, "n1").setValue (cycleCounter / 60);
        NexNumber (2, 15, "n5").setValue (repCounter / 60);

        NexProgressBar (2, 5, "progressCycle").setValue(map((cycleCounter), 0, Cycle, 0, 100));
        NexProgressBar (2, 6, "progressRep").setValue(map((repCounter), 0, repTime, 0, 100));

        valnum = pressureSensorRead ();
        itoa(valnum, val, 10);
        if (abs(valnum - Pressure) > 100) {

          NexGauge (2, 2, "gaugeAnalog").setValue(map(pressureSensorRead (), -1000, 3000, 0, 216));

          NexText(2, 27, "gaugeDigiRep").setText(val);

        }

      }
    }

    nexLoop(nex_listen_list);
  }

  finRep();

}


/* *******************************************************************

                     fin reparation

 *********************************************************************/

void finRep ()
{
  digitalWrite(valvePressure, LOW);
  digitalWrite(valveVacuum, LOW);
  digitalWrite(pump, LOW);

}

/* *******************************************************************

                     Check Pressure

 *********************************************************************/

bool checkPressure ( int targetPressure )
{
  int currentPressure = pressureSensorRead ();
  targetPressure = map( targetPressure, -1000, 3000, 0, 4000 );
  currentPressure = map( currentPressure, -1000, 3000, 0, 4000 );
  int Tolerance = 50;



  currentWorkTime = millis();
  // bool leakAlarm =  pressureLeak ( );

  // On va à fond vers notre objectif de pression


  if (( currentPressure < targetPressure - Tolerance  ) && ( leakTime <= 5))
  {
    digitalWrite(pump, HIGH);
    digitalWrite(valvePressure, HIGH);
    digitalWrite(valveVacuum, LOW);
    if ( currentWorkTime - previousWorkTime >= 1000)
    {
      previousWorkTime = currentWorkTime;
      leakTime += 1;
    }

    return 0;
  }
  else if (( currentPressure  > targetPressure + Tolerance  ) && ( leakTime <= 5))
  {
    digitalWrite(pump, HIGH);
    digitalWrite(valvePressure, LOW);
    digitalWrite(valveVacuum, HIGH);
    if ( currentWorkTime - previousWorkTime >= 1000)
    {
      previousWorkTime = currentWorkTime;
      leakTime += 1;
    }

    return 0;
  }
  // On stoppe si on a atteint notre pression recherché avec +ou- 50 de tolérance
  else if ( ( targetPressure - Tolerance < currentPressure < targetPressure + Tolerance ) || ( leakTime > 5) )
  {
    digitalWrite(pump, LOW);
    digitalWrite(valveVacuum, LOW);
    digitalWrite(valvePressure, LOW);
    if (leakTime > 5)
    {
      leakTime = 0;
      repStarted = 0;
      pagePressureLeak.show();
      tone (5, 1200, 1500);
    }
    return 1;
  }

  delay(1);
}


/* *******************************************************************

                      pressure Leak

 *********************************************************************/
bool pressureLeak ()
{
  unsigned long currentWorkTime = millis();
  unsigned long previousWorkTime = millis();
  currentWorkTime = millis();
  int leakTime = 0;
  if ( currentWorkTime - previousWorkTime >= 1000)
  {
    previousWorkTime = currentWorkTime;
    leakTime += 1;
    Serial.println(leakTime);
  }
  if (leakTime > 3)
  {
    digitalWrite(pump, LOW);
    digitalWrite(valvePressure, LOW);
    digitalWrite(valveVacuum, LOW);
    tone (5, 800, 2000);
    return 1;
  } else
  {
    return 0;
  }
}

/* *******************************************************************

                     repair Number Incrementation

 *********************************************************************/

void repairNumberInc ()
{
  repNumber = 0;
  repNumber = EEPROM.readInt(20);
  repNumber += 1;
  EEPROM.writeInt(20, repNumber);

}

/* *******************************************************************

                     Setup

 *********************************************************************/

void setup() {

  tone (5, 1200, 500);
  checkBatteryLevel ();

  pinMode (valvePressure, OUTPUT);
  pinMode (valveVacuum, OUTPUT);
  pinMode (6, OUTPUT);
  pinMode (pump, OUTPUT);
  pinMode (7, OUTPUT);
  pinMode (5, OUTPUT);
  pinMode (A0, INPUT);
  pinMode (A1, INPUT);
  pinMode (17, INPUT);
  pinMode (18, INPUT);



  digitalWrite (valvePressure, LOW);
  digitalWrite (valveVacuum, LOW);
  digitalWrite (6, LOW);
  digitalWrite (pump, LOW);
  digitalWrite (7, LOW);

  // checkAlim ();

  nexInit();



  /* Register the pop event callback function of the current button component. */

  batBtn.attachPop (batteryLevelPopCallBack);
  offSlider.attachPop (turnOffPopCallBack);
  calibrateStart.attachPop (calibrationPopCallBack);
  speedRep.attachPop (speedRepPopCallback);
  departAuto.attachPop (departAutoPopCallBack);
  arretAuto.attachPop (arretAutoPopCallBack);
  starRep.attachPop (starRepPopCallBack);
  bullsEyeRep.attachPop (bullsEyeRepPopCallBack);
  combiRep.attachPop (combiRepPopCallBack);
  manualRep.attachPop (manualRepPopCallBack);
  curingUV.attachPop (curingUVPopCallBack);
  pressureManu.attachPop (pressureManuPopCallBack);
  vacuumManu.attachPop (vacuumManuPopCallBack);
  plusUV.attachPop (plusUVPopCallBack);
  minusUV.attachPop (minusUVPopCallBack);
  startUV.attachPop (startUVPopCallBack);
  stopUV.attachPop (stopUVPopCallBack);
  exitUV.attachPop (exitUVPopCallBack);
  infosBtn.attachPop (infosPopCallBack);

  speedRep.attachPush (speedRepPushCallBack);
  starRep.attachPush (starRepPushCallBack);
  bullsEyeRep.attachPush (bullsEyeRepPushCallBack);
  combiRep.attachPush (combiRepPushCallBack);
  manualRep.attachPush (manualRepPushCallBack);
  curingUV.attachPush (curingUVPushCallBack);
  setUp.attachPush (setUpPushCallBack);
  langueBtn.attachPush (langueBtnPushCallBack);
  batBtn.attachPush (batBtnPushCallBack);
  calibrationBtn.attachPush (calibrationBtnPushCallBack);
  infosBtn.attachPush (infosBtnPushCallBack);
  departAuto.attachPush (departAutoPushCallBack);
  arretAuto.attachPush (arretAutoPushCallBack);
  sortieAuto.attachPush (sortieAutoPushCallBack);

  dbSerialPrintln ("setup done");

}

void loop()
{
  nexLoop(nex_listen_list);

}
