// if CAN is not working make sure that FlexCAN is not
// installed. (Check the onenote for instructions)
#include <FlexCAN.h>
#include <kinetis_flexcan.h>

// define message type
static CAN_message_t msg;
static CAN_message_t rxmsg;


//------------------------------------------------------------------------------
//
//             Timer Initialization
//
//------------------------------------------------------------------------------

// initialize the CAN send timer variables
unsigned long SendTimer1000Hz   = 0;
unsigned long SendTimer500Hz    = 0;
unsigned long SendTimer200Hz    = 0;
unsigned long SendTimer100Hz    = 0;
unsigned long SendTimer50Hz     = 0;
unsigned long SendTimer20Hz     = 0;
unsigned long SendTimer10Hz     = 0;
unsigned long SendTimer1Hz      = 0;

// initialize the Sensor reading timer variables
unsigned long SensTimer10000Hz  = 0;
unsigned long SensTimer2000Hz   = 0;
unsigned long SensTimer1000Hz   = 0;
unsigned long SensTimer500Hz    = 0;
unsigned long SensTimer200Hz    = 0;
unsigned long SensTimer100Hz    = 0;
unsigned long SensTimer50Hz     = 0;
unsigned long SensTimer20Hz     = 0;
unsigned long SensTimer10Hz     = 0;
unsigned long SensTimer1Hz      = 0;

// intitialize a timer just for testing purposes
unsigned long TestingTimer       = 0;

// initialize the led blinking timer variable
unsigned long LEDTimer40Hz      = 0;

// initialize a variable to keep track of the led
bool LED_on = false;

// initialize the variable for water pump override
bool PUMP_on = false;

// initialize the CAN message counters
uint8_t messageCount100Hz = 0;

//------------------------------------------------------------------------------
//
//             Structures for PDM Output Channels
//
//------------------------------------------------------------------------------

// structure for PDM channels with only voltage logging
typedef struct
{

  // used to tell the status of the module
  uint8_t deviceStatus;

  // voltage sensing
  int voltSensVal = 0;
  int voltMax = -2147483647;      // minimum possible value for int
  int voltMin =  2147483647;      // maximum possible value for int
  int voltAvg = 0;
  int voltSensCount = 0;

} senseVolt;

senseVolt MAIN, DATA;

// structure for PDM channels with only voltage and current logging
typedef struct
{

  // used to tell the status of the module
  uint8_t deviceStatus;

  // voltage sensing
  int voltSensVal = 0;
  int voltMax = -2147483647;
  int voltMin = 2147483647;
  int voltAvg = 0;
  int voltSensCount = 0;

  // current sensing
  int currentSensVal = 0;
  int currentMax = -2147483647;
  int currentMin = 2147483647;
  int currentAvg = 0;
  int currentSensCount = 0;

} senseVoltCurrent;

senseVoltCurrent PDM, FUEL;

// structure for PDM channels with voltage, current, and PWM logging
typedef struct
{

  // used to tell the status of the module
  uint8_t deviceStatus;

  // voltage sensing
  int voltSensVal = 0;
  int voltMax = 0;
  int voltMin = 8191;
  int voltAvg = 0;
  int voltSensCount = 0;

  // current sensing
  int currentSensVal = 0;
  int currentMax = 0;
  int currentMin = 8191;
  int currentAvg = 0;
  int currentSensCount = 0;

  // pulse width modulation
  int currentPwmRate = 0;
  int targetPwmRate = 0;
  int currentPwmFreq = 0;
  int targetPwmFreq = 0;

} senseVoltCurrentPWM;

senseVoltCurrentPWM FANR, FANL, WP;

//------------------------------------------------------------------------------
//
//              Other Analog Inputs Initialization
//
//------------------------------------------------------------------------------

// board temp sensing
int BOARD_temp;


//------------------------------------------------------------------------------
//
//              CAN Input Variable Initialization
//
//------------------------------------------------------------------------------

// structure to store the attributes of messages recieved on the CAN Bus
typedef struct
{
  bool valid = 0;
  unsigned long lastRecieve = 0;
  int16_t value = 0;
  int16_t lowerBound = 0;
  int16_t upperBound = 0;

}canSensor;

// CAN0 sensors
canSensor CAN0_engTemp, CAN0_rpm;

// BatteryVoltAvg used for fan and water pump speed.
// values updated of PDM volt values after calculated in CAN send function
// used in setFanSpeed and setWaterPumpSpeed
int BatteryVoltAvg = 120000;
int MainVoltAvg = 0;

//------------------------------------------------------------------------------
//
//              Initialization of Variables Associated with Fan
//              and Water Pump Speed
//
//------------------------------------------------------------------------------

// timer for running the PWM updates
unsigned long PWM_calc_timer = 0; // runs all main PWM updates

// Pump speed set from look up table
int WP_speedPercent = 0;

// Variables used by SOFT_POWER
// used to store the current PWM vals
int WP_livePWM = 0;
int WP_livePWM2 = 0;

// PWM output modification
#define PWM_freq_pin 22 // 22 is the teensy pin for water pump output
int PWM_freq_live = 488.28; // Hz; default for teensy
int PWM_freq_high = 488.28;
int PWM_freq_low = 40;
int PWM_startup_mode_duration = 3500; // in millis
int PWM_startup_mode_range = 5; // PWM increments
unsigned long PWM_startup_end_time = 0;
int PWM_freq_status = 1; //this is just or testing. 1 = 488.28 Hz, 0 = 20 Hz. Sends under "water pump status" in DBC
// used to store the minimum allowed PWM
int WP_minPWM = 35;
//used to store the max PWM, should be 255
int WP_maxPWM = 255;
// used to store how much to increment the PWM when SOFT_POWER is called
int WP_incrementPWM = 1;

