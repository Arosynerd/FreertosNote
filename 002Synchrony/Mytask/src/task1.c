#include "task1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
extern QueueHandle_t semphore_handle;
void task1(void *pvParameters)
{
    uint8_t cnt = 0;
    BaseType_t err;
    while (1)
    {
        err = xSemaphoreTake(semphore_handle, portMAX_DELAY);
        if(err == pdTRUE){
            printf("task1获得信号量\r\n");
            cnt++;
        }
        else{
            printf("task1获取信号量超时\r\n");
        }
        if(cnt == 2){
            printf("开始炒菜\r\n");
        }
    }
}
