
/**************************************Store UID to the EEPROM***************************************/

void write2eeprom(int address, char* ID, char sep)
{
  int index = 0;
  while (ID[index] != '\0')
  {
    if (EEPROM.read(address) == "")
      EEPROM.write(address, ID[index]);
    address++;
    index++;
  }
  EEPROM.write(address, sep);
}

/*****************************************************************************************************/

/**************************************Store From the EEPROM******************************************/

char *readfromeeprom(int address, char sep)
{
  int index = 0;
  while (EEPROM.read(address) != "" && EEPROM.read(address) != sep)
  {
    StoredUID[index] = EEPROM.read(address);
    address++;
    index++;
  }
  StoredUID[index] = '\0';
  return (StoredUID);
}

/*****************************************************************************************************/