// Variables used to hold values returned by findTemp
int WP_temperatureGreater = 0;
int WP_temperatureLesser = 0;

// Variables used to hold values returned by findRPM
int WP_rpmLesser = 0;
int WP_rpmGreater = 0;

// Variable to see if the car is on (true if the main circuit is powered)
bool carOn = false;
int carOnThreshold = 8000; // determine the car is on when the Main voltage is above 8V



// Number of temperature entries in the water pump speed table
const int WP_numTempEntries = 12;

// Number of battery voltage entries in the fan speed table
const int WP_numRPMEntries = 8;


//    rows: temp in degrees celcius * 10
// columns: RPM
int waterPumpTable[WP_numTempEntries][WP_numRPMEntries] =
{
  {   00,   0,  10,  20, 500, 510,  5000, 15000},
  {  100,   0,   0,   0,   0,  20,    20,    20},
  {  200,   0,   0,   0,   0,  20,    20,    20},
  {  400,   0,   0,   0,   0,  35,    35,    35},
  {  500,   0,   0,   0,   0,  35,    35,    35},
  {  600,   0,   0,   0,   0,  35,    35,    35},
  {  699,   0,   0,   0,   0,  35,    35,    35},
  {  700,   0,   0,   0,   0,  40,    40,    40},
  {  845,   0,   0,   0,   0,  50,    50,    50},
  {  846,  25,  25,   0,   0,  60,    60,    60},
  { 1000, 100, 100,   0,   0, 100,   100,   100},
  { 1500, 100, 100,   0,   0, 100,   100,   100},
};


//------------------------------------------------------------------------------
//
//
//
//              END INITIALIZATION
//
//
//
//------------------------------------------------------------------------------



void setup() {

  // start the CAN busses
  Can0.begin(1000000);

  Serial.begin(9600);

  // set the resolution of the analog input from 0-1023 (10bit) to 0-1891 (13bit)
  analogReadResolution(10);

  // set the pins as inputs or outputs
  pinMode(A0, INPUT); // Fuel Current Sensor
  pinMode(A1, INPUT); // FanR Current Sensor
  pinMode(A2, INPUT); // FanL Current Sensor
  pinMode(A3, OUTPUT); // Brake Light Signal
  pinMode(A4, INPUT); // WP Current Sensor
  pinMode(A5, INPUT); // PDM Voltage
  pinMode(A6, OUTPUT); // FanL Signal
  pinMode(A7, OUTPUT); // FanR Signal
  pinMode(A8, OUTPUT); // Water Pump Signal
  pinMode(A9, INPUT); // PDM Current Sense
  pinMode(13, OUTPUT); // Onboard LED
  pinMode(A16, INPUT); // Board Temperature
  pinMode(A17, INPUT); // Data Voltage
  pinMode(A18, INPUT); // Main Voltage
  pinMode(A19, INPUT); // Fuel Voltage
  pinMode(A20, INPUT); // FanL Voltage
  pinMode(A21, INPUT); // FanR Voltage
  pinMode(A22, INPUT); // WP Voltage

}

//------------------------------------------------------------------------------
//
//              LOOP
//
//------------------------------------------------------------------------------

void loop() {


  CAN_READ();
  //----------------------------------------------------------------------------
  //
  //              Timer for LED Blink
  //
  //----------------------------------------------------------------------------

  if ( millis() - LEDTimer40Hz >= 25)
  {
    LEDTimer40Hz = millis();
    //analogWrite(A7, 255);

    if      ( LED_on == false ){ digitalWrite(13, HIGH); LED_on = true; }
    else if ( LED_on == true ){ digitalWrite(13, LOW); LED_on = false; }
  }

  //----------------------------------------------------------------------------
  //
  //              Get Fan and Water Pump Speeds
  //
  //----------------------------------------------------------------------------

  // Pump calcs
  if (millis() - PWM_calc_timer >= 50)
  {
    PWM_calc_timer = millis();

    // water pump calculations
    WP_findTemp(waterPumpTable);
    WP_findRPM(waterPumpTable);
    WP_speedPercent = WATER_PUMP_PERCENT(waterPumpTable);
    WP_livePWM = SOFT_POWER(WP_speedPercent, WP_livePWM, WP_minPWM, WP_maxPWM, WP_incrementPWM);

    // determine what PWM frequency is appropriate before writing
    SET_PWM_FREQUENCY();

    // this if statement is for the WP override button. (analogread <= 100 for a small buffer);
    // REPLACE WITH ACTUAL PIN
    if (analogRead(A8) <= 100){
      // this if statement only writes to the pin if the PWM changes from it's previous value (held by livePWM2)
      if (WP_livePWM != WP_livePWM2) {WP_livePWM2 = WP_livePWM; analogWrite(A8, WP_livePWM);}
    } else {
      // keep dat pump on
      analogWrite(A8, 255);
    }
  }



  //----------------------------------------------------------------------------
  //
  //              Timer for Reading Sensors
  //
  //----------------------------------------------------------------------------


  if ( micros() - SensTimer10000Hz >= 1) // 10,000 times a second
  {
    SensTimer10000Hz = micros();

    ANA_STORE(0);

  }

  //----------------------------------------------------------------------------
  //
  //              Calculating sensors and sending CAN
  //
  //----------------------------------------------------------------------------

  CALC_SEND_CAN();



}






//------------------------------------------------------------------------------
//
//
//
//              FUNCTIONS
//
//
//
//------------------------------------------------------------------------------


