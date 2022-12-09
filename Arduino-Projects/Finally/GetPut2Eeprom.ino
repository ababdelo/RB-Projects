
/**************************************Store UID to the EEPROM***************************************/

void Store2Eeprom(int *address, char* ID)
{
  size_t  len = ft_strlen(ID);
  EEPROM.write(address, len);
  for (size_t x = 0; x < len; x++)
  { if (EEPROM.read(address + 1 + x) == "")
    {
      successStore = true;
      EEPROM.write(address + 1 + x, ID[x]);
    }
    else
    {
      successStore = false;
      address = address + len;
    }
  }
}

/*****************************************************************************************************/

/**************************************Store From the EEPROM******************************************/

char *ReadfromEeprom(int *address)
{
  size_t  len = EEPROM.read(address);
  for (size_t x = 0; x < len; x++)
  {
    StoredUID[x] = EEPROM.read(address + x);
  }

}

/*****************************************************************************************************/
