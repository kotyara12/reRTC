#include "reRTC.h"
#include "reEvents.h"
#include <stdint.h>
#include <esp_err.h>
#include <sys/time.h> 
#include "reDS1307.h"

reDS1307 ds1307(CONFIG_RTC_DS1307_I2C_BUS, DS1307_ADDR);

static void rtcEventHandlerTime(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
  if (event_id == RE_TIME_SNTP_SYNC_OK) {
    struct tm ti;
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    localtime_r(&tv.tv_sec, &ti);
    if (!ds1307.is_running()) ds1307.start(true);
    ds1307.set_time(&ti);
  };
}

bool rtcStart()
{
  struct tm ti;
  struct timeval tv;
  // Set timezone
  #if defined(CONFIG_SNTP_TIMEZONE)
  setenv("TZ", CONFIG_SNTP_TIMEZONE, 1);
  tzset(); 
  #endif
  // Start DS1307
  if (ds1307.start(true) && ds1307.get_time(&ti)) {
    // Update system time
    gettimeofday(&tv, nullptr);
    tv.tv_sec = mktime(&ti);
    settimeofday(&tv, nullptr);
    // Register event handler
    eventHandlerRegister(RE_TIME_EVENTS, RE_TIME_SNTP_SYNC_OK, rtcEventHandlerTime, nullptr);
    // Post system event
    eventLoopPost(RE_TIME_EVENTS, RE_TIME_RTC_ENABLED, nullptr, 0, portMAX_DELAY);
    return true;  
  };
  return false;
}