static void ANA_READ( int sensGroup )
{

  //----------------------------------------------------------------------------
  // Reads the analog value of the pins ( 0 - 8191) and
  // assigns it to the correct SensVal
  //----------------------------------------------------------------------------

  switch ( sensGroup ) {

    case 0:

      // Read Analog Pins ------------------------------------------

      FUEL.currentSensVal   = analogRead(A0);   //Fuel Current Sense
      FANR.currentSensVal   = analogRead(A1);   // FanR Current Sense
      FANL.currentSensVal   = analogRead(A2);   // FanL Current Sense
      //placeHolder         = analogRead(A3);   // Teensy Brake Sig
      WP.currentSensVal     = analogRead(A4);   // WP Current Sense
      PDM.voltSensVal       = analogRead(A5);   // PDM Voltage
      //placeHolder         = analogRead(A6);   // FanL Signal
      //placeHolder         = analogRead(A7);   // FanR Signal
      //placeHolder         = analogRead(A8);   // Water Pump Signal
      PDM.currentSensVal    = analogRead(A9);   // PDM Current Sense
      //placeHolder         = analogRead(A10);
      //placeHolder         = analogRead(A11);
      //placeHolder         = analogRead(A12);
      //placeHolder         = analogRead(A13);
      //placeHolder         = analogRead(A14);
      //placeHolder         = analogRead(A15);
      //BOARD_temp          = analogRead(A16);  // Board Temperature
      DATA.voltSensVal      = analogRead(A17);  // Data Voltage
      MAIN.voltSensVal      = analogRead(A18);  // Main Voltage
      FUEL.voltSensVal      = analogRead(A19);  // Fuel Voltage
      FANL.voltSensVal      = analogRead(A20);  // FanL Voltage
      FANR.voltSensVal      = analogRead(A21);  // FanR Voltage
      WP.voltSensVal        = analogRead(A22);  // WP Voltage

      // print what what the teensy is reading
      // Serial.println();
      // Serial.print("      Fuel current = "); Serial.println(FUEL.currentSensVal);
      // Serial.print("      FanR current = "); Serial.println(FANR.currentSensVal);
      // Serial.print("      FanL current = "); Serial.println(FANL.currentSensVal);
      // Serial.print("Water Pump current = "); Serial.println(WP.currentSensVal);
      // Serial.print("       PDM current = "); Serial.println(PDM.currentSensVal);
      // Serial.print("      Data voltage = "); Serial.println(DATA.voltSensVal);
      // Serial.print("      Main voltage = "); Serial.println(MAIN.voltSensVal);
      // Serial.print("      Fuel voltage = "); Serial.println(FUEL.voltSensVal);
      // Serial.print("      FanL voltage = "); Serial.println(FANL.voltSensVal);
      // Serial.print("      FanR voltage = "); Serial.println(FANR.voltSensVal);
      // Serial.print("Water Pump voltage = "); Serial.println(WP.voltSensVal);



      break;
  }
}










