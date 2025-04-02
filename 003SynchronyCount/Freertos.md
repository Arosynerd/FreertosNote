# 计数信号量

任务1每隔1s获取一次信号量，获取不到时阻塞

外部中断按下会释放一次信号量

## 创建队列句柄

```
QueueHandle_t semphore_count_handle;
```



## 创建计数信号量

```
semphore_count_handle = xSemaphoreCreateCounting(100,0);
```

参数解释：

计数器长度

计数默认值

## 获取当前计数值

```
UBaseType_t count = 0;
count = uxSemaphoreGetCount(sem_count_handle);
```



## 任务1获取信号量，随后阻塞

```
void task1(void *pvParameters)
{
    BaseType_t err;
    UBaseType_t count; 
    while (1)
    {
        err = xSemaphoreTake(semphore_count_handle, portMAX_DELAY);
        if(err == pdTRUE){
            printf("task1获得信号量\r\n");
            count = uxSemphoreGetCount(sem_count_handle);
            printf("count = %d",count);
        }
        else{
            printf("task1获取信号量超时\r\n");
        }

    }
}
```

cnt = 2时炒菜



## 外部中断（按键）

```
uint64_t pretime;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  uint64_t nowtime;
  BaseType_t err;
  if(GPIO_Pin == GPIO_PIN_0) {
    //中断处理
    nowtime = HAL_GetTick();
    if(nowtime - pretime < 50)return;
    printf("pressed\r\n");
    
    //给task1发送信号量
    err = xSemaphoreGiveFromISR(semphore_handle, NULL);
    if(err == pdTRUE){
      printf("释放信号量成功\r\n");
    }
    else{
      printf("释放信号量失败\r\n");
    }
  }
  pretime = nowtime;
}
/* USER CODE END Application */
```

在中断中没有获取计数值的保护函数





