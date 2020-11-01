#include <stdio.h>
#include <string.h>

#include "cmsis_os.h"
#include "rl_usb.h"
#include "GPIO_STM32F10x.h"
#include "usb_hid_keys.h"

/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HID.c
 * Purpose: USB Device Human Interface Device example program
 *----------------------------------------------------------------------------*/
void usb_handle (void)
{
    uint8_t keybrd[] = {KEY_MOD_LMETA, 0, 0, 0 ,0, 0, 0, KEY_L};
    uint8_t  mouse[] = {0, 0, 0, 0, 2};
    
    // Emulate USB disconnect (USB_DP) ->
    GPIO_PortClock(GPIOA, true);
    GPIO_PinConfigure(GPIOA, 12, GPIO_OUT_OPENDRAIN, GPIO_MODE_OUT2MHZ);
    GPIO_PinWrite(GPIOA, 12, 0);
    
    osDelay(20);
    // Emulate USB disconnect (USB_DP) <-
    
    USBD_Initialize    (0);               /* USB Device 0 Initialization        */
    USBD_Connect       (0);               /* USB Device 0 Connect               */
    
    while(!USBD_Configured(0)){};
    osDelay(2000);
    
    USBD_HID_GetReportTrigger(0, 2, mouse, sizeof(mouse));
    osDelay(100);
    memset(mouse, 0, sizeof(mouse));
    USBD_HID_GetReportTrigger(0, 2, mouse, sizeof(mouse));
    osDelay(2000);                    /* 100 ms delay for sampling buttons  */
    
    
//    USBD_HID_GetReportTrigger(0, 1, keybrd, sizeof(keybrd));
//    osDelay(100);
//    memset(keybrd, 0, sizeof(keybrd));
//    USBD_HID_GetReportTrigger(0, 1, keybrd, sizeof(keybrd));
//    osDelay(2000);                    /* 100 ms delay for sampling buttons  */
    
    while(1){};
}

int main(void)
{
    printf("main runing..\r\n");
    
    usb_handle();
    
    return 0;
}