static void ANA_STORE( int senseGroup )
{

  //----------------------------------------------------------------------------
  // calls the read funcion
  // determines if the SenseVal is a min or max
  // adds the SenseVal to the average
  // adds one to the sensor counter
  //----------------------------------------------------------------------------

  switch ( senseGroup ) {

    //--------------------------------------------------------------------------
    //
    //
    //
    //             CASE 0
    //
    //
    //
    //--------------------------------------------------------------------------

    case 0:

      // read the analog sensor values for the specified case
      ANA_READ(senseGroup);

      //------------------------------------------------------------------------
      //
      //              Raw Voltage Min, Max, Avg, and Count Storage
      //
      //------------------------------------------------------------------------

      // PDM Voltage ---------------------------------------------
      if        ( PDM.voltSensVal < PDM.voltMin ){ PDM.voltMin = PDM.voltSensVal; }
      else if   ( PDM.voltSensVal > PDM.voltMax ){ PDM.voltMax = PDM.voltSensVal; }
      PDM.voltAvg += PDM.voltSensVal;
      PDM.voltSensCount++;

      // Main Voltage --------------------------------------------
      if        ( MAIN.voltSensVal < MAIN.voltMin ){ MAIN.voltMin = MAIN.voltSensVal; }
      else if   ( MAIN.voltSensVal > MAIN.voltMax ){ MAIN.voltMax = MAIN.voltSensVal; }
      MAIN.voltAvg += MAIN.voltSensVal;
      MAIN.voltSensCount++;

      // DATA Voltage --------------------------------------------
      if        ( DATA.voltSensVal < DATA.voltMin ){ DATA.voltMin = DATA.voltSensVal; }
      else if   ( DATA.voltSensVal > DATA.voltMax ){ DATA.voltMax = DATA.voltSensVal; }
      DATA.voltAvg += DATA.voltSensVal;
      DATA.voltSensCount++;

      // FUEL Voltage --------------------------------------------
      if        ( FUEL.voltSensVal < FUEL.voltMin ){ FUEL.voltMin = FUEL.voltSensVal; }
      else if   ( FUEL.voltSensVal > FUEL.voltMax ){ FUEL.voltMax = FUEL.voltSensVal; }
      FUEL.voltAvg += FUEL.voltSensVal;
      FUEL.voltSensCount++;

      // WP Voltage ----------------------------------------------
      if        ( WP.voltSensVal < WP.voltMin ){ WP.voltMin = WP.voltSensVal; }
      else if   ( WP.voltSensVal > WP.voltMax ){ WP.voltMax = WP.voltSensVal; }
      WP.voltAvg += WP.voltSensVal;
      WP.voltSensCount++;

      // FANR Voltage --------------------------------------------
      if        ( FANR.voltSensVal < FANR.voltMin ){ FANR.voltMin = FANR.voltSensVal; }
      else if   ( FANR.voltSensVal > FANR.voltMax ){ FANR.voltMax = FANR.voltSensVal; }
      FANR.voltAvg += FANR.voltSensVal;
      FANR.voltSensCount++;

      // FANL Voltage --------------------------------------------
      if        ( FANL.voltSensVal < FANL.voltMin ){ FANL.voltMin = FANL.voltSensVal; }
      else if   ( FANL.voltSensVal > FANL.voltMax ){ FANL.voltMax = FANL.voltSensVal; }
      FANL.voltAvg += FANL.voltSensVal;
      FANL.voltSensCount++;

      //------------------------------------------------------------------------
      //
      //              Raw Current Min, Max, Avg, and Calc Storage
      //
      //------------------------------------------------------------------------

      // PDM Current ---------------------------------------------
      if        ( PDM.currentSensVal < PDM.currentMin ){ PDM.currentMin = PDM.currentSensVal; }
      else if   ( PDM.currentSensVal > PDM.currentMax ){ PDM.currentMax = PDM.currentSensVal; }
      PDM.currentAvg += PDM.currentSensVal;
      PDM.currentSensCount++;

      // FUEL Current --------------------------------------------
      if        ( FUEL.currentSensVal < FUEL.currentMin ){ FUEL.currentMin = FUEL.currentSensVal; }
      else if   ( FUEL.currentSensVal > FUEL.currentMax ){ FUEL.currentMax = FUEL.currentSensVal; }
      FUEL.currentAvg += FUEL.currentSensVal;
      FUEL.currentSensCount++;

      // WP Current ----------------------------------------------
      if        ( WP.currentSensVal < WP.currentMin ){ WP.currentMin = WP.currentSensVal; }
      else if   ( WP.currentSensVal > WP.currentMax ){ WP.currentMax = WP.currentSensVal; }
      WP.currentAvg += WP.currentSensVal;
      WP.currentSensCount++;

      // FANR Current --------------------------------------------
      if        ( FANR.currentSensVal < FANR.currentMin ){ FANR.currentMin = FANR.currentSensVal; }
      else if   ( FANR.currentSensVal > FANR.currentMax ){ FANR.currentMax = FANR.currentSensVal; }
      FANR.currentAvg += FANR.currentSensVal;
      FANR.currentSensCount++;

      // FANL Current --------------------------------------------
      if        ( FANL.currentSensVal < FANL.currentMin ){ FANL.currentMin = FANL.currentSensVal; }
      else if   ( FANL.currentSensVal > FANL.currentMax ){ FANL.currentMax = FANL.currentSensVal; }
      FANL.currentAvg += FANL.currentSensVal;
      FANL.currentSensCount++;

      break;
  }
}










static void ANA_TO_SENSORVAL( int sensGroup )
{

  //----------------------------------------------------------------------------
  // converts the analog values to sensor voltage
  // converts the sensor voltage to true sensor values (based on datasheets)
  //----------------------------------------------------------------------------

  // only do conversions for desired sensors
  switch ( sensGroup ){

    case 0 :
      //------------------------------------------------------------------------
      //
      //                Analog to Sensor Voltage
      //
      //------------------------------------------------------------------------

      // reading to convert               * analog to teensy voltage    * teensy voltage to sensor voltage
      PDM.voltMin = PDM.voltMin           * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      PDM.voltMax = PDM.voltMax           * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      PDM.voltAvg /= PDM.voltSensCount;
      PDM.voltSensCount = 0;
      PDM.voltAvg = PDM.voltAvg           * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      // divide by 10 to factor down one decimal place
      PDM.voltMin /= 10;
      PDM.voltMax /= 10;
      PDM.voltAvg /= 10;
      // store the avg battery voltage in a variable that doesn't reset every cycle (used in water pump speed calculation)
      BatteryVoltAvg= PDM.voltAvg;

      MAIN.voltMin = MAIN.voltMin         * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      MAIN.voltMax = MAIN.voltMax         * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      MAIN.voltAvg /= MAIN.voltSensCount;
      MAIN.voltSensCount = 0;
      MAIN.voltAvg = MAIN.voltAvg         * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      MAIN.voltMin /= 10;
      MAIN.voltMax /= 10;
      MAIN.voltAvg /= 10;
      // store the avg Main voltage in a variable that doesn't reset every cycle (used to determine if the Main circuit is on)
      MainVoltAvg = MAIN.voltAvg;

      DATA.voltMin = DATA.voltMin         * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      DATA.voltMax = DATA.voltMax         * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      DATA.voltAvg /= DATA.voltSensCount;
      DATA.voltSensCount = 0;
      DATA.voltAvg = DATA.voltAvg         * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      DATA.voltMin /= 10;
      DATA.voltMax /= 10;
      DATA.voltAvg /= 10;

      FUEL.voltMin = FUEL.voltMin         * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      FUEL.voltMax = FUEL.voltMax         * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      FUEL.voltAvg /= FUEL.voltSensCount;
      FUEL.voltSensCount = 0;
      FUEL.voltAvg = FUEL.voltAvg         * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      FUEL.voltMin /= 10;
      FUEL.voltMax /= 10;
      FUEL.voltAvg /= 10;

      WP.voltMin = WP.voltMin             * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      WP.voltMax = WP.voltMax             * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      WP.voltAvg /= WP.voltSensCount;
      WP.voltSensCount = 0;
      WP.voltAvg = WP.voltAvg             * (33000 / 1023)              / 10000.0000 * (10000.0000 + 39000.0000);
      WP.voltMin /= 10;
      WP.voltMax /= 10;
      WP.voltAvg /= 10;


      PDM.currentMin = PDM.currentMin     * (33000 / 1023);
      PDM.currentMax = PDM.currentMax     * (33000 / 1023);
      PDM.currentAvg /= PDM.currentSensCount;
      PDM.currentSensCount = 0;
      PDM.currentAvg = PDM.currentAvg     * (33000 / 1023);

      FUEL.currentMin = FUEL.currentMin   * (33000 / 1023);
      FUEL.currentMax = FUEL.currentMax   * (33000 / 1023);
      FUEL.currentAvg /= FUEL.currentSensCount;
      FUEL.currentSensCount = 0;
      FUEL.currentAvg = FUEL.currentAvg   * (33000 / 1023);

      WP.currentMin = WP.currentMin       * (33000 / 1023);
      WP.currentMax = WP.currentMax       * (33000 / 1023);
      WP.currentAvg /= WP.currentSensCount;
      WP.currentSensCount = 0;
      WP.currentAvg = WP.currentAvg       * (33000 / 1023);


      //------------------------------------------------------------------------
      //
      //                Sensor Voltage to True Values
      //
      //------------------------------------------------------------------------

      // formula:
      // = ( ANA_sensorVolt[ x] -  16500 ) / (1.0000/132.0000) * 100;
      //            |                |                |            |
      // recorded sensor (mV * 10)   |                |            |
      //                             |                |            |
      //            sensor zero voltage (mV * 10)     |            |
      //                                              |            |
      //                    inverse of (mV*10)/unit for sensor     |
      //                                                           |
      //                       scaling factor ( inverse of scaling factor in the DBC)

      // ***** sensor calibration *****                                     //  factor, min rate, CAN signal name
      FUEL.currentMin = ( FUEL.currentMin - 16500 ) * (1.0000 / 264.0000) * 100;
      FUEL.currentMax = ( FUEL.currentMax - 16500 ) * (1.0000 / 264.0000) * 100;
      FUEL.currentAvg = ( FUEL.currentAvg - 16500 ) * (1.0000 / 264.0000) * 100;

      WP.currentMin   = ( WP.currentMin - 16390 )   * (1.0000 / 264.0000) * 100;
      WP.currentMax   = ( WP.currentMax - 16390 )   * (1.0000 / 264.0000) * 100;
      WP.currentAvg   = ( WP.currentAvg - 16390 )   * (1.0000 / 264.0000) * 100;

      PDM.currentMin  = ( PDM.currentMin - 16390 )  * (1.0000/132.0000) * 100;
      PDM.currentMax  = ( PDM.currentMax - 16390 )  * (1.0000/132.0000) * 100;
      PDM.currentAvg  = ( PDM.currentAvg - 16390 )  * (1.0000/132.0000) * 100;


      //
      break;
  }
}










static void CALC_SEND_CAN()
{

  //----------------------------------------------------------------------------
  // Timers for every CAN message
  //    calculate values and put them into CAN arrays
  //    call  send CAN function
  //    reset sensor min, max, avg, and count for future data calculations
  //----------------------------------------------------------------------------

  /*
   * Template
    msg.buf[0] = SensVal[0];
    msg.buf[1] = SensVal[0] >> 8; //
    msg.buf[2] = SensVal[1];
    msg.buf[3] = SensVal[1] >> 8; //
    msg.buf[4] = SensVal[2];
    msg.buf[5] = SensVal[2] >> 8; //
    msg.buf[6] = SensVal[3];
    msg.buf[7] = SensVal[3] >> 8; //
    CAN_DATA_SEND(0x50, 8, 1); // 500Hz
   *
   */

  // if ( millis() - SendTimer1000Hz >= 1 )
  // {
  //   SendTimer1000Hz = millis();
  // }
  //
  // if ( millis() - SendTimer500Hz >=  2 )
  // {
  //   SendTimer500Hz = millis();
  //
  // }
  //
  // if ( millis() - SendTimer200Hz >= 5 )
  // {
  //   SendTimer200Hz = millis();
  // }

  if ( millis() - SendTimer100Hz >= 10 )
  {
    SendTimer100Hz = millis();

    ANA_TO_SENSORVAL(0);

    if ( messageCount100Hz < 15 ){messageCount100Hz++;}
    else {messageCount100Hz = 0;}


//   Serial.println();
//   Serial.print("      Fuel current = "); Serial.println(FUEL.currentAvg);
//   Serial.print("      FanR current = "); Serial.println(FANR.currentAvg);
//   Serial.print("      FanL current = "); Serial.println(FANL.currentAvg);
//   Serial.print("Water Pump current = "); Serial.println(WP.currentAvg);
//   Serial.print("       PDM current = "); Serial.println(PDM.currentAvg);
//   Serial.print("       PDM voltage = "); Serial.println(PDM.voltAvg);
//   Serial.print("      Data voltage = "); Serial.println(DATA.voltAvg);
//   Serial.print("      Main voltage = "); Serial.println(MAIN.voltAvg);
//   Serial.print("      Fuel voltage = "); Serial.println(FUEL.voltAvg);
//   Serial.print("      FanL voltage = "); Serial.println(FANL.voltAvg);
//   Serial.print("      FanR voltage = "); Serial.println(FANR.voltAvg);
//   Serial.print("Water Pump voltage = "); Serial.println(WP.voltAvg);
//   Serial.println(analogRead(A21));

    //PDM_01
    msg.buf[0] = messageCount100Hz; //add counter
    msg.buf[1] = FANR.currentMax; //
    msg.buf[2] = FANR.currentMax >> 8;
    msg.buf[3] = FANR.currentAvg; //
    msg.buf[4] = FANR.currentAvg >> 8;
    msg.buf[5] = FANR.currentMin; //
    msg.buf[6] = FANR.currentMin >> 8;
    msg.buf[7] = 0;
    CAN_DATA_SEND(0x97, 8, 1); // 100Hz

    FANR.currentMax = -2147483647;
    FANR.currentAvg = 0;
    FANR.currentMin = 2147483647;

    //PDM_02
    msg.buf[0] = messageCount100Hz; // add counter
    msg.buf[1] = FANL.currentMax; //
    msg.buf[2] = FANL.currentMax >> 8;
    msg.buf[3] = FANL.currentAvg;
    msg.buf[4] = FANL.currentAvg >> 8;
    msg.buf[5] = FANL.currentMin;
    msg.buf[6] = FANL.currentMin >> 8;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0x98, 8, 1); // 100Hz

    FANL.currentMax = -2147483647;
    FANL.currentAvg = 0;
    FANL.currentMin = 2147483647;

    //PDM_03
    msg.buf[0] = messageCount100Hz; // counter
    msg.buf[1] = WP.currentMax;
    msg.buf[2] = WP.currentMax >> 8;
    msg.buf[3] = WP.currentAvg;
    msg.buf[4] = WP.currentAvg >> 8;
    msg.buf[5] = WP.currentMin;
    msg.buf[6] = WP.currentMin >> 8;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0x99, 8, 1); // 100Hz

    WP.currentMax = -2147483647;
    WP.currentAvg = 0;
    WP.currentMin = 2147483647;

    //PDM_04
    msg.buf[0] = messageCount100Hz; // counter
    msg.buf[1] = PDM.currentMax;
    msg.buf[2] = PDM.currentMax >> 8;
    msg.buf[3] = PDM.currentAvg;
    msg.buf[4] = PDM.currentAvg >> 8;
    msg.buf[5] = PDM.currentMin;
    msg.buf[6] = PDM.currentMin >> 8;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0x9A, 8, 1); // 100Hz

    PDM.currentMax = -2147483647;
    PDM.currentAvg = 0;
    PDM.currentMin = 2147483647;

    //PDM_05
    msg.buf[0] = messageCount100Hz; // counter
    msg.buf[1] = FUEL.currentMax;
    msg.buf[2] = FUEL.currentMax >> 8;
    msg.buf[3] = FUEL.currentAvg;
    msg.buf[4] = FUEL.currentAvg >> 8;
    msg.buf[5] = FUEL.currentMin;
    msg.buf[6] = FUEL.currentMin >> 8;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0x9B, 8, 1); // 100Hz

    FUEL.currentMax = -2147483647;
    FUEL.currentAvg = 0;
    FUEL.currentMin = 2147483647;

    //PDM_06
    msg.buf[0] = messageCount100Hz; // counter
    msg.buf[1] = FANR.voltMax;
    msg.buf[2] = FANR.voltMax >> 8;
    msg.buf[3] = FANR.voltAvg;
    msg.buf[4] = FANR.voltAvg >> 8;
    msg.buf[5] = FANR.voltMin;
    msg.buf[6] = FANR.voltMin >> 8;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0x9C, 8, 1); // 100Hz

    FANR.voltAvg = 0;
    FANR.voltMax = -2147483647;
    FANR.voltMin = 2147483647;

    //PDM_07
    msg.buf[0] = messageCount100Hz; // counter
    msg.buf[1] = FANL.voltMax;
    msg.buf[2] = FANL.voltMax >> 8;
    msg.buf[3] = FANL.voltAvg;
    msg.buf[4] = FANL.voltAvg >> 8;
    msg.buf[5] = FANL.voltMin;
    msg.buf[6] = FANL.voltMin >> 8;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0x9D, 8, 1); // 100Hz

    FANL.voltMax = -2147483647;
    FANL.voltAvg = 0;
    FANL.voltMin = 2147483647;

    //PDM_08
    msg.buf[0] = messageCount100Hz; // counter
    msg.buf[1] = WP.voltMax;
    msg.buf[2] = WP.voltMax >> 8;
    msg.buf[3] = WP.voltAvg;
    msg.buf[4] = WP.voltAvg >> 8;
    msg.buf[5] = WP.voltMin;
    msg.buf[6] = WP.voltMin >> 8;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0x9E, 8, 1); // 100Hz

    WP.voltMax = -2147483647;
    WP.voltAvg = 0;
    WP.voltMin = 2147483647;

    //PDM_09
    msg.buf[0] = messageCount100Hz; // counter
    msg.buf[1] = PDM.voltMax;
    msg.buf[2] = PDM.voltMax >> 8;
    msg.buf[3] = PDM.voltAvg;
    msg.buf[4] = PDM.voltAvg >> 8;
    msg.buf[5] = PDM.voltMin;
    msg.buf[6] = PDM.voltMin >> 8;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0x9F, 8, 1); // 100Hz

    PDM.voltMax = -2147483647;
    PDM.voltAvg = 0;
    PDM.voltMin = 2147483647;

    //PDM_10
    msg.buf[0] = messageCount100Hz; // counter
    msg.buf[1] = FUEL.voltMax;
    msg.buf[2] = FUEL.voltMax >> 8;
    msg.buf[3] = FUEL.voltAvg;
    msg.buf[4] = FUEL.voltAvg >> 8;
    msg.buf[5] = FUEL.voltMin;
    msg.buf[6] = FUEL.voltMin >> 8;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0xA0, 8, 1); // 100Hz

    FUEL.voltMax = -2147483647;
    FUEL.voltAvg = 0;
    FUEL.voltMin = 2147483647;

    //PDM_11
    msg.buf[0] = messageCount100Hz; // counter
    msg.buf[1] = MAIN.voltMax;
    msg.buf[2] = MAIN.voltMax >> 8;
    msg.buf[3] = MAIN.voltAvg;
    msg.buf[4] = MAIN.voltAvg >> 8;
    msg.buf[5] = MAIN.voltMin;
    msg.buf[6] = MAIN.voltMin >> 8;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0xA1, 8, 1); // 100Hz

    MAIN.voltMax = -2147483647;
    MAIN.voltAvg = 0;
    MAIN.voltMin = 2147483647;

    //PDM_12
    msg.buf[0] = messageCount100Hz; // counter
    msg.buf[1] = DATA.voltMax;
    msg.buf[2] = DATA.voltMax >> 8;
    msg.buf[3] = DATA.voltAvg;
    msg.buf[4] = DATA.voltAvg >> 8;
    msg.buf[5] = DATA.voltMin;
    msg.buf[6] = DATA.voltMin >> 8;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0xA2, 8, 1); // 100Hz

    DATA.voltMax = -2147483647;
    DATA.voltAvg = 0;
    DATA.voltMin = 2147483647;

    //PDM_13
    msg.buf[0] = 0;// brakelight state and counter - set up later;
    msg.buf[1] = 0;
    msg.buf[2] = 0;
    msg.buf[3] = WP_livePWM;
    msg.buf[4] = 0;
    msg.buf[5] = 0;
    msg.buf[6] = 0;
    msg.buf[7] = 0; //
    CAN_DATA_SEND(0xA3, 8, 1); // 100Hz

    // message for PWM update signal (testing; temp)
    msg.buf[0] = 0;
    msg.buf[1] = 0;
    msg.buf[2] = PWM_freq_status << 4;
    msg.buf[3] = 0;
    msg.buf[4] = 0;
    msg.buf[5] = 0;
    msg.buf[6] = 0;
    msg.buf[7] = 0;
    CAN_DATA_SEND(0x96, 8, 1);


  }
}











