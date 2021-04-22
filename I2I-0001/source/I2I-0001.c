/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "fsl_debug_console.h"
#include "fsl_adc16.h"
#include "littlevgl_support.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "lvgl.h"
#include "lv_examples/src/lv_demo_widgets/lv_demo_widgets.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
static volatile bool s_lvgl_initialized = false;
#define ADC16_BASE0          ADC0  	//defines base 0 to chip ADC 0
#define ADC16_BASE1          ADC1  	//defines base 1 to chip ADC 1
#define ADC16_CHANNEL_GROUP0 0U		//defines group 0 which signifies ADC user group A
#define ADC16_CHANNEL_GROUP1 1U		//defines group 1 which signifies ADC user group B
#define ADC16_USER_CHANNEL0 8U      //define channel 0 to ADC_SE8 for battery voltage reading

//below ADCs are for Spencer's stuff
#define ADC16_USER_CHANNEL0 8U      //define channel 0 to ADC_SE8
#define ADC16_USER_CHANNEL0 8U      //define channel 0 to ADC_SE8
#define ADC16_USER_CHANNEL0 8U      //define channel 0 to ADC_SE8
#define ADC16_USER_CHANNEL0 8U      //define channel 0 to ADC_SE8
#define ADC16_USER_CHANNEL0 8U      //define channel 0 to ADC_SE8
#define ADC16_USER_CHANNEL0 8U      //define channel 0 to ADC_SE8
#define ADC16_USER_CHANNEL0 8U      //define channel 0 to ADC_SE8
#define ADC16_USER_CHANNEL0 8U      //define channel 0 to ADC_SE8

/*******************************************************************************
 * Variables
 ******************************************************************************/

const uint32_t g_Adc16_12bitFullRange = 4096U;
int led1active = 0;
int led2active = 0;
int led3active = 0;
int batterycharge = 0;
int priorityindex = 0;

int adcbattery = 0;
int adcin1v = 0;
int adcin1a = 0;
int adcin2v = 0;
int adcin2a = 0;
int adcin3v = 0;
int adcin3a = 0;
int adcout1v = 0;
int adcout1a = 0;
int adcout2v = 0;
int adcout2a = 0;
int adcout3v = 0;
int adcout3a = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void AppTask(void *param)
{
    lv_port_pre_init();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    s_lvgl_initialized = true;

    lv_demo_widgets();

    for (;;)
    {
        lv_task_handler();
        vTaskDelay(5);
    }
}

int ConvertADCVBatteryValue(void)
{
	if (adcbattery <= 1822) batterycharge = 0;
	else if (adcbattery == 1823) batterycharge = 1;
	else if (adcbattery == 1824) batterycharge = 2;
	else if (adcbattery == 1825) batterycharge = 3;
	else if (adcbattery == 1826) batterycharge = 4;
	else if (adcbattery == 1827) batterycharge = 5;
	else if (adcbattery == 1828) batterycharge = 6;
	else if (adcbattery == 1829) batterycharge = 7;
	else if (adcbattery == 1830) batterycharge = 8;
	else if (adcbattery == 1831) batterycharge = 9;
	else if (adcbattery == 1832) batterycharge = 10;
	else if (adcbattery == 1833) batterycharge = 10;
	else if (adcbattery == 1834) batterycharge = 11;
	else if (adcbattery == 1835) batterycharge = 11;
	else if (adcbattery == 1836) batterycharge = 12;
	else if (adcbattery == 1837) batterycharge = 12;
	else if (adcbattery == 1838) batterycharge = 12;
	else if (adcbattery == 1839) batterycharge = 13;
	else if (adcbattery == 1840) batterycharge = 13;
	else if (adcbattery == 1841) batterycharge = 14;
	else if (adcbattery == 1842) batterycharge = 14;
	else if (adcbattery == 1843) batterycharge = 14;
	else if (adcbattery == 1844) batterycharge = 15;
	else if (adcbattery == 1845) batterycharge = 15;
	else if (adcbattery == 1846) batterycharge = 16;
	else if (adcbattery == 1847) batterycharge = 16;
	else if (adcbattery == 1848) batterycharge = 17;
	else if (adcbattery == 1849) batterycharge = 17;
	else if (adcbattery == 1850) batterycharge = 17;
	else if (adcbattery == 1851) batterycharge = 18;
	else if (adcbattery == 1852) batterycharge = 18;
	else if (adcbattery == 1853) batterycharge = 19;
	else if (adcbattery == 1854) batterycharge = 19;
	else if (adcbattery == 1855) batterycharge = 20;
	else if (adcbattery == 1856) batterycharge = 20;
	else if (adcbattery == 1857) batterycharge = 20;
	else if (adcbattery == 1858) batterycharge = 21;
	else if (adcbattery == 1859) batterycharge = 21;
	else if (adcbattery == 1860) batterycharge = 22;
	else if (adcbattery == 1861) batterycharge = 22;
	else if (adcbattery == 1862) batterycharge = 22;
	else if (adcbattery == 1863) batterycharge = 23;
	else if (adcbattery == 1864) batterycharge = 23;
	else if (adcbattery == 1865) batterycharge = 24;
	else if (adcbattery == 1866) batterycharge = 24;
	else if (adcbattery == 1867) batterycharge = 25;
	else if (adcbattery == 1868) batterycharge = 25;
	else if (adcbattery == 1869) batterycharge = 25;
	else if (adcbattery == 1870) batterycharge = 26;
	else if (adcbattery == 1871) batterycharge = 26;
	else if (adcbattery == 1872) batterycharge = 27;
	else if (adcbattery == 1873) batterycharge = 27;
	else if (adcbattery == 1874) batterycharge = 27;
	else if (adcbattery == 1875) batterycharge = 28;
	else if (adcbattery == 1876) batterycharge = 28;
	else if (adcbattery == 1877) batterycharge = 29;
	else if (adcbattery == 1878) batterycharge = 29;
	else if (adcbattery == 1879) batterycharge = 30;
	else if (adcbattery == 1880) batterycharge = 30;
	else if (adcbattery == 1881) batterycharge = 30;
	else if (adcbattery == 1882) batterycharge = 31;
	else if (adcbattery == 1883) batterycharge = 31;
	else if (adcbattery == 1884) batterycharge = 32;
	else if (adcbattery == 1885) batterycharge = 32;
	else if (adcbattery == 1886) batterycharge = 33;
	else if (adcbattery == 1887) batterycharge = 33;
	else if (adcbattery == 1888) batterycharge = 33;
	else if (adcbattery == 1889) batterycharge = 34;
	else if (adcbattery == 1890) batterycharge = 34;
	else if (adcbattery == 1891) batterycharge = 35;
	else if (adcbattery == 1892) batterycharge = 35;
	else if (adcbattery == 1893) batterycharge = 35;
	else if (adcbattery == 1894) batterycharge = 36;
	else if (adcbattery == 1895) batterycharge = 36;
	else if (adcbattery == 1896) batterycharge = 37;
	else if (adcbattery == 1897) batterycharge = 37;
	else if (adcbattery == 1898) batterycharge = 38;
	else if (adcbattery == 1899) batterycharge = 38;
	else if (adcbattery == 1900) batterycharge = 38;
	else if (adcbattery == 1901) batterycharge = 39;
	else if (adcbattery == 1902) batterycharge = 39;
	else if (adcbattery == 1903) batterycharge = 40;
	else if (adcbattery == 1904) batterycharge = 40;
	else if (adcbattery == 1905) batterycharge = 41;
	else if (adcbattery == 1906) batterycharge = 41;
	else if (adcbattery == 1907) batterycharge = 41;
	else if (adcbattery == 1908) batterycharge = 42;
	else if (adcbattery == 1909) batterycharge = 42;
	else if (adcbattery == 1910) batterycharge = 43;
	else if (adcbattery == 1911) batterycharge = 43;
	else if (adcbattery == 1912) batterycharge = 44;
	else if (adcbattery == 1913) batterycharge = 44;
	else if (adcbattery == 1914) batterycharge = 45;
	else if (adcbattery == 1915) batterycharge = 45;
	else if (adcbattery == 1916) batterycharge = 45;
	else if (adcbattery == 1917) batterycharge = 46;
	else if (adcbattery == 1918) batterycharge = 46;
	else if (adcbattery == 1919) batterycharge = 47;
	else if (adcbattery == 1920) batterycharge = 47;
	else if (adcbattery == 1921) batterycharge = 48;
	else if (adcbattery == 1922) batterycharge = 48;
	else if (adcbattery == 1923) batterycharge = 49;
	else if (adcbattery == 1924) batterycharge = 49;
	else if (adcbattery == 1925) batterycharge = 50;
	else if (adcbattery == 1926) batterycharge = 50;
	else if (adcbattery == 1927) batterycharge = 50;
	else if (adcbattery == 1928) batterycharge = 51;
	else if (adcbattery == 1929) batterycharge = 51;
	else if (adcbattery == 1930) batterycharge = 52;
	else if (adcbattery == 1931) batterycharge = 52;
	else if (adcbattery == 1932) batterycharge = 53;
	else if (adcbattery == 1933) batterycharge = 53;
	else if (adcbattery == 1934) batterycharge = 54;
	else if (adcbattery == 1935) batterycharge = 54;
	else if (adcbattery == 1936) batterycharge = 54;
	else if (adcbattery == 1937) batterycharge = 55;
	else if (adcbattery == 1938) batterycharge = 55;
	else if (adcbattery == 1939) batterycharge = 56;
	else if (adcbattery == 1940) batterycharge = 56;
	else if (adcbattery == 1941) batterycharge = 57;
	else if (adcbattery == 1942) batterycharge = 57;
	else if (adcbattery == 1943) batterycharge = 58;
	else if (adcbattery == 1944) batterycharge = 58;
	else if (adcbattery == 1945) batterycharge = 58;
	else if (adcbattery == 1946) batterycharge = 59;
	else if (adcbattery == 1947) batterycharge = 59;
	else if (adcbattery == 1948) batterycharge = 60;
	else if (adcbattery == 1949) batterycharge = 60;
	else if (adcbattery == 1950) batterycharge = 61;
	else if (adcbattery == 1951) batterycharge = 61;
	else if (adcbattery == 1952) batterycharge = 62;
	else if (adcbattery == 1953) batterycharge = 62;
	else if (adcbattery == 1954) batterycharge = 63;
	else if (adcbattery == 1955) batterycharge = 63;
	else if (adcbattery == 1956) batterycharge = 64;
	else if (adcbattery == 1957) batterycharge = 64;
	else if (adcbattery == 1958) batterycharge = 65;
	else if (adcbattery == 1959) batterycharge = 65;
	else if (adcbattery == 1960) batterycharge = 66;
	else if (adcbattery == 1961) batterycharge = 66;
	else if (adcbattery == 1962) batterycharge = 67;
	else if (adcbattery == 1963) batterycharge = 67;
	else if (adcbattery == 1964) batterycharge = 68;
	else if (adcbattery == 1965) batterycharge = 68;
	else if (adcbattery == 1966) batterycharge = 69;
	else if (adcbattery == 1967) batterycharge = 69;
	else if (adcbattery == 1968) batterycharge = 70;
	else if (adcbattery == 1969) batterycharge = 70;
	else if (adcbattery == 1970) batterycharge = 70;
	else if (adcbattery == 1971) batterycharge = 71;
	else if (adcbattery == 1972) batterycharge = 71;
	else if (adcbattery == 1973) batterycharge = 72;
	else if (adcbattery == 1974) batterycharge = 72;
	else if (adcbattery == 1975) batterycharge = 73;
	else if (adcbattery == 1976) batterycharge = 73;
	else if (adcbattery == 1977) batterycharge = 74;
	else if (adcbattery == 1978) batterycharge = 74;
	else if (adcbattery == 1979) batterycharge = 75;
	else if (adcbattery == 1980) batterycharge = 75;
	else if (adcbattery == 1981) batterycharge = 76;
	else if (adcbattery == 1982) batterycharge = 76;
	else if (adcbattery == 1983) batterycharge = 77;
	else if (adcbattery == 1984) batterycharge = 77;
	else if (adcbattery == 1985) batterycharge = 78;
	else if (adcbattery == 1986) batterycharge = 78;
	else if (adcbattery == 1987) batterycharge = 79;
	else if (adcbattery == 1988) batterycharge = 79;
	else if (adcbattery == 1989) batterycharge = 80;
	else if (adcbattery == 1990) batterycharge = 80;
	else if (adcbattery == 1991) batterycharge = 81;
	else if (adcbattery == 1992) batterycharge = 81;
	else if (adcbattery == 1993) batterycharge = 82;
	else if (adcbattery == 1994) batterycharge = 82;
	else if (adcbattery == 1995) batterycharge = 83;
	else if (adcbattery == 1996) batterycharge = 83;
	else if (adcbattery == 1997) batterycharge = 84;
	else if (adcbattery == 1998) batterycharge = 84;
	else if (adcbattery == 1999) batterycharge = 85;
	else if (adcbattery == 2000) batterycharge = 85;
	else if (adcbattery == 2001) batterycharge = 86;
	else if (adcbattery == 2002) batterycharge = 87;
	else if (adcbattery == 2003) batterycharge = 87;
	else if (adcbattery == 2004) batterycharge = 88;
	else if (adcbattery == 2005) batterycharge = 88;
	else if (adcbattery == 2006) batterycharge = 89;
	else if (adcbattery == 2007) batterycharge = 89;
	else if (adcbattery == 2008) batterycharge = 90;
	else if (adcbattery == 2009) batterycharge = 90;
	else if (adcbattery == 2010) batterycharge = 91;
	else if (adcbattery == 2011) batterycharge = 91;
	else if (adcbattery == 2012) batterycharge = 92;
	else if (adcbattery == 2013) batterycharge = 92;
	else if (adcbattery == 2014) batterycharge = 93;
	else if (adcbattery == 2015) batterycharge = 94;
	else if (adcbattery == 2016) batterycharge = 94;
	else if (adcbattery == 2017) batterycharge = 95;
	else if (adcbattery == 2018) batterycharge = 95;
	else if (adcbattery == 2019) batterycharge = 96;
	else if (adcbattery == 2020) batterycharge = 97;
	else if (adcbattery == 2021) batterycharge = 97;
	else if (adcbattery == 2022) batterycharge = 98;
	else if (adcbattery == 2023) batterycharge = 98;
	else if (adcbattery == 2024) batterycharge = 99;
	else if (adcbattery == 2025) batterycharge = 99;
	else if (adcbattery >= 2026) batterycharge = 100;
}

static void MeasurementTask(void *param)
  {
	adc16_config_t adc16ConfigStruct;
	adc16_channel_config_t adc16ChannelConfigStruct;

    ADC16_GetDefaultConfig(&adc16ConfigStruct);
	#ifdef BOARD_ADC_USE_ALT_VREF
    adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
	#endif
    ADC16_Init(ADC16_BASE0, &adc16ConfigStruct);
    ADC16_Init(ADC16_BASE1, &adc16ConfigStruct);

    ADC16_EnableHardwareTrigger(ADC16_BASE0, false); /* Make sure the software trigger is used. */
    ADC16_EnableHardwareTrigger(ADC16_BASE1, false); /* Make sure the software trigger is used. */

	#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    if (kStatus_Success == ADC16_DoAutoCalibration(ADC0))
    {
        PRINTF("ADC16_DoAutoCalibration() on ACO0 Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() on ACO0 Failed.\r\n");
    }
    if (kStatus_Success == ADC16_DoAutoCalibration(ADC1))
    {
        PRINTF("ADC16_DoAutoCalibration() on ACO1 Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() on ACO1 Failed.\r\n");
    }
	#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
    PRINTF("ADC Full Range: %d\r\n", g_Adc16_12bitFullRange);
    PRINTF("Press any key to get user channel's ADC value ...\r\n");

    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */


	for( ;; )
	{
		adc16ChannelConfigStruct.channelNumber = ADC16_USER_CHANNEL0;
		ADC16_SetChannelConfig(ADC16_BASE0, ADC16_CHANNEL_GROUP0, &adc16ChannelConfigStruct);
        while (0U == (kADC16_ChannelConversionDoneFlag &
                      ADC16_GetChannelStatusFlags(ADC16_BASE0, ADC16_CHANNEL_GROUP0)))
        {
        }

        adcbattery = ADC16_GetChannelConversionValue(ADC16_BASE0, ADC16_CHANNEL_GROUP0);

        ConvertADCVBatteryValue();


		int j;
        int k = 0;
        for(j=0;j<262144;j++);
        k++;
	}

	/* Tasks must not attempt to return from their implementing
	function or otherwise exit.  In newer FreeRTOS port
	attempting to do so will result in an configASSERT() being
	called if it is defined.  If it is necessary for a task to
	exit then have the task call vTaskDelete( NULL ) to ensure
	its exit is clean. */
	vTaskDelete( NULL );
  }

