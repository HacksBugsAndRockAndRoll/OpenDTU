#include <Arduino.h>
#include <SevenSegment.h>
#include <Hoymiles.h>

#define DASH 0x0a
#define BLANK 0x0f

SevenSegmentClass SevenSegment;

void SevenSegmentClass::init()
{
  pinMode(MAX7219_Data_IN, OUTPUT);
  pinMode(MAX7219_Chip_Select, OUTPUT);
  pinMode(MAX7219_Clock, OUTPUT);
  digitalWrite(MAX7219_Clock, HIGH);
  delay(200);

  // Setup of MAX7219 chip
  shift(0x0f, 0x00); // display test register - test mode off
  shift(0x0c, 0x01); // shutdown register - normal operation
  shift(0x0b, 0x07); // scan limit register - display digits 0 thru 7
  shift(0x0a, 0x07); // intensity register
  shift(0x09, 0xff); // decode mode register - CodeB decode all digits
  writeEmpty();
}

void SevenSegmentClass::loop()
{
  if (millis() - _lastRefresh > (_refreshInterval * 1000))
  {
    boolean wroteStats = false;
    if (Hoymiles.getNumInverters() == 0)
    {
      writeEmpty();
      _lastShownPos = 0;
    }
    else
    {
      uint8_t next = _lastShownPos + 1;
      if (next >= Hoymiles.getNumInverters())
      {
        next = 0;
      }
      auto inv = Hoymiles.getInverterByPos(next);
      _lastShownPos = next;
      for (uint8_t channel = 0; channel <= inv->Statistics()->getChannelCount(); channel++)
      {
        if (inv->Statistics()->hasChannelFieldValue(channel, FLD_PAC))
        {
          int acPower = (int)inv->Statistics()->getChannelFieldValue(channel, FLD_PAC);
          write(next, acPower);
          wroteStats = true;
          break;
        }
      }
      if (!wroteStats || inv->Statistics()->getLastUpdate() < millis() - 60000)
      {
        writeNa(next);
      }
    }
    _lastRefresh = millis();
  }
}

void SevenSegmentClass::writeEmpty()
{
  shift(0x08, DASH);
  shift(0x07, DASH);
  shift(0x06, DASH);
  shift(0x05, DASH);
  shift(0x04, DASH);
  shift(0x03, DASH);
  shift(0x02, DASH);
  shift(0x01, DASH);
}

void SevenSegmentClass::shift(byte send_to_address, byte send_this_data)
{
  digitalWrite(MAX7219_Chip_Select, LOW);
  shiftOut(MAX7219_Data_IN, MAX7219_Clock, MSBFIRST, send_to_address);
  shiftOut(MAX7219_Data_IN, MAX7219_Clock, MSBFIRST, send_this_data);
  digitalWrite(MAX7219_Chip_Select, HIGH);
}

void SevenSegmentClass::writeNa(int inverter)
{
  writeInverter(inverter);
  shift(0x05, BLANK);
  shift(0x04, DASH);
  shift(0x03, DASH);
  shift(0x02, DASH);
  shift(0x01, DASH);
}

void SevenSegmentClass::writeInverter(int inverter)
{
  inverter++; // start at 1
  // inverter
  shift(0x08, inverter / 10);
  shift(0x07, inverter % 10);
  shift(0x06, DASH); // blinking dash as activity indicator
}

void SevenSegmentClass::write(int inverter, int acPower)
{
  Serial.printf("write inv: %d, power: %d \n",inverter,acPower);

  writeInverter(inverter);
  // power
  shift(0x05, acPower / 10000 == 0 ? BLANK : acPower / 10000);
  acPower = acPower % 10000;
  shift(0x04, acPower / 1000 == 0 ? BLANK : acPower / 1000);
  acPower = acPower % 1000;
  shift(0x03, acPower / 100 == 0 ? BLANK : acPower / 100);
  acPower = acPower % 100;
  shift(0x02, (acPower / 10));
  acPower = acPower % 10;
  shift(0x01, acPower);
}
