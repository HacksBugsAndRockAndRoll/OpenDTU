#include <Arduino.h>
#ifndef MAX7219_Data_IN
#define MAX7219_Data_IN 26
#endif
#ifndef MAX7219_Chip_Select
#define MAX7219_Chip_Select  27
#endif
#ifndef MAX7219_Clock
#define MAX7219_Clock 25
#endif

class SevenSegmentClass {

  public:
    void init();
    void loop();
  private:
    void write(int inverter, int acPower);
    void writeEmpty();
    void shift(byte send_to_address, byte send_this_data);
    uint32_t _refreshInterval = 10;
    uint32_t _lastRefresh = 0;
};
extern SevenSegmentClass SevenSegment;