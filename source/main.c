#include <stdio.h>

#include "cmsis_os.h"
#include "rl_usb.h"
#include "GPIO_STM32F10x.h"

/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HID.c
 * Purpose: USB Device Human Interface Device example program
 *----------------------------------------------------------------------------*/
void usb_handle (void) {
  uint8_t buf[1];

    // Emulate USB disconnect (USB_DP) ->
    GPIO_PortClock(GPIOA, true);
    GPIO_PinConfigure(GPIOA, 12, GPIO_OUT_OPENDRAIN, GPIO_MODE_OUT2MHZ);
    GPIO_PinWrite(GPIOA, 12, 0);
    
	osDelay(10);
    // Emulate USB disconnect (USB_DP) <-

  USBD_Initialize    (0);               /* USB Device 0 Initialization        */
  USBD_Connect       (0);               /* USB Device 0 Connect               */

  while (1) {                           /* Loop forever                       */
    USBD_HID_GetReportTrigger(0, 0, &buf[0], 1);
    osDelay(100);                       /* 100 ms delay for sampling buttons  */
  }
}

int main(void)
{
    printf("main runing..\r\n");
    
    usb_handle();
    
    return 0;
}
