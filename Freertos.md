# Freertos

## 按键消抖

```
static uint64_t pre_time = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)

{ 

 uint64_t time;

 if(GPIO_Pin == Key_Shift_Pin)

 {

  time = HAL_GetTick();

  if(time - pre_time < 200)return;

  printf("pressed\r\n");

 }

 else if(GPIO_Pin == Key_Left_Pin)

 {

  if(time - pre_time < 200)return;

  printf("pressed");

 }

 else if(GPIO_Pin == Key_Right_Pin){

  if(time - pre_time < 200)return;

  printf("pressed");

 }

 pre_time = time;

}
```

获取hal时基，对比两次时刻，重复触发在200ms内认为是电平波动