static void DecisionMaker(void *param)
  {

	for( ;; )
	{
		if (priorityindex == 0)
		{
			GPIO_PinWrite(GPIOA, 5, 1);
			GPIO_PinWrite(GPIOA, 13, 1);
			GPIO_PinWrite(GPIOA, 12, 1);
		}
		else if (priorityindex == 1)
		{
			GPIO_PinWrite(GPIOA, 5, 0);
			GPIO_PinWrite(GPIOA, 13, 0);
			GPIO_PinWrite(GPIOA, 12, 0);
		}
		else if (priorityindex == 2)
		{
			GPIO_PinWrite(GPIOA, 5, 0);
			GPIO_PinWrite(GPIOA, 13, 0);
			GPIO_PinWrite(GPIOA, 12, 1);
		}
		else if (priorityindex == 3)
		{
			GPIO_PinWrite(GPIOA, 5, 0);
			GPIO_PinWrite(GPIOA, 13, 1);
			GPIO_PinWrite(GPIOA, 12, 0);
		}
		else if (priorityindex == 4)
		{
			GPIO_PinWrite(GPIOA, 5, 1);
			GPIO_PinWrite(GPIOA, 13, 0);
			GPIO_PinWrite(GPIOA, 12, 0);
		}
		else if (priorityindex == 5)
		{
			GPIO_PinWrite(GPIOA, 5, 1);
			GPIO_PinWrite(GPIOA, 13, 0);
			GPIO_PinWrite(GPIOA, 12, 1);
		}
		else if (priorityindex == 6)
		{
			GPIO_PinWrite(GPIOA, 5, 1);
			GPIO_PinWrite(GPIOA, 13, 1);
			GPIO_PinWrite(GPIOA, 12, 0);
		}
	}

	/* Tasks must not attempt to return from their implementing
	function or otherwise exit.  In newer FreeRTOS port
	attempting to do so will result in an configASSERT() being
	called if it is defined.  If it is necessary for a task to
	exit then have the task call vTaskDelete( NULL ) to ensure
	its exit is clean. */
	vTaskDelete( NULL );
  }

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t DSPI1_GetFreq(void)
{
    return CLOCK_GetBusClkFreq();
}