static void CAN_DATA_SEND(int id, int len, int busNo)
{
  msg.len = len;  //CAN message length unit: Byte (8 bits)
  msg.id = id; //CAN ID

  switch(busNo)
  {
    case 0:
      Can0.write(msg);  // this is send the static
      break;
  }
}











void CAN_READ()
{
  //----------------------------------------------------------------------------
  //              CAN 0
  //----------------------------------------------------------------------------

  if ( Can0.read(rxmsg) )
  {

    // store the message ID and length
    int rxID = rxmsg.id;
    int rxDataLen = rxmsg.len;

    // assign the message data into a more easily readable array, rxData
    int rxData[rxDataLen];
    for(int i=0; i<rxDataLen; i++)
    {
      rxData[i]=rxmsg.buf[i];
      //Serial.print(rxData[i]);
      //Serial.print(" ");
    }

    // assign the first byte as the multiplexer ID
    int rxMultID = rxData[0];

    switch(rxID)
    {
      // MOTEC M400 MESSAGES *KEEP AT TOP*
      // M400_dataSet2
      // ID 0x5F0
      case 0x5EF:
      {

        // read the multiplexor
        switch(rxMultID)
        {

          // MultID 0x0
          case 0x0:
            CAN0_rpm.value = rxData[4] * 256 + rxData[5];

            CAN0_rpm.lastRecieve = millis();
            break;

          // MultID 0x4
          case 0x4:
            CAN0_engTemp.value = rxData[4] * 256 + rxData[5];

            CAN0_engTemp.lastRecieve = millis();
            break;
        }
      }
    }
  }
}




void WP_findTemp(int table[WP_numTempEntries][WP_numRPMEntries])
// ---------------------------------------------------------------------------------------
// Given a temperature value read from the sensor, findTemp will look through entries
// in the water pump table to determine if the value is present in the table. If the value
// is present, the value is returned twice. If the value is not present, the values that
// are greater than and less than are returned.
// ---------------------------------------------------------------------------------------
{
  for (int i = 1; i < WP_numTempEntries; i++)
  {

    if (table[i][0] == CAN0_engTemp.value)
    {
      WP_temperatureGreater = i;
      WP_temperatureLesser = i;
      break;
    }

    // check that the lesser will not exceed the lower bounds of the table
    if (table[1][0] > CAN0_engTemp.value)
    {
      WP_temperatureGreater = i;
      WP_temperatureLesser = i;
      break;
    }

    if (table[i][0] > CAN0_engTemp.value)
    {
      WP_temperatureGreater = i;
      WP_temperatureLesser = i - 1;
      break;
    }
  }
}




void WP_findRPM(int table[WP_numTempEntries][WP_numRPMEntries])
// ---------------------------------------------------------------------------------------
// Given an rpm value read from the sensor, findRPM will look through entries
// in the water pump table to determine if the value is present in the table. If the value
// is present, the value is returned twice. If the value is not present, the values that
// are greater than and less than are returned.
// ---------------------------------------------------------------------------------------
{
  for (int i = 1; i < WP_numRPMEntries; i++)
  {

    if (table[0][i] == CAN0_rpm.value)
    {
      WP_rpmGreater = i;
      WP_rpmLesser = i;
      break;
    }

    // check that the lesser will not exceed the lower bounds of the table
    if (table[0][1] > CAN0_engTemp.value)
    {
      WP_rpmLesser = i;
      WP_rpmGreater = i;
      break;
    }

    if (table[0][i] > CAN0_rpm.value)
    {
      WP_rpmGreater = i;
      WP_rpmLesser = i - 1;
      break;
    }
  }
}




