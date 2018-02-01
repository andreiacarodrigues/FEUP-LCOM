#pragma once
#include "i8042.h"
#include <assert.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
/** @defgroup Platform Platform
 * @{
 * Functions for getting the date from the RTC
 */
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 10
#define RTC_UIP BIT(7)

typedef struct {
	unsigned long registerA;
	int day;
	int month;
	int year;
} RTC;
/**
 * @brief Converts bcd number to decimal
 * @param hex number in bcd
 * @return decimal number
 */
static inline int bcd_decimal(uint8_t hex);
/**
 * @brief Initializes a new RTC
 * @return pointer to the new RTC created
 */
RTC* newRTC();
/**
 * @brief Gets the date from the RTC
 * @param rtc rtc created
 */
void getDate(RTC* rtc);
/**
 * @brief Waits for valid RTC output
 * @param rtc rtc created
 */
void wait_valid_rtc(RTC* rtc);
