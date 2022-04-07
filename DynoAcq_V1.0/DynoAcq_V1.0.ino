/* Ronald Hodge 2021
    Michigan State Formula Racing Team
    Code for reading and calibrating torque from strain gauges.
    Also for controling stepper motor in future.
*/
#include <circular_buffer.h>
#include <FlexCAN_T4.h>
#include <imxrt_flexcan.h>
#include <kinetis_flexcan.h>
#include <StateCAN.h>

#include <EEPROM.h>
#include <SPI.h>
#include <ReadADC.h>
#include <EasyTimer.h>

#include "dyno.hpp"

#define ADC_CS 20

//CAN Bus Creation
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> cbus1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> cbus2;
static CAN_message_t msg;
static CAN_message_t rxmsg;

ADCChip adc(ADC_CS);

ADCSensor strainGauge(7, 2450, 1);

EasyTimer sampleTimer(100000);
EasyTimer calcTimer(1000);
EasyTimer serialTimer(10);
EasyTimer sendCAN(100);

static float TORQUE_COEFF[2] = {1.0f,0.0f};
float torqueDataPoint[3] = {2400, 2500, 2600};
float strainDataPoint[3] = {2400, 2500, 2600};

int dataPointNumber = 0; // placeholder for CAN message

float torqueValue = 0;

void setup() {
  Serial.begin(112500);
  SPI.begin();
  adc.begin();
  cbus1.begin();
  cbus1.setBaudRate(1000000);
  //set_mailboxes();
  //EEPROM.put(0,TORQUE_COEFF);
  //EEPROM.get(0, TORQUE_COEFF);
  Serial.println(TORQUE_COEFF[0]);
  Serial.println(TORQUE_COEFF[1]);


}

void loop() {
  read_can1();
  int saveFlag = USER_daqSaveFlag.value();
  int board_state = (int)USER_daqBoardState.value();
  if (sampleTimer.isup()) {
    adc.sample(strainGauge); 
  }
  if(calcTimer.isup()){
    torqueValue = calculateTorque(strainGauge.avg());
    send_DAQ_00();
  }
  
  if(serialTimer.isup()){
    Serial.print("Torque_Value:");
    Serial.print(torqueValue);
    Serial.print("\t");
    Serial.print("Board_State:");
    Serial.print(USER_daqBoardState.value());
    Serial.print("\t");
    Serial.print("Save_Flag:");
    Serial.print(USER_daqSaveFlag.value());
    Serial.print("\t");
    Serial.print("Data_Point_Number:");
    Serial.print(USER_configDataPointNumber.value());
    Serial.print("\t");
    Serial.print("Torque_Data_Point:");
    Serial.println(USER_configTorqueDataPoint.value());
    Serial.print("Torque_Coeff_0:");
    Serial.print(TORQUE_COEFF[0]);
    Serial.print("\t");
    Serial.print("Torque_Coeff_1:");
    Serial.println(TORQUE_COEFF[1]);
    
  }
 // if(sendCAN.isup()){
  switch ((int)USER_daqBoardState.value()) {
    case 0: // Normal state for reading strain gauge
    
      break;

    case 1: // Simple zeroing of torque value
      TORQUE_COEFF[1] = (-1) * (strainGauge.avg() * TORQUE_COEFF[0]);
      break;

    case 2: // Full calibration of torque arm
    dataPointNumber = USER_configDataPointNumber.value();
      if (dataPointNumber != 0) {
        torqueDataPoint[dataPointNumber - 1] = USER_configTorqueDataPoint.value();
        strainDataPoint[dataPointNumber - 1] = strainGauge.avg();
      }
      break;
    case 3:
        simpLinReg(strainDataPoint, torqueDataPoint, TORQUE_COEFF);
      break;

    default:

      break;
  }
 // }
  
  if (saveFlag == 1 && USER_daqSaveFlag.is_updated()) {
    //EEPROM.put(0, TORQUE_COEFF);
  }
  
}

float calculateTorque(float strain) {
  float torqueNm = 0;
  torqueNm = (strain * TORQUE_COEFF[0]) + TORQUE_COEFF[1];
  return torqueNm;

}

void simpLinReg(float* x, float* y, float* coeff) {

  // initialize variables
  float xbar = 0;
  float ybar = 0;
  float xybar = 0;
  float xsqbar = 0;
  int n = 3;

  // calculations required for linear regression
  for (int i = 0; i < n; i++) {
    xbar = xbar + x[i];
    ybar = ybar + y[i];
    xybar = xybar + x[i] * y[i];
    xsqbar = xsqbar + x[i] * x[i];
  }
  
  xbar = xbar / n;
  ybar = ybar / n;
  xybar = xybar / n;
  xsqbar = xsqbar / n;

  // simple linear regression algorithm
  if((xsqbar - xbar * xbar)!=0.0){
  coeff[0] = (xybar - xbar * ybar) / (xsqbar - xbar * xbar);
  coeff[1] = ybar - coeff[0] * xbar;
  }
}

void send_DAQ_00() {
  msg.id = 0x48;
  msg.len = 8;

  DAQ_torqueValue = torqueValue;

  msg.buf[0] = DAQ_torqueValue.can_value();
  msg.buf[1] = DAQ_torqueValue.can_value() >> 8;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  cbus1.write(msg);
}

void read_can1() {
  if (cbus1.read(rxmsg)) {
    switch (rxmsg.id) {
      case 129:
        read_USER_request3(rxmsg);
        break;
      default:
        // Not for me -> Do Nothing
        break;
    }
  }
}
