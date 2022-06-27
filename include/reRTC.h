/* 
   RU: Служба часов реального времени
   EN: Real time clock service
   --------------------------------------------------------------------------------
   (с) 2022 Разживин Александр | Razzhivin Alexander
   kotyara12@yandex.ru | https://kotyara12.ru | tg: @kotyara1971
*/

#ifndef __RE_RTC_H__
#define __RE_RTC_H__

#include "project_config.h"

#if CONFIG_RTC_DS1307_INSTALLED

#ifdef __cplusplus
extern "C" {
#endif

bool rtcStart();

#ifdef __cplusplus
}
#endif

#endif // CONFIG_RTC_DS1307_INSTALLED

#endif // __RE_RTC_H__