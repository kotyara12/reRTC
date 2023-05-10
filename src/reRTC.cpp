#include "reRTC.h"
#include "reEvents.h"
#include <stdint.h>
#include <esp_err.h>
#include <sys/time.h> 
#include "project_config.h"

#if CONFIG_RTC_INSTALLED

#if CONFIG_RTC_TYPE == DS1307
  #include "reDS1307.h"
  reDS1307 rtc(CONFIG_RTC_I2C_BUS, DS1307_ADDR);
#elif CONFIG_RTC_TYPE == DS3231
  #include "reDS3231.h"
  reDS3231 rtc(CONFIG_RTC_I2C_BUS, DS3231_ADDR);
#endif // CONFIG_RTC_TYPE

static void rtcEventHandlerTime(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
  if (event_id == RE_TIME_SNTP_SYNC_OK) {
    struct tm ti;
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    localtime_r(&tv.tv_sec, &ti);
    if (!rtc.is_running()) rtc.start();
    rtc.set_time(&ti);
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
  // Start RTC
  if (rtc.start() && rtc.get_time(&ti)) {
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

#endif // CONFIG_RTC_INSTALLED