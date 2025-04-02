# 互斥信号量

使用互斥信号量，解决优先级反转问题

任务一：获取信号量，执行逻辑，释放信号量。优先级1

任务二：执行逻辑。												优先级2

任务三：获取信号量，执行逻辑，释放信号量。优先级3

解决任务2抢占任务一，任务三较晚执行的优先级反转问题

## 创建队列句柄

```
QueueHandle_t semphore_mutex_handle;
```



## 创建计数信号量

```
semphore_mutex_handle = xSemaphoreCreateMutex();
```



## 任务1

```
void task1(void *pvParameters)
{
    BaseType_t err;
    while (1)
    {
        err = xSemaphoreTake(semphore_mutex_handle, portMAX_DELAY);
        if(err == pdTRUE){
            printf("task1获得信号量\r\n");
        }
        else{
            printf("task1获取信号量超时\r\n");
        }
        printf("task1运行....\r\n");
        HAL_Delay(2000);
        err = xSemaphoreGive(semphore_mutex_handle);
        if(err != pdTRUE){
            printf("task1释放信号量失败\r\n");
        }
        vTaskDelay(1000);
        
    }
}

```

## 任务二

```
void task2(void *pvParameters) {
  for (;;) {
    printf("task2运行....\r\n");
    HAL_Delay(2000);
    vTaskDelay(1000);
  }
}

```

## 任务三

```
void task3(void *pvParameters)
{
  BaseType_t err;
  while (1)
  {
      err = xSemaphoreTake(semphore_mutex_handle, portMAX_DELAY);
      if(err == pdTRUE){
          printf("task3获得信号量\r\n");
      }
      else{
          printf("task3获取信号量超时\r\n");
      }
      printf("task3运行....\r\n");
      HAL_Delay(1000);
      err = xSemaphoreGive(semphore_mutex_handle);
      if(err != pdTRUE){
          printf("task3释放信号量失败\r\n"); 
      }
      vTaskDelay(1000);
      
  }
}
```





