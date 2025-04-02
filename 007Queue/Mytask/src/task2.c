#include "task2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"

//mydefine
#include "queue.h"

extern QueueHandle_t myBigQueueHandle;
void task2(void *pvParameters)
{
    BaseType_t err; // 用于接收队列发送状态的变量
    char *buff;
    while (1)
    {
        err = xQueueReceive(myBigQueueHandle, &buff, portMAX_DELAY); // 从队列中接收数据
        if (err != pdPASS)
        {
            printf("receive error\r\n"); // 接收失败，打印错误信息
        }
        else
        {
            printf("receive ok, count = %s\r\n", buff); // 接收成功，打印接收到的数据
        }
    }
}
