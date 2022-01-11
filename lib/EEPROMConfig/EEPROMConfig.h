#pragma once


struct _EEPROMConfigData
{
	// Program EEPROM signature (internal)
	uint16_t _programSignature;

	// Default dim level
	uint8_t defaultDimLevel;
};


class EEPROMConfig
{
public:
  EEPROMConfig();
  ~EEPROMConfig();

  void read();
  void write();

  struct _EEPROMConfigData data;

private:
  void init();
};
