#include <DS3231.h>

// Init the DS3231 using the hardware interface
DS3231 rtc(SDA, SCL);

void rtcSetup()
{
  rtc.begin();
}

String getDateRtc()
{
  return rtc.getDateStr();
}

String getTimeRtc()
{
  return rtc.getTimeStr();
}

void checkRTC()
{
  Serial.print(getDateRtc() + " ");
  Serial.print(" ");
  Serial.println(getTimeRtc());
  delay(1000);
}

String convertDate(String date)
{
  char date_A[12];
  date.toCharArray(date_A, 12);
  for (int i = 2; i < 12; i++)
  {
    if (i < 4)
    {
      date_A[i] = date_A[i + 1];
    }
    else if (i < 12)
    {
      date_A[i] = date_A[i + 2];
    }
  }
  String str(date_A);
  return str;
}
