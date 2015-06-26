//Hämtade bibliotek
#include <SoftwareSerial.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <LedControl.h>

//Mickes egna bibliotek
#include "RS485WithErrChk.h"
#include "SendReciveRemoteTemp.h"
#include "MickeDallasTemperature.h"
#include "MickesLedControl.h"
#include "TemperatureDisplay.h"

/*
 * HusetEnhet.ino
 *
 * Created: 5/15/2015 4:59:20 PM
 * Author: Micke
 */ 

//Pin nummer konfigurationen som enheterna är
//inkopplade på
#define LEDCONTROL_DATA_PIN 9
#define LEDCONTROL_CLOCK_PIN 8
#define LEDCONTROL_CS_PIN 7

#define RS485_SERIAL_RX_PIN       10  //Serial Receive pin
#define RS485_SERIAL_TX_PIN        11  //Serial Transmit pin
#define RS485_SERIAL_CTRL_PIN 3   //RS485 Direction control

//#define ONE_WIRE_CONTROL_PIN 4

//Antal inkopplade MAX 7219 ledcontrol enheter
#define NUM_OF_LEDCONTROLS 2

//Inställningar för R485 och Remote send/recive
#define RS485_SERIAL_COM_SPEED 4800
#define NUM_OF_REMOTE_SENSORS 3
#define REQUEST_TIMEOUT 5000

//Skapa de globala objekten
MickeDebug mDebug(100);

RS485WithErrChk rs485 = RS485WithErrChk(RS485_SERIAL_RX_PIN,RS485_SERIAL_TX_PIN,RS485_SERIAL_CTRL_PIN);
SendReciveRemoteTemp sendRecvRTemp(&rs485,NUM_OF_REMOTE_SENSORS,REQUEST_TIMEOUT);

//Skapa det globala Micke Led control objektet
MickesLedControl MickeLC(LEDCONTROL_DATA_PIN,LEDCONTROL_CLOCK_PIN,
LEDCONTROL_CS_PIN,NUM_OF_LEDCONTROLS);

//Skapa led display objekt.
TemperatureDisplay leftDisplay = TemperatureDisplay(0,0,4,MickeLC);
TemperatureDisplay midDisplay = TemperatureDisplay(1,0,0,MickeLC);
TemperatureDisplay rightDisplay = TemperatureDisplay(2,1,0,MickeLC);


void setup()
{
	Serial.begin(9600);
	rs485.InitRs485ComPort(RS485_SERIAL_COM_SPEED);
	
	//Starta upp led displayerna;
	MickeLC.initLedDisplay(0,5);
	MickeLC.initLedDisplay(1,5);
	
		boolean b;
		b = sendRecvRTemp.sendRemoteRequest(TRUE);

}

void loop()
{
	float sjoTemp;
	float bastuTemp;
	float uteTemp;
	
	boolean b;
	b = sendRecvRTemp.sendRemoteRequest(0);
	mDebug.serialDebugLine("Send request",b);
	
	String sTemp;
	sTemp = sendRecvRTemp.checkForRemoteData();
	mDebug.serialDebugLine("check Remotedata",sTemp);
	
	
	
	sjoTemp = sendRecvRTemp.getRemoteTemperature(0);
	bastuTemp = sendRecvRTemp.getRemoteTemperature(1);
	uteTemp = sendRecvRTemp.getRemoteTemperature(2);
	
	rightDisplay.showTemperature(sjoTemp);
	midDisplay.showTemperature(bastuTemp);
	leftDisplay.showTemperature(uteTemp);
	
	mDebug.serialDebugLine("Sjo Temp",sjoTemp);
	mDebug.serialDebugLine("Bastu Temp",bastuTemp);
	mDebug.serialDebugLine("Ute Temp",uteTemp);
}
