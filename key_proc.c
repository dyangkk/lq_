uint8_t key_scan(void)
{
	uint8_t ret=0;
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == GPIO_PIN_RESET)
		ret=1;
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == GPIO_PIN_RESET)
		ret=2;
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == GPIO_PIN_RESET)
		ret=3;
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET)
		ret=4;
	return ret;
}


//key
uint8_t k_down,k_up,k_val,k_old,k_temp;
uint8_t k_down_num=0; //按键按下次数

uint32_t k_time_2; //双击--时间
uint32_t k_time_1; //长按--时间
uint32_t k_time;

void key_proc(void)
{
	k_val = key_scan();              //按键扫描
	k_up  = ~k_val & (k_old^k_val);  //上升沿有效 
	k_down = k_val & (k_old^k_val);  //下降沿有效  
	k_old = k_val;
	
	if(k_down)
	{
		k_time_1 = 0;		
	}
	if(k_up)
	{
		k_temp = k_up;
		if(k_down_num==0)
		{
			k_down_num=1;
			k_time_2 = k_time;
		}
		else
			k_down_num=0;
	}	
    
	if(k_down_num==1)  
	{
		if((k_time-k_time_2)<20) //双击
		{
			switch(k_down)
			{
				case 1:set_led(16);break;
				case 2:set_led(0);break;		
				case 3:break;
				case 4:break;
			}			
		}
		else //单击 
		{
			k_down_num=0;
			if(k_time_1<80)   
			{
				switch(k_temp)
				{
					case 1:set_led(1);break;
					case 2:break;		
					case 3:break;
					case 4:break;
				}						
			}
		}
	}
	if(k_time_1>80) //长按
	{
		switch(k_val)
		{
			case 1:set_led(128);break;
			case 2:break;		
			case 3:break;
			case 4:break;
		}		
	}
}

//10ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	UNUSED(htim);
	if(htim->Instance == TIM6)
	{
		k_time++;
		k_time_1++;
		HAL_TIM_Base_Start_IT(&htim6);
	}
}




