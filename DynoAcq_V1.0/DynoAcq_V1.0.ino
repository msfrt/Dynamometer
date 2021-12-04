#include <SPI.h>
#include <ReadADC.h>
#include <EasyTimer.h>

#define ADC_CS 20

ADCChip adc(ADC_CS);

ADCSensor strainGauge(8,0,1);

EasyTimer sampleTimer(100);
EasyTimer sendCAN(10);

#define TORQUE_FACTOR 1
#define TORQUE_OFFSET 0

int torqueValue = 0;

void setup() {
  Serial.begin(112500);
  SPI.begin();
  adc.begin();

}

void loop() {
  if(sampleTimer.isup()){
    adc.sample(strainGauge);
    torqueValue = calculateTorque(strainGauge.avg());
  }

}

float calculateTorque(int strain){
  uint16_t torqueNm = 0;
  torqueNm = (strain * TORQUE_FACTOR) + TORQUE_OFFSET;
  return torqueNm;

}
