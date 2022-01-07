#pragma once


struct _EEPROMConfigData
{
	// Program EEPROM signature (internal)
	uint16_t _programSignature;

	// Current dim level
	uint8_t level;
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
