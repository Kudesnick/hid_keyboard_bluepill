#include <stdio.h>

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
void usb_handle (void) {
  uint8_t buf1[8] = {KEY_LEFTMETA, KEY_L, 0 ,0 ,0, 0, 0, 0};
  uint8_t buf2[8] = {0};

    // Emulate USB disconnect (USB_DP) ->
    GPIO_PortClock(GPIOA, true);
    GPIO_PinConfigure(GPIOA, 12, GPIO_OUT_OPENDRAIN, GPIO_MODE_OUT2MHZ);
    GPIO_PinWrite(GPIOA, 12, 0);
    
	osDelay(10);
    // Emulate USB disconnect (USB_DP) <-

  USBD_Initialize    (0);               /* USB Device 0 Initialization        */
  USBD_Connect       (0);               /* USB Device 0 Connect               */

  while (1) {                           /* Loop forever                       */
    osDelay(2000);
    if (USBD_Configured (0))
    {
      USBD_HID_GetReportTrigger(0, 0, buf1, sizeof(buf1));
      USBD_HID_GetReportTrigger(0, 0, buf2, sizeof(buf2));
      osDelay(2000);                       /* 100 ms delay for sampling buttons  */
    }
  }
}

int main(void)
{
    printf("main runing..\r\n");
    
    usb_handle();
    
    return 0;
}
