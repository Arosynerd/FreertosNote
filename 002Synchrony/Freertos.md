# 任务间同步

任务2负责切菜，任务1炒菜

任务1先获取信号量，cnt++

任务2通过按键告知切好菜，随后释放信号量

任务再次获取信号量 cnt++，cnt == 2，开始炒菜，最后cnt = 0；

## 创建队列句柄

```
QueueHandle_t semphore_handle;
```



## 创建信号量

```
vSemaphoreCreateBinary(semphore_handle);
```

创建完后会释放信号量

## 任务1获取信号量，随后阻塞

```
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
    if(nowtime - pretime < 100)return;
    printf("pressed\r\n");
    
    //给task1发送信号量
    err = xSemaphoreGiveFromISR(semphore_handle, NULL);
    if(err == pdTRUE){
      printf("释放信号量成功\r\n");
      printf("菜切好了\r\n");
    }
    else{
      printf("释放信号量失败\r\n");
    }
  }
  pretime = nowtime;
}
/* USER CODE END Application */
```

菜切好时按下按键，释放信号量