int WATER_PUMP_PERCENT(int table[WP_numTempEntries][WP_numRPMEntries])
// -----------------------------------------------------------------------------------------------------------------------
// When a table is input into the setWaterPumpSpeed function, the function looks at the global temperature in relation to
// tempLesser and tempGreater, then maps it to the corresponding fanspeeds for the lower RPM. The process happens
// again but maps to the fanspeeds for the higher RPM. Then, the function looks at the rpm in relation to rpmLesser
// and rpmGreater, and maps it between the results of the first two maps. Simple, right?
//
// Returns percentage (0-100) of maximum pump speed
// -----------------------------------------------------------------------------------------------------------------------
{

  int pumpSpeed = 0;

  // map the actual temp input between the max and min temp in the table,
  // to the corresponding bottom and top rates found in rpmLesser
  int map1 = map(CAN0_engTemp.value, table[WP_temperatureLesser][0], table[WP_temperatureGreater][0], table[WP_temperatureLesser][WP_rpmLesser], table[WP_temperatureGreater][WP_rpmLesser]);


  // do the same as map1, only map it to the corresponding rpmLesser values in the fan table
  int map2 = map(CAN0_engTemp.value, table[WP_temperatureLesser][0], table[WP_temperatureGreater][0], table[WP_temperatureLesser][WP_rpmGreater], table[WP_temperatureGreater][WP_rpmGreater]);


  // now, map the opposite direction in the table, by mapping the actual rpm between the min and max in the table
  // to the results of the previous map
  pumpSpeed = map(CAN0_rpm.value, table[0][WP_rpmLesser], table[0][WP_rpmGreater], map1, map2);

  return pumpSpeed;
}











int SOFT_POWER(int powerPercent, int livePWM, int minPWM, int maxPWM, int incrementPWM)
{
//-------------------------------------------------------------------------------------------
// when given a percentage of power from either WATER_PUMP_PERCENT or FAN_PERCENT, live PWM,
// minimum allowed PWM, and the desired PWM increment per cycle, SOFT_POWER will return a PWM
// value +/- the increment desired
//--------------------------------------------------------------------------------------------

  // first convert the power percent to a PWM value
  int targetPWM = map(powerPercent, 0, 100, 0, 255);
  //Serial.print("    Target PWM: "); Serial.println(targetPWM);

  // make sure that the target is above the minimum PWM, otherwise do nothing
  if ( targetPWM >= minPWM )
  {

    // if the live PWM is less than the minimum PWM, set it to the minimum
    if ( livePWM < minPWM )
    {
      livePWM = minPWM;
    }

    // the PWM is already at the correct value (do nothing)
    else if (livePWM == targetPWM)
    {
      // do nothing
    }

    // live PWM is less than the target PWM (raise the PWM)
    else if ( livePWM < targetPWM )
    {
      livePWM += incrementPWM;

      // be sure that the new PDM does not exceed the maximum!
      if ( livePWM > maxPWM )
      {
        livePWM = maxPWM;
      }
    }

    // live PWM is more than the target PWM (lower the PWM)
    else if ( livePWM > targetPWM)
    {
      livePWM -= incrementPWM;

      // if the increments fuck up because of weird readings, make sure that the livePWM
      // does not slip below the minumim. This is because we're in the (targetPWM >= minPWM) if statement
      // so we still want to motors to run, even if it's at the minimum PWM permitted. NOTE: notice that this
      // if statement alreacy exists, but it's important to repeat it after we reduce the PWM, because otherwise
      // we might end up sending a dead current to the motors.
      if ( livePWM < minPWM )
      {
        livePWM = minPWM;
      }
    }


  }
  // if the target PWM is below the minimum, set the actual PWM to 0
  else
  {
    livePWM = 0;
  }

  return livePWM;
}










void SET_PWM_FREQUENCY()
//------------------------------------------------------------------------------
// if any of the motor PWMs are in a certain threshold range, initialize startup
// mode by adding the desired time to the current time. Startup mode will run
// until the current time is greater than the time set by the initializer.
// only writes the frequency to the teeny's PWM timers if they change.
//------------------------------------------------------------------------------
{
  // STARTUP MODE INITIALIZER
  // determine if we should trigger startup mode (PWM buffer for triggering range)
  if ( WP_minPWM <= WP_livePWM && WP_livePWM <= (WP_minPWM + PWM_startup_mode_range) )
  {
    // take the current time, then add a set duration. this defines when the
    // startup mode (high freq) will end
    PWM_startup_end_time = millis() + PWM_startup_mode_duration;
  }



  // STARTUP MODE
  // note: PWM_startup_end_time should always be less than millis() unless the initializer triggers
  if (millis() <= PWM_startup_end_time) // "if we're in startup mode"
  {
    // PWM should be high
    if (PWM_freq_live != PWM_freq_high) {PWM_freq_live = PWM_freq_high; analogWriteFrequency(PWM_freq_pin, PWM_freq_live);}
    PWM_freq_status = 1;

  }
  else
  {
    // PWM should be low
    if (PWM_freq_live != PWM_freq_low) {PWM_freq_live = PWM_freq_low; analogWriteFrequency(PWM_freq_pin, PWM_freq_live);}
    PWM_freq_status = 0;
  }
}
