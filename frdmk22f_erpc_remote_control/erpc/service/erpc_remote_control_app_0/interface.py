# Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
# Copyright 2016 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

#
# Generated by erpcgen 1.8.0 on Thu Oct  8 16:38:35 2020.
#
# AUTOGENERATED - DO NOT EDIT
#

import erpc

#App 0: client part is on the PC, server on the board
class Iremote_control_app_0(object):
    SERVICE_ID = 1
    GET_BOARD_CONFIG_ID = 1
    GET_ADC_CONFIG_ID = 2
    CONVERT_DAC_ADC_ID = 3
    READ_MAG_ACCEL_ID = 4
    SET_LED_ID = 5

   #Get board configuration
    def get_board_config(self, config):
        raise NotImplementedError()

   #Get configuration for ADC
    def get_adc_config(self, config):
        raise NotImplementedError()

   #Convert number through DAC
    def convert_dac_adc(self, numberToConvert, result):
        raise NotImplementedError()

   #Read magnetometer and accelerometer data
    def read_mag_accel(self, results, status):
        raise NotImplementedError()

   #Turn on selected LED
    def set_led(self, whichLed):
        raise NotImplementedError()


