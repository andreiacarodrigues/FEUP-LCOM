#include "RTC.h"

static inline int bcd_decimal(uint8_t hex)
{
    assert(((hex & 0xF0) >> 4) < 10);  // More significant nybble is valid
    assert((hex & 0x0F) < 10);         // Less significant nybble is valid
    int dec = ((hex & 0xF0) >> 4) * 10 + (hex & 0x0F);
    return dec;
}

RTC* newRTC()
{
	RTC* rtc = (RTC*) malloc(sizeof(RTC));
	getDate(rtc);
	return rtc;
}

void getDate(RTC* rtc)
{
	wait_valid_rtc(rtc);

	unsigned long data_reg[3];
	unsigned int i;

	for(i = 7; i < 10; i++)
	{
		sys_outb(RTC_ADDR_REG, i);
		sys_inb(RTC_DATA_REG, &data_reg[i-7]);
	}

	rtc->day = bcd_decimal(data_reg[0]);
	rtc->month = bcd_decimal(data_reg[1]);
	rtc->year = bcd_decimal(data_reg[2]);
}

void wait_valid_rtc(RTC* rtc)
{
	do
	{
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &rtc->registerA);
	}
	while (rtc->registerA & RTC_UIP);
}
