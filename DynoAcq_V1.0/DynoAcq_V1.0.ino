#include <EEPROM.h>
#include <SPI.h>
#include <ReadADC.h>
#include <EasyTimer.h>

#define ADC_CS 20

ADCChip adc(ADC_CS);

ADCSensor strainGauge(8, 0, 1);

EasyTimer sampleTimer(100);
EasyTimer sendCAN(10);

float TORQUE_COEFF[2] = {1, 0};
float torqueDataPoint[3] = {0, 0, 0};
int strainDataPoint[3] = {0, 0, 0};

int dataPointNumber = 0; // placeholder for CAN message

float torqueValue = 0;

byte boardState = 0;

void setup() {
  Serial.begin(112500);
  SPI.begin();
  adc.begin();
  EEPROM.get(0,TORQUE_COEFF);
  Serial.println(TORQUE_COEFF[0]);
  Serial.println(TORQUE_COEFF[1]);

}

void loop() {
int saveFlag = 0;
  if (sampleTimer.isup()) {
    adc.sample(strainGauge);
    torqueValue = calculateTorque(strainGauge.avg());
  }

  switch (boardState) {
    case 0: // Normal state for reading strain gauge

      break;

    case 1: // Simple zeroing of torque value
      TORQUE_COEFF[1] = (-1) * (strainGauge.avg() * TORQUE_COEFF[0]);
      break;

    case 2: // Full calibration of torque arm
      if (dataPointNumber != 0) {
        torqueDataPoint[dataPointNumber - 1] = 9999; // 99999 placeholder for message data
        strainDataPoint[dataPointNumber - 1] = strainGauge.avg();
      }
      else {
        simpLinReg((float*)strainDataPoint,torqueDataPoint,TORQUE_COEFF);
      }
      break;

    default:

      break;
  }
  if (saveFlag == 1){
    EEPROM.put(0,TORQUE_COEFF);
  }
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
