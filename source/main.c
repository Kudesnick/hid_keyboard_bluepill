#include <stdio.h>
#include <string.h>

#include "cmsis_os.h"
#include "rl_usb.h"
#include "GPIO_STM32F10x.h"
#include "usb_hid_keys.h"
#include "ctl.h"

void ctl_init(void)
{
    GPIO_PortClock(CTL_PORT, true);
    // Btns config
    GPIO_PinConfigure(CTL_PORT, CTL_BTN1_PIN, GPIO_IN_PULL_DOWN, GPIO_MODE_INPUT);
    GPIO_PinConfigure(CTL_PORT, CTL_BTN2_PIN, GPIO_IN_PULL_DOWN, GPIO_MODE_INPUT);
    // Leds config
    GPIO_PinConfigure(CTL_PORT, CTL_LED1_PIN, GPIO_OUT_OPENDRAIN, GPIO_MODE_OUT2MHZ);
    GPIO_PinWrite(CTL_PORT, CTL_LED1_PIN, CTL_LED_ON);
    GPIO_PinConfigure(CTL_PORT, CTL_LED2_PIN, GPIO_OUT_OPENDRAIN, GPIO_MODE_OUT2MHZ);
    GPIO_PinWrite(CTL_PORT, CTL_LED2_PIN, CTL_LED_ON);
    GPIO_PinConfigure(CTL_PORT, CTL_LED3_PIN, GPIO_OUT_OPENDRAIN, GPIO_MODE_OUT2MHZ);
    GPIO_PinWrite(CTL_PORT, CTL_LED3_PIN, CTL_LED_ON);
}

struct btn_s
{
    uint8_t pin;
    uint8_t state;
    uint8_t next_state;
} btn[2] = {{CTL_BTN1_PIN, CTL_BTN_OFF, CTL_BTN_OFF}, {CTL_BTN2_PIN, CTL_BTN_OFF, CTL_BTN_OFF}};

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
    
    osDelay(10);
    // Emulate USB disconnect (USB_DP) <-
    
    USBD_Initialize    (0);               /* USB Device 0 Initialization        */
    USBD_Connect       (0);               /* USB Device 0 Connect               */
    
    while(!USBD_Configured(0)){};

//    USBD_HID_GetReportTrigger(0, 2, mouse, sizeof(mouse));
//    osDelay(100);
//    memset(mouse, 0, sizeof(mouse));
//    USBD_HID_GetReportTrigger(0, 2, mouse, sizeof(mouse));
//    osDelay(2000);                    /* 100 ms delay for sampling buttons  */

//    USBD_HID_GetReportTrigger(0, 1, keybrd, sizeof(keybrd));
//    osDelay(100);
//    memset(keybrd, 0, sizeof(keybrd));
//    USBD_HID_GetReportTrigger(0, 1, keybrd, sizeof(keybrd));
//    osDelay(2000);                    /* 100 ms delay for sampling buttons  */
    
    while(osDelay(10))
    {
        for (uint8_t i = 0; i < 2; i++)
        {
            uint8_t btn_tmp = GPIO_PinRead(CTL_PORT, btn[i].pin);
            
            if (btn[i].next_state != btn_tmp)
            {
                btn[i].next_state = btn_tmp;
            }
            else if (btn[i].state != btn[i].next_state)
            {
                btn[i].state = btn[i].next_state;
                
                switch (i)
                {
                    // Btn "logout"
                    case 0:
                        switch (btn[i].state)
                        {
                            case CTL_BTN_ON:
                                {
                                    uint8_t keybrd[] = {KEY_MOD_LMETA, 0, 0, 0 ,0, 0, 0, KEY_L};
                                    USBD_HID_GetReportTrigger(0, 1, keybrd, sizeof(keybrd));
                                }
                                break;
                            case CTL_BTN_OFF:
                                {
                                    uint8_t keybrd[] = {0, 0, 0, 0 ,0, 0, 0, 0};
                                    USBD_HID_GetReportTrigger(0, 1, keybrd, sizeof(keybrd));
                                }
                                break;
                        }
                        break;
                    // Btn "volume up"
                    case 1:
                        switch (btn[i].state)
                        {
                            case CTL_BTN_ON:
                                {
                                    uint8_t media[] = {1 << 6};
                                    USBD_HID_GetReportTrigger(0, 3, media, sizeof(media));
                                }
                                break;
                            case CTL_BTN_OFF:
                                {
                                    uint8_t media[] = {0};
                                    USBD_HID_GetReportTrigger(0, 3, media, sizeof(media));
                                }
                                break;
                        }
                        break;
                }
            }
        }
    };
}

int main(void)
{
    printf("main runing..\r\n");
    
    ctl_init();
    
    usb_handle();
    
    return 0;
}
