
/**************************************Store UID to the EEPROM***************************************/

bool write2eeprom(int address, char* ID, char sep)
{
  int index = 0;
  while (ID[index] != '\0')
  {
    if (EEPROM.read(address) == "")
    {
      successStore = true;
      EEPROM.write(address, ID[index]);
    }
    else
      successStore = false;
    address++;
    index++;
  }
  EEPROM.write(address, sep);
  return (successStore);
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
