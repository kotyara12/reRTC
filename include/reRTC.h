/* 
   RU: Служба часов реального времени
   EN: Real time clock service
   --------------------------------------------------------------------------------
   (с) 2022-2024 Разживин Александр | Razzhivin Alexander
   kotyara12@yandex.ru | https://kotyara12.ru | tg: @kotyara1971
*/

#ifndef __RE_RTC_H__
#define __RE_RTC_H__

#include "project_config.h"

#define DS1307 1
#define DS3231 2

#if defined(CONFIG_RTC_INSTALLED)

#if CONFIG_RTC_TYPE == DS1307
   #include "reDS1307.h"
#elif CONFIG_RTC_TYPE == DS3231
   #include "reDS3231.h"
#endif // CONFIG_RTC_TYPE

#ifdef __cplusplus
extern "C" {
#endif

bool rtcStart();

#ifdef __cplusplus
}
#endif

#endif // CONFIG_RTC_INSTALLED

#endif // __RE_RTC_H__