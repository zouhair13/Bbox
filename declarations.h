/******************************************************************************************************

        Page Welcome

 ******************************************************************************************************/
//NexButton welcomeExit = NexButton(0, 4, "b0");
//NexText serial        = NexText (10, 4, "serial");
//NexNumber repairs     = NexNumber (10, 3, "repairs");
/******************************************************************************************************

        Page Menu

 ******************************************************************************************************/
/*
  NexNumber jus         = NexNumber(0, 5, "n0");
  NexVar varPlug        = NexVar(0, 5, "plug");
  NexPage menuPage      = NexPage(1, 0, "Menu");
*/
NexButton speedRep    = NexButton(1, 3, "fast");
NexButton starRep     = NexButton(1, 4, "star");
NexButton bullsEyeRep = NexButton(1, 5, "bullseye");
NexButton combiRep    = NexButton(1, 6, "combination");
NexButton manualRep   = NexButton(1, 7, "manual");
NexButton curingUV    = NexButton(1, 8, "uvCuring");
NexButton setUp       = NexButton(1, 9, "setup");
NexButton menuExit    = NexButton(1, 2, "setup");

/******************************************************************************************************

        Page Reparation Automatique

 ******************************************************************************************************/

NexButton departAuto                = NexButton (2, 18, "Start");
NexButton arretAuto                 = NexButton (2, 19, "Stop");
NexButton sortieAuto                = NexButton (2, 3, "Exit");
/*
  NexButton exhaustAuto               = NexButton (2, 22, "Exhaust");
  NexProgressBar cycleProgressBar     = NexProgressBar (2, 5, "progressCycle");
  NexProgressBar totalTimeProgressBar = NexProgressBar (2, 6, "progressRep");
  NexGauge gaugeAnalogAuto            = NexGauge (2, 2, "gaugeAnalog");
  NexNumber gaugeDigitalAuto          = NexNumber (2, 4, "gaugeDigital");
  NexNumber MinCurrentCycle           = NexNumber (2, 11, "n1");
  NexNumber SecCurrentCycle           = NexNumber (2, 12, "n2");
  NexNumber MinTargetCycle            = NexNumber (2, 13, "n3");
  NexNumber SecTargetCycle            = NexNumber (2, 14, "n4");
  NexNumber MinCurrentRep             = NexNumber (2, 15, "n5");
  NexNumber SecCurrentRep             = NexNumber (2, 16, "n6");
  NexNumber MinTargetRep              = NexNumber (2, 17, "n7");
  NexNumber SecTargetRep              = NexNumber (2, 18, "n8");
*/
NexText t4                          = NexText (2, 25, "t4");

/******************************************************************************************************

        Page Reparation Manuelle

 ******************************************************************************************************/


NexButton exitManu                  = NexButton (9, 3, "Exit");
NexButton vacuumManu                = NexButton (9, 8, "vacuum");
NexButton pressureManu              = NexButton (9, 9, "pressure");
/*
  NexGauge gaugeAnalogManu            = NexGauge  (9, 2, "gaugeAnaMan");
  NexNumber gaugeDigitalManu          = NexNumber (9, 2, "gaugeDigiMan");
  NexNumber minElapsedTime             = NexNumber (9, 6, "n1");
  NexNumber secElapsedTime             = NexNumber (9, 7, "n2");
  NexText t0                            = NexText (9, 0, "t0");
*/

/******************************************************************************************************

        Page Niveau battery

 ******************************************************************************************************/

NexProgressBar batProgressBar = NexProgressBar(5, 1, "progressBat");
NexNumber batPercent          = NexNumber(5, 3, "batLevelAnalog");

/******************************************************************************************************

        Page Reglages

 ******************************************************************************************************/

NexButton langueBtn      = NexButton(3, 2, "languages");
NexButton batBtn         = NexButton(3, 3, "batLevel");
NexButton calibrationBtn = NexButton(3, 4, "calibration");
NexButton infosBtn       = NexButton(3, 5, "infos");
NexButton setupExit      = NexButton(3, 6, "setup");

/******************************************************************************************************

        Page Calibration

 ******************************************************************************************************/
NexNumber atmMemory             = NexNumber(7, 11, "atm");
NexButton calibrateStart        = NexButton(7, 3, "calibrateStart");
NexButton calibrateExit         = NexButton(7, 2, "calibrateExit");
/*
  NexGauge gaugeAnalogCalibrate   = NexGauge (7, 6, "calibrateGauge");
  NexNumber gaugeDigitalCalibrate = NexNumber (7, 8, "calibreDigital");
*/
/******************************************************************************************************

        Page UV Curing

 ******************************************************************************************************/

NexButton plusUV               = NexButton(8, 6, "plusUV");
NexButton minusUV              = NexButton(8, 5, "minusUV");
NexButton startUV              = NexButton(8, 3, "startUV");
NexButton stopUV               = NexButton(8, 4, "stopUV");
NexButton exitUV               = NexButton(8, 2, "exitUV");
/*
  NexNumber secUV                = NexNumber (8, 3, "n0");
  NexNumber minUV                = NexNumber (8, 9, "n1");
  NexCrop lightUV  = NexCrop(8, 7, "q0");
*/

/******************************************************************************************************

        Page Extinction

 ******************************************************************************************************/

NexSlider offSlider = NexSlider(6, 1, "h0");

NexPage pagePressureLeak    = NexPage (13, 0, "PressureLeak");
NexButton exitLeak  = NexButton(13, 1, "PressureLeak");

/******************************************************************************************************

       Déclarations

******************************************************************************************************/

const int valvePressure     = 3; //  Pression
const int valveVacuum       = 2; //  Depression
const int pump              = 4; //  Pump 
/*
  const int pinOff            = 6; // Extinction
  const int pinUV             = 7; // Lampe UV
  const int pinBuzzer         = 5; // Buzzer
  const int pressureSensorPin = A0; // Capteur Pression sur A0
  const int BatLevelPin       = A1; // Capteur niveau batterie
  const int chargerPin        = A3; // detection chargeur
  const int alimPin           = A4; // detection alim 12v Extérieure
*/


int Cycle_1;
int Cycle_2;
int Cycle_3;
int Cycle_4;
int CycleTotal;

int Pression_1  = -800;
int Pression_2  = 3100;
int Pression_3  = -800;
int Pression_4  = 3100;


int atm;
int mBar;

bool pressureOk;
bool repStarted = 0;
bool flag;
int curingTime;
unsigned long previousMillis;
bool batteryChargeOk;
unsigned long currentMillis;

char val[4] = {0};
int valnum;
int leakTime = 0;
int repNumber;
unsigned long currentWorkTime = millis();
unsigned long previousWorkTime = millis();

float real_vin;
bool batteryState;
bool alimState;
char buffer[100] = {0};

NexTouch *nex_listen_list[] =
{
  &speedRep, &starRep, &bullsEyeRep, &combiRep, &manualRep, &curingUV, &setUp,

  &exitManu, &pressureManu, &vacuumManu,

  &departAuto, &arretAuto, &sortieAuto,

  &plusUV, &minusUV, &startUV, &stopUV, &exitUV,

  &langueBtn, &batBtn, &infosBtn, &calibrateStart, &calibrateExit,

  &offSlider,

  &exitLeak,



  NULL
};
