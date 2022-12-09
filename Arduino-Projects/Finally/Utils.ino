
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

size_t ft_strlen(char *str)
{
  size_t  index;

  index = 0;
  while (str[index] != '\0')
    index++;
  return (index);
}

/**********************************************/

size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
  size_t  index;
  size_t  srcsize;

  index = 0;
  srcsize = ft_strlen(src);
  if (dstsize != 0)
  {
    while (src[index] != '\0' && index < dstsize - 1)
    {
      dst[index] = src[index];
      index++;
    }
    dst[index] = '\0';
  }
  return (srcsize);
}

/**********************************************/

char  *ft_substr(char const *s, unsigned int start, size_t len)
{
  char  *ptr;
  size_t  size;
  size_t  val;

  if (!s)
    return (NULL);
  size = ft_strlen(s);
  if (start >= ft_strlen(s))
    return (ft_strdup(""));
  if (size - start >= len)
    val = len + 1;
  else
    val = size - start + 1;
  ptr = malloc(val * sizeof(char));
  if (!ptr)
    return (NULL);
  if (len >= SIZE_MAX)
    ft_strlcpy(ptr, &s[start], size + 1);
  else
    ft_strlcpy(ptr, (s + start), (len + 1));
  return (ptr);
}

/**********************************************/

char  *ft_strdup(char *s1)
{
  int   index;
  char  *dst;

  index = 0;
  dst = (char *)malloc((ft_strlen(s1) + 1) * sizeof(char));
  if (!dst)
    return (NULL);
  while (s1[index] != '\0')
  {
    dst[index] = s1[index];
    index++;
  }
  dst[index] = '\0';
  return (dst);
}

/**********************************************/

int  wrd_cnt(char const *s, char del)
{
  int index;
  int count;

  index = 0;
  count = 0;
  if (!s)
    return (0);
  while (s[index] != '\0')
  {
    while (s[index] == del)
      index++;
    if (s[index] != '\0')
      count++;
    while (s[index] != del && s[index] != '\0')
      index++;
  }
  return (count);
}

/**********************************************/

char  **ft_free(char **str, int x)
{
  while (x >= 0)
  {
    free(str[x]);
    x--;
  }
  free(str);
  return (NULL);
}

/**********************************************/

char  **ft_split(char const *s, char c)
{
  int   i;
  int   j;
  int   k;
  char  **str;

  i = 0;
  k = 0;
  str = (char **)malloc((wrd_cnt(s, c) + 1) * sizeof(char *));
  if (!str)
    return (NULL);
  while (k < wrd_cnt(s, c))
  {
    while (s[i] == c && s[i] != '\0')
      i++;
    j = i;
    while (s[i] != c && s[i] != '\0')
      i++;
    str[k] = ft_substr(s, j, i - j);
    if (!str[k])
      return (ft_free(str, k));
    k++;
  }
  str[k] = NULL;
  return (str);
}

/*****************************************************************************************************/
