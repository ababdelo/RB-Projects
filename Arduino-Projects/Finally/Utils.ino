
/************************Display to the lcd*******************************/
void StandByMsg() {
  delay(1500);
  lcd.clear();
  lcd.print("Access Control");
  lcd.setCursor(0, 1);
  lcd.print(" Scan Your Tag!");
}
/**********************************************/

/**********************************************/
void access() {
  digitalWrite(BUZ, HIGH);
  digitalWrite(GRLED, HIGH);
  delay(100);
  digitalWrite(BUZ, LOW);
  digitalWrite(GRLED, LOW);
  delay(100);
  digitalWrite(BUZ, HIGH);
  digitalWrite(GRLED, HIGH);
  delay(200);
  digitalWrite(BUZ, LOW);
  digitalWrite(GRLED, LOW);
}
/**********************************************/

/**********************************************/
void denied() {
  digitalWrite(BUZ, HIGH);
  digitalWrite(RDLED, HIGH);
  delay(700);
  digitalWrite(BUZ, LOW);
  digitalWrite(RDLED, LOW);
}
/**********************************************/

/**************Open Door***********************/
void OpenDoor() {
  //Lock opening function open for 3s
  lcd.clear();
  lcd.print("     Welcome    ");  //With a message printed
  digitalWrite(DOOR, 1);
  access();
  delay(2500);
  digitalWrite(DOOR, 0);
  lcd.clear();
}
/*********************************************************************/

/*****************************************************************************************************/
int  ft_strstr(String str, String to_f)
{
  size_t  x;
  size_t  y;

  x = 0;
  if (to_f[0] == 0)
    return ;
  while (str[x] != '\0')
  {
    y = 0;
    while (str[x + y] == to_f[y])
    {
      if (to_f[y + 1] == '\0')
        return ((char *) &str[x]);
      y++;
    }
    x++;
  }
  return (x);
}

/*********************************************************************/

void  ft_bzero(void *s, size_t n)
{
  char  *c;
  size_t  index;

  index = 0;
  c = s;
  while (index < n)
  {
    c[index] = 0;
    index++;
  }
}
/*****************************************************************************************************/
