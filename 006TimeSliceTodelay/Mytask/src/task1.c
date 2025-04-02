#include "task1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
void task1(void *pvParameters)
{
    uint16_t count = 0;
    uint32_t haltick = 0;
    while (1)
    {
        haltick = HAL_GetTick();
        printf("task1运行:%d次\r\n", count++);
        while(HAL_GetTick() - haltick < 10);
    }
}
