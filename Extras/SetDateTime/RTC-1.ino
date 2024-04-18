#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "hardware/rtc.h"

char datetime_buf[256];
char *datetime_str = &datetime_buf[0];


    datetime_t t = {
            .year  = 2020,
            .month = 06,
            .day   = 05,
            .dotw  = 5, // 0 is Sunday, so 5 is Friday
            .hour  = 15,
            .min   = 45,
            .sec   = 00
    };
    
void setup() {
    Serial.begin(115200);
    //Serial.println("Hello RTC");

    rtc_init();
    //delay(500);
    rtc_set_datetime(&t);
    delay(500);
    
}

// the loop function runs over and over again forever
void loop() {
        rtc_get_datetime(&t);
        datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
        Serial.println("Hello RTC3");
        Serial.println(datetime_str);
        delay(5000);            
}
