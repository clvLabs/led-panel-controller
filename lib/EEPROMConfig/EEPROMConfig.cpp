#include <Arduino.h>
#include <EEPROM.h>
#include "EEPROMConfig.h"
#include "Config.h"


// Program EEPROM signature
#define PROGRAM_EEPROM_SIGNATURE	( 1942 )


EEPROMConfig::EEPROMConfig() {
  EEPROM.begin(512);  // ESP8266 EEPROM size
}

EEPROMConfig::~EEPROMConfig() {}

void EEPROMConfig::read()
{
	uint8_t* lCfgPtr = ( uint8_t* )&data;

	for( unsigned int liAddress = 0; liAddress < sizeof( struct _EEPROMConfigData ); liAddress++ )
	{
		lCfgPtr[liAddress] = EEPROM.read( liAddress );
	}

	// If program signature is not found, initialize EEPROM
	if( data._programSignature != PROGRAM_EEPROM_SIGNATURE )
		init();
}

void EEPROMConfig::write()
{
	uint8_t* lCfgPtr = ( uint8_t* )&data;

	for( unsigned int liAddress = 0; liAddress < sizeof( struct _EEPROMConfigData ); liAddress++ )
	{
		EEPROM.write( liAddress, lCfgPtr[liAddress] );
	}

  EEPROM.commit();
}

void EEPROMConfig::init()
{
	// Set initial configuration values
	data._programSignature = PROGRAM_EEPROM_SIGNATURE;
	data.defaultDimLevel = PANEL_DEFAULT_LEVEL;
  data.fadeSpeed = PWM_DEFAULT_FADE_SPEED;

	// Write new values to EEPROM
	write();
}
