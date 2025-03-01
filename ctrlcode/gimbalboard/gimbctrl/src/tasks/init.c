#include "main.h"
void chasmotinit()
{
    init_shootmot_para(fric, &trig);
    init_shootmot_pid(fricspid, &trigapid, &trigspid);
}

void devinit()
{
    NUC_INIT();
    HAL_Delay(50);
    can_filter_init();
    HAL_Delay(50);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
    HAL_Delay(50);
    HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
    HAL_Delay(50);
}

void userinit()
{
    chasmotinit();
    HAL_Delay(50);
    devinit();
    HAL_Delay(50);
}