uint32_t I2C1_GetFreq(void)
{
    return CLOCK_GetFreq(I2C1_CLK_SRC);
}
/*!
 * @brief Main function
 */
int main(void)
{
    BaseType_t stat;
    BaseType_t stat2;
    BaseType_t stat3;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();



    stat = xTaskCreate(AppTask, "littlevgl", configMINIMAL_STACK_SIZE + 800, NULL, tskIDLE_PRIORITY + 2, NULL);

    if (pdPASS != stat)
    {
        PRINTF("Failed to create littlevgl task");
        while (1)
            ;
    }

    stat2 = xTaskCreate(MeasurementTask, "measure", configMINIMAL_STACK_SIZE + 10, NULL, tskIDLE_PRIORITY + 1, NULL);

    if (pdPASS != stat2)
    {
        PRINTF("Failed to create measurement task");
        while (1)
            ;
    }

    stat3 = xTaskCreate(DecisionMaker, "decisions", configMINIMAL_STACK_SIZE + 10, NULL, tskIDLE_PRIORITY + 1, NULL);

    if (pdPASS != stat3)
    {
        PRINTF("Failed to create measurement task");
        while (1)
            ;
    }

    vTaskStartScheduler();

    for (;;)
    {
    } /* should never get here */
}

/*!
 * @brief Malloc failed hook.
 */
void vApplicationMallocFailedHook(void)
{
    for (;;)
        ;
}

/*!
 * @brief FreeRTOS tick hook.
 */
void vApplicationTickHook(void)
{
    if (s_lvgl_initialized)
    {
        lv_tick_inc(1);
    }
}

/*!
 * @brief Stack overflow hook.
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    (void)pcTaskName;
    (void)xTask;

    for (;;)
        ;
}
