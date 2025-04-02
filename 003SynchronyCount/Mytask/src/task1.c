#include "task1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
extern QueueHandle_t semphore_count_handle;
void task1(void *pvParameters)
{
    BaseType_t err;
    UBaseType_t count;
    while (1)
    {
        err = xSemaphoreTake(semphore_count_handle, portMAX_DELAY);
        if(err == pdTRUE){
            printf("task1获得信号量\r\n");
            count = uxSemaphoreGetCount(semphore_count_handle);
            printf("任务1,当前信号量计数值为：%d\r\n", count);
        }
        else{
            printf("task1获取信号量超时\r\n");
        }
        vTaskDelay(1000);
    }
}
