/******************************************************************************
    
    This file was generated automatically from a DBC file by the dbctocpp 
    utility, which is part of the StateCAN library, 
    a component of SR-Libraries.

    https://github.com/msfrt/SR-Libraries
    
******************************************************************************/

#ifndef C_GIT_DYNAMOMETER_DYNOACQ_V1_0_DYNO_HPP 
#define C_GIT_DYNAMOMETER_DYNOACQ_V1_0_DYNO_HPP 

#include <FlexCAN_T4.h>
#include <StateCAN.h>

// Message: USER_request3 [0x81]
StateSignal USER_daqBoardState(2, false, 1, 0.0, 0, 0, 0.0, 0, 129);
StateSignal USER_configDataPointNumber(3, false, 1, 0.0, 0, 0, 0.0, 0, 129);
StateSignal USER_daqSaveFlag(1, false, 1, 0.0, 0, 0, 0.0, 0, 129);
StateSignal USER_configTorqueDataPoint(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 129);

// Message: DAQ_00 [0x48]
StateSignal DAQ_torqueValue(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 72);

// Message: USER_request2 [0x80]
StateSignal USER_throttleKp(16, true, 100, 0.0, -327, 327, 0.0, 0, 128);
StateSignal USER_throttleKi(16, true, 100, 0.0, -327, 327, 0.0, 0, 128);
StateSignal USER_throttleKd(16, true, 100, 0.0, -327, 327, 0.0, 0, 128);

// Message: ETC_output [0x79]
StateSignal ETC_status(8, true, 1, 0.0, -128, 127, 0.0, 0, 121);
StateSignal ETC_throttlePosition(16, true, 10, 0.0, 0, 100, 0.0, 0, 121);
StateSignal ETC_servoOutput(16, true, 1, 0.0, 0, 255, 0.0, 0, 121);

// Message: USER_request1 [0x78]
StateSignal USER_throttleRequest(16, true, 10, 0.0, 0, 100, 0.0, 0, 120);
StateSignal USER_rpmRequest(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 120);
StateSignal USER_wpOverrideStatus(8, true, 1, 0.0, -128, 127, 0.0, 0, 120);
StateSignal USER_wpOverridePercent(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 120);

