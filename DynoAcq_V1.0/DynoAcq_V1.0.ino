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

#define ADC_CS 20

//CAN Bus Creation
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> cbus1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> cbus2;
static CAN_message_t msg;
static CAN_message_t rxmsg;

//CAN Signals
StateSignal DAQ_torqueValue(16, true, 10, 0, -3276.8, 3276.7, -1);
StateSignal USER_configTorqueDataPoint(16, true, 10, 0, -3276.8, 3276.7, -1);
StateSignal USER_daqSaveFlag(1, false, 1, 0, 0, 1, -1);
StateSignal USER_daqBoardState(2, false, 1, 0, 0, 3, -1);
StateSignal USER_configDataPointNumber(3, false, 1, 0, 0, 7, -1);

ADCChip adc(ADC_CS);

ADCSensor strainGauge(8, 0, 1);

EasyTimer sampleTimer(100);
EasyTimer sendCAN(10);

float TORQUE_COEFF[2] = {1, 0};
float torqueDataPoint[3] = {0, 0, 0};
int strainDataPoint[3] = {0, 0, 0};

int dataPointNumber = 0; // placeholder for CAN message

float torqueValue = 0;

void setup() {
  Serial.begin(112500);
  SPI.begin();
  adc.begin();
  cbus1.begin();
  cbus1.setBaudRate(1000000);
  //set_mailboxes();
  EEPROM.get(0, TORQUE_COEFF);
  Serial.println(TORQUE_COEFF[0]);
  Serial.println(TORQUE_COEFF[1]);

}

void loop() {
  read_can1();
  int saveFlag = USER_daqSaveFlag.value();
  if (sampleTimer.isup()) {
    adc.sample(strainGauge);
    torqueValue = calculateTorque(strainGauge.avg());
  }

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
      else {
        simpLinReg((float*)strainDataPoint, torqueDataPoint, TORQUE_COEFF);
      }
      break;

    default:

      break;
  }
  if (saveFlag == 1) {
    EEPROM.put(0, TORQUE_COEFF);
  }
  send_DAQ_00();
}

float calculateTorque(int strain) {
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
  coeff[0] = (xybar - xbar * ybar) / (xsqbar - xbar * xbar);
  coeff[1] = ybar - coeff[0] * xbar;
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

void read_USER_request3(CAN_message_t &imsg) {
  USER_configTorqueDataPoint.set_can_value((imsg.buf[0] & 0b00000011));
  USER_daqSaveFlag.set_can_value(((imsg.buf[0] >> 2) & 0b00000111));
  USER_daqBoardState.set_can_value(((imsg.buf[0] >> 5) & 0b00000001));
  USER_configDataPointNumber.set_can_value((imsg.buf[1] | (imsg.buf[2]<<8)));
}

void read_can1() {
  if (cbus1.read(rxmsg)) {
    switch (rxmsg.id) {
      case 0x81:
        read_USER_request3(rxmsg);
        break;
      default:
        // Not for me -> Do Nothing
        break;
    }
  }
}