// Message: M400_dataSet2 [0x5ef]
StateSignal M400_inletAirTemp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_ignOut2DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_ignETComp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_ignATComp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelStartingComp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelLambdaComp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelInjDutyCycle(16, true, 10, 0.0, 0, 0, 0.0, 0, 1519);
StateSignal M400_fuelETComp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelComp2(16, true, 10, 0.0, 0, 0, 0.0, 0, 1519);
StateSignal M400_fuelATComp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelAccel(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_fuel3PulseWidth(16, true, 500, 0.0, 0, 0, 0.0, 0, 1519);
StateSignal M400_fuel2IndivTrim(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_exhaustGasTemp4(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_exhaustGasTemp1(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_ecuCPUusage(16, true, 10, 0.0, 0, 0, 0.0, 0, 1519);
StateSignal M400_auxOut7DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_auxOut4DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_auxOut1DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_la1LongTermTrim(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_ignOut3DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_ignMAPComp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_ignComp1(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelTemp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelMAPComp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelInjDutyPrim(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_fuelFPComp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelDecel(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_fuelBasePulseW(16, true, 500, 0.0, -65, 65, 0.0, 0, 1519);
StateSignal M400_fuelAccelClamp(16, true, 500, 0.0, 0, 0, 0.0, 0, 1519);
StateSignal M400_fuel4IndivTrim(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_fuel2PulseWidth(16, true, 500, 0.0, 0, 0, 0.0, 0, 1519);
StateSignal M400_fuel1IndivTrim(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_exhaustGasTemp2(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_ecuInternalTemp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_auxOut8DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_auxOut5DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_auxOut2DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_oilTemp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_ignOut4DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_ignOut1DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_ignComp2(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelUsed(16, true, 100, 0.0, 0, 0, 0.0, 0, 1519);
StateSignal M400_fuelPressure(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelInjDutySec(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_fuelFTComp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_fuelDecelClamp(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_fuelComp1(16, true, 10, 0.0, 0, 0, 0.0, 0, 1519);
StateSignal M400_fuelActualPulseW(16, true, 500, 0.0, 0, 0, 0.0, 0, 1519);
StateSignal M400_fuel4PulseWidth(16, true, 500, 0.0, 0, 0, 0.0, 0, 1519);
StateSignal M400_fuel3IndivTrim(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_fuel1PulseWidth(16, true, 500, 0.0, 0, 0, 0.0, 0, 1519);
StateSignal M400_exhaustGasTemp3(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_engineTemp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1519);
StateSignal M400_batteryVoltage(16, true, 100, 0.0, -327, 327, 0.0, 0, 1519);
StateSignal M400_auxOut6DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);
StateSignal M400_auxOut3DutyCycle(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1519);

// Message: M400_dataSet1 [0x5f0]
StateSignal M400_throttlePosition(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_manifoldPressure(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_la1AimValue(16, true, 1000, 0.0, -32, 32, 0.0, 0, 1520);
StateSignal M400_ignOverallTrim(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_ignDecel(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1520);
StateSignal M400_ign4IndivTrim(16, true, 5, 0.0, -6553, 6553, 0.0, 0, 1520);
StateSignal M400_ign3Advance(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_ign1IndivTrim(16, true, 5, 0.0, -6553, 6553, 0.0, 0, 1520);
StateSignal M400_gearDet(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1520);
StateSignal M400_fuelInjTimingSec(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_fuelCutLevelTotal(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_oilPressure(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_la1ShortTermTrim(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_ignDwellTime(16, true, 500, 0.0, -65, 65, 0.0, 0, 1520);
StateSignal M400_ignBaseAdvance(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_ignAccel(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1520);
StateSignal M400_ign3IndivTrim(16, true, 5, 0.0, -6553, 6553, 0.0, 0, 1520);
StateSignal M400_ign2Advance(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_gearSensorVoltage(16, true, 1000, 0.0, -32, 32, 0.0, 0, 1520);
StateSignal M400_fuelOverallTrim(16, true, 10, 0.0, 0, 0, 0.0, 0, 1520);
StateSignal M400_fuelInjTiming(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_rpm(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1520);
StateSignal M400_lambda1(16, true, 1000, 0.0, -32, 32, 0.0, 0, 1520);
StateSignal M400_knock1(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1520);
StateSignal M400_ignCutLevelTotal(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1520);
StateSignal M400_ignAdvance(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_ign4Advance(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_ign2IndivTrim(16, true, 5, 0.0, -6553, 6553, 0.0, 0, 1520);
StateSignal M400_ign1Advance(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 1520);
StateSignal M400_gear(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1520);
StateSignal M400_fuelInjTimingComp(16, true, 1, 0.0, -32768, 32767, 0.0, 0, 1520);

// Message: PDM_13 [0xa3]
StateSignal PDM_counter13(4, false, 1, 0.0, 0, 0, 0.0, 0, 163);
StateSignal PDM_wpPWM(8, false, 1, 0.0, 0, 255, 0.0, 0, 163);
StateSignal PDM_boardTemp(16, true, 10, 0.0, -3276, 3276, 0.0, 0, 163);

// Message: PDM_12 [0xa2]
StateSignal PDM_counter12(4, false, 1, 0.0, 0, 0, 0.0, 0, 162);
StateSignal PDM_dataVoltMax(16, true, 1000, 0.0, -32, 32, 0.0, 0, 162);
StateSignal PDM_dataVoltAvg(16, true, 1000, 0.0, -32, 32, 0.0, 0, 162);
StateSignal PDM_dataVoltMin(16, true, 1000, 0.0, -32, 32, 0.0, 0, 162);

// Message: PDM_11 [0xa1]
StateSignal PDM_counter11(4, false, 1, 0.0, 0, 0, 0.0, 0, 161);
StateSignal PDM_mainVoltMax(16, true, 1000, 0.0, -32, 32, 0.0, 0, 161);
StateSignal PDM_mainVoltAvg(16, true, 1000, 0.0, -32, 32, 0.0, 0, 161);
StateSignal PDM_mainVoltMin(16, true, 1000, 0.0, -32, 32, 0.0, 0, 161);

// Message: PDM_10 [0xa0]
StateSignal PDM_counter10(4, false, 1, 0.0, 0, 0, 0.0, 0, 160);
StateSignal PDM_fuelVoltMax(16, true, 1000, 0.0, -32, 32, 0.0, 0, 160);
StateSignal PDM_fuelVoltAvg(16, true, 1000, 0.0, -32, 32, 0.0, 0, 160);
StateSignal PDM_fuelVoltMin(16, true, 1000, 0.0, -32, 32, 0.0, 0, 160);

// Message: PDM_09 [0x9f]
StateSignal PDM_counter09(4, false, 1, 0.0, 0, 0, 0.0, 0, 159);
StateSignal PDM_pdmVoltMax(16, true, 1000, 0.0, -32, 32, 0.0, 0, 159);
StateSignal PDM_pdmVoltAvg(16, true, 1000, 0.0, -32, 32, 0.0, 0, 159);
StateSignal PDM_pdmVoltMin(16, true, 1000, 0.0, -32, 32, 0.0, 0, 159);

// Message: PDM_08 [0x9e]
StateSignal PDM_counter08(4, false, 1, 0.0, 0, 0, 0.0, 0, 158);
StateSignal PDM_wpVoltMax(16, true, 1000, 0.0, -32, 32, 0.0, 0, 158);
StateSignal PDM_wpVoltAvg(16, true, 1000, 0.0, -32, 32, 0.0, 0, 158);
StateSignal PDM_wpOverrideStatus(8, true, 1, 0.0, -128, 127, 0.0, 0, 158);

// Message: PDM_03 [0x99]
StateSignal PDM_counter03(4, false, 1, 0.0, 0, 15, 0.0, 0, 153);
StateSignal PDM_wpCurrentMax(16, true, 100, 0.0, -327, 327, 0.0, 0, 153);
StateSignal PDM_wpCurrentAvg(16, true, 100, 0.0, -327, 327, 0.0, 0, 153);
StateSignal PDM_wpCurrentMin(16, true, 100, 0.0, -327, 327, 0.0, 0, 153);

// Message: PDM_04 [0x9a]
StateSignal PDM_counter04(4, false, 1, 0.0, 0, 0, 0.0, 0, 154);
StateSignal PDM_pdmCurrentMax(16, true, 100, 0.0, -327, 327, 0.0, 0, 154);
StateSignal PDM_pdmCurrentAvg(16, true, 100, 0.0, -327, 327, 0.0, 0, 154);
StateSignal PDM_pdmCurrentMin(16, true, 100, 0.0, -327, 327, 0.0, 0, 154);

// Message: PDM_05 [0x9b]
StateSignal PDM_counter05(4, false, 1, 0.0, 0, 0, 0.0, 0, 155);
StateSignal PDM_fuelCurrentMax(16, true, 100, 0.0, -327, 327, 0.0, 0, 155);
StateSignal PDM_fuelCurrentAvg(16, true, 100, 0.0, -327, 327, 0.0, 0, 155);
StateSignal PDM_fuelCurrentMin(16, true, 100, 0.0, -327, 327, 0.0, 0, 155);

// Message: PDM_00 [0x96]
StateSignal PDM_counter00(4, false, 1, 0.0, 0, 15, 0.0, 0, 150);
StateSignal PDM_status(4, false, 1, 0.0, 0, 15, 0.0, 0, 150);
StateSignal PDM_wpStatus(4, false, 1, 0.0, 0, 15, 0.0, 0, 150);




/************************************************************************************
    
    Incoming CAN frame decoding functions

************************************************************************************/


/*
 * Decode a CAN frame for the message USER_request3
 * \param imsg A reference to the incoming CAN message frame
 */
void read_USER_request3(const CAN_message_t &imsg) {

	USER_daqBoardState.set_can_value(((imsg.buf[0] & 0b00000011)));
	USER_configDataPointNumber.set_can_value((((imsg.buf[0] & 0b00011100)) >> 2));
	USER_daqSaveFlag.set_can_value((((imsg.buf[0] & 0b00100000)) >> 5));
	USER_configTorqueDataPoint.set_can_value((imsg.buf[1]) | (imsg.buf[2] << 8));

}

/*
 * Decode a CAN frame for the message DAQ_00
 * \param imsg A reference to the incoming CAN message frame
 */
void read_DAQ_00(const CAN_message_t &imsg) {

	DAQ_torqueValue.set_can_value((imsg.buf[0]) | (imsg.buf[1] << 8));

}

/*
 * Decode a CAN frame for the message USER_request2
 * \param imsg A reference to the incoming CAN message frame
 */
void read_USER_request2(const CAN_message_t &imsg) {

	USER_throttleKp.set_can_value((imsg.buf[0]) | (imsg.buf[1] << 8));
	USER_throttleKi.set_can_value((imsg.buf[2]) | (imsg.buf[3] << 8));
	USER_throttleKd.set_can_value((imsg.buf[4]) | (imsg.buf[5] << 8));

}

/*
 * Decode a CAN frame for the message ETC_output
 * \param imsg A reference to the incoming CAN message frame
 */
void read_ETC_output(const CAN_message_t &imsg) {

	ETC_status.set_can_value((imsg.buf[0]));
	ETC_throttlePosition.set_can_value((imsg.buf[1]) | (imsg.buf[2] << 8));
	ETC_servoOutput.set_can_value((imsg.buf[3]) | (imsg.buf[4] << 8));

}

/*
 * Decode a CAN frame for the message USER_request1
 * \param imsg A reference to the incoming CAN message frame
 */
void read_USER_request1(const CAN_message_t &imsg) {

	USER_throttleRequest.set_can_value((imsg.buf[0]) | (imsg.buf[1] << 8));
	USER_rpmRequest.set_can_value((imsg.buf[2]) | (imsg.buf[3] << 8));
	USER_wpOverrideStatus.set_can_value((imsg.buf[4]));
	USER_wpOverridePercent.set_can_value((imsg.buf[5]) | (imsg.buf[6] << 8));

}

/*
 * Decode a CAN frame for the message M400_dataSet2
 * \param imsg A reference to the incoming CAN message frame
 */
void read_M400_dataSet2(const CAN_message_t &imsg) {

	// multiplexer signal
	int M400_rowCounterDataSet2 = imsg.buf[0];

	switch (M400_rowCounterDataSet2) {

		case 0:
			M400_auxOut1DutyCycle.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_auxOut2DutyCycle.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_auxOut3DutyCycle.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 1:
			M400_auxOut4DutyCycle.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_auxOut5DutyCycle.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_auxOut6DutyCycle.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 2:
			M400_auxOut7DutyCycle.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_auxOut8DutyCycle.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_batteryVoltage.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 3:
			M400_ecuCPUusage.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_ecuInternalTemp.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_engineTemp.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 4:
			M400_exhaustGasTemp1.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_exhaustGasTemp2.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_exhaustGasTemp3.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 5:
			M400_exhaustGasTemp4.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuel1IndivTrim.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_fuel1PulseWidth.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 6:
			M400_fuel2IndivTrim.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuel2PulseWidth.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_fuel3IndivTrim.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 7:
			M400_fuel3PulseWidth.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuel4IndivTrim.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_fuel4PulseWidth.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 8:
			M400_fuelAccel.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuelAccelClamp.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_fuelActualPulseW.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 9:
			M400_fuelATComp.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuelBasePulseW.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_fuelComp1.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 10:
			M400_fuelComp2.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuelDecel.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_fuelDecelClamp.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 11:
			M400_fuelETComp.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuelFPComp.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_fuelFTComp.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 12:
			M400_fuelInjDutyCycle.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuelInjDutyPrim.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_fuelInjDutySec.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 13:
			M400_fuelLambdaComp.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuelMAPComp.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_fuelPressure.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 14:
			M400_fuelStartingComp.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuelTemp.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_fuelUsed.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 15:
			M400_ignATComp.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_ignComp1.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_ignComp2.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 16:
			M400_ignETComp.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_ignMAPComp.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_ignOut1DutyCycle.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 17:
			M400_ignOut2DutyCycle.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_ignOut3DutyCycle.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_ignOut4DutyCycle.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 18:
			M400_inletAirTemp.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_la1LongTermTrim.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_oilTemp.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

	}

}

/*
 * Decode a CAN frame for the message M400_dataSet1
 * \param imsg A reference to the incoming CAN message frame
 */
void read_M400_dataSet1(const CAN_message_t &imsg) {

	// multiplexer signal
	int M400_rowCounterDataSet1 = imsg.buf[0];

	switch (M400_rowCounterDataSet1) {

		case 0:
			M400_fuelCutLevelTotal.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuelInjTiming.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_fuelInjTimingComp.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 1:
			M400_fuelInjTimingSec.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_fuelOverallTrim.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_gear.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 2:
			M400_gearDet.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_gearSensorVoltage.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_ign1Advance.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 3:
			M400_ign1IndivTrim.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_ign2Advance.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_ign2IndivTrim.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 4:
			M400_ign3Advance.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_ign3IndivTrim.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_ign4Advance.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 5:
			M400_ign4IndivTrim.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_ignBaseAdvance.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_ignCutLevelTotal.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 6:
			M400_ignOverallTrim.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_ignAccel.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_ignAdvance.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 7:
			M400_ignDecel.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_ignDwellTime.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_knock1.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 8:
			M400_la1AimValue.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_la1ShortTermTrim.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_lambda1.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 9:
			M400_manifoldPressure.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			M400_oilPressure.set_can_value((imsg.buf[5]) | (imsg.buf[4] << 8));
			M400_rpm.set_can_value((imsg.buf[7]) | (imsg.buf[6] << 8));
			break;

		case 10:
			M400_throttlePosition.set_can_value((imsg.buf[3]) | (imsg.buf[2] << 8));
			break;

	}

}

/*
 * Decode a CAN frame for the message PDM_13
 * \param imsg A reference to the incoming CAN message frame
 */
void read_PDM_13(const CAN_message_t &imsg) {

	PDM_counter13.set_can_value(((imsg.buf[0] & 0b00001111)));
	PDM_wpPWM.set_can_value((imsg.buf[3]));
	PDM_boardTemp.set_can_value((imsg.buf[4]) | (imsg.buf[5] << 8));

}

/*
 * Decode a CAN frame for the message PDM_12
 * \param imsg A reference to the incoming CAN message frame
 */
void read_PDM_12(const CAN_message_t &imsg) {

	PDM_counter12.set_can_value(((imsg.buf[0] & 0b00001111)));
	PDM_dataVoltMax.set_can_value((imsg.buf[1]) | (imsg.buf[2] << 8));
	PDM_dataVoltAvg.set_can_value((imsg.buf[3]) | (imsg.buf[4] << 8));
	PDM_dataVoltMin.set_can_value((imsg.buf[5]) | (imsg.buf[6] << 8));

}

/*
 * Decode a CAN frame for the message PDM_11
 * \param imsg A reference to the incoming CAN message frame
 */
void read_PDM_11(const CAN_message_t &imsg) {

	PDM_counter11.set_can_value(((imsg.buf[0] & 0b00001111)));
	PDM_mainVoltMax.set_can_value((imsg.buf[1]) | (imsg.buf[2] << 8));
	PDM_mainVoltAvg.set_can_value((imsg.buf[3]) | (imsg.buf[4] << 8));
	PDM_mainVoltMin.set_can_value((imsg.buf[5]) | (imsg.buf[6] << 8));

}

/*
 * Decode a CAN frame for the message PDM_10
 * \param imsg A reference to the incoming CAN message frame
 */
void read_PDM_10(const CAN_message_t &imsg) {

	PDM_counter10.set_can_value(((imsg.buf[0] & 0b00001111)));
	PDM_fuelVoltMax.set_can_value((imsg.buf[1]) | (imsg.buf[2] << 8));
	PDM_fuelVoltAvg.set_can_value((imsg.buf[3]) | (imsg.buf[4] << 8));
	PDM_fuelVoltMin.set_can_value((imsg.buf[5]) | (imsg.buf[6] << 8));

}

/*
 * Decode a CAN frame for the message PDM_09
 * \param imsg A reference to the incoming CAN message frame
 */
void read_PDM_09(const CAN_message_t &imsg) {

	PDM_counter09.set_can_value(((imsg.buf[0] & 0b00001111)));
	PDM_pdmVoltMax.set_can_value((imsg.buf[1]) | (imsg.buf[2] << 8));
	PDM_pdmVoltAvg.set_can_value((imsg.buf[3]) | (imsg.buf[4] << 8));
	PDM_pdmVoltMin.set_can_value((imsg.buf[5]) | (imsg.buf[6] << 8));

}

/*
 * Decode a CAN frame for the message PDM_08
 * \param imsg A reference to the incoming CAN message frame
 */
void read_PDM_08(const CAN_message_t &imsg) {

	PDM_counter08.set_can_value(((imsg.buf[0] & 0b00001111)));
	PDM_wpVoltMax.set_can_value((imsg.buf[1]) | (imsg.buf[2] << 8));
	PDM_wpVoltAvg.set_can_value((imsg.buf[3]) | (imsg.buf[4] << 8));
	PDM_wpOverrideStatus.set_can_value((imsg.buf[5]));

}

/*
 * Decode a CAN frame for the message PDM_03
 * \param imsg A reference to the incoming CAN message frame
 */
void read_PDM_03(const CAN_message_t &imsg) {

	PDM_counter03.set_can_value(((imsg.buf[0] & 0b00001111)));
	PDM_wpCurrentMax.set_can_value((imsg.buf[1]) | (imsg.buf[2] << 8));
	PDM_wpCurrentAvg.set_can_value((imsg.buf[3]) | (imsg.buf[4] << 8));
	PDM_wpCurrentMin.set_can_value((imsg.buf[5]) | (imsg.buf[6] << 8));

}

/*
 * Decode a CAN frame for the message PDM_04
 * \param imsg A reference to the incoming CAN message frame
 */
void read_PDM_04(const CAN_message_t &imsg) {

	PDM_counter04.set_can_value(((imsg.buf[0] & 0b00001111)));
	PDM_pdmCurrentMax.set_can_value((imsg.buf[1]) | (imsg.buf[2] << 8));
	PDM_pdmCurrentAvg.set_can_value((imsg.buf[3]) | (imsg.buf[4] << 8));
	PDM_pdmCurrentMin.set_can_value((imsg.buf[5]) | (imsg.buf[6] << 8));

}

/*
 * Decode a CAN frame for the message PDM_05
 * \param imsg A reference to the incoming CAN message frame
 */
void read_PDM_05(const CAN_message_t &imsg) {

	PDM_counter05.set_can_value(((imsg.buf[0] & 0b00001111)));
	PDM_fuelCurrentMax.set_can_value((imsg.buf[1]) | (imsg.buf[2] << 8));
	PDM_fuelCurrentAvg.set_can_value((imsg.buf[3]) | (imsg.buf[4] << 8));
	PDM_fuelCurrentMin.set_can_value((imsg.buf[5]) | (imsg.buf[6] << 8));

}

/*
 * Decode a CAN frame for the message PDM_00
 * \param imsg A reference to the incoming CAN message frame
 */
void read_PDM_00(const CAN_message_t &imsg) {

	PDM_counter00.set_can_value(((imsg.buf[0] & 0b00001111)));
	PDM_status.set_can_value(((imsg.buf[1] & 0b00001111)));
	PDM_wpStatus.set_can_value((((imsg.buf[2] & 0b11110000)) >> 4));

}




/************************************************************************************

    Distribute incoming messages to the correct decoding functions

************************************************************************************/


/*
 * Decode a CAN message for the bus captured in DYNO.dbc.
 * To more efficiently allocate microcontroller resources, simply comment
 * out unnecessary messages that do not need to be decoded.
 * \param imsg A reference to the incoming CAN frame
 */
void decode_DYNO(const CAN_message_t &imsg) {

	switch (imsg.id) {

		case 129:
			read_USER_request3(imsg);
			break;

		case 72:
			read_DAQ_00(imsg);
			break;

		case 128:
			read_USER_request2(imsg);
			break;

		case 121:
			read_ETC_output(imsg);
			break;

		case 120:
			read_USER_request1(imsg);
			break;

		case 1519:
			read_M400_dataSet2(imsg);
			break;

		case 1520:
			read_M400_dataSet1(imsg);
			break;

		case 163:
			read_PDM_13(imsg);
			break;

		case 162:
			read_PDM_12(imsg);
			break;

		case 161:
			read_PDM_11(imsg);
			break;

		case 160:
			read_PDM_10(imsg);
			break;

		case 159:
			read_PDM_09(imsg);
			break;

		case 158:
			read_PDM_08(imsg);
			break;

		case 153:
			read_PDM_03(imsg);
			break;

		case 154:
			read_PDM_04(imsg);
			break;

		case 155:
			read_PDM_05(imsg);
			break;

		case 150:
			read_PDM_00(imsg);
			break;

	}
}


#endif
