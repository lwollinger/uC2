# Exercicio 1

Piscar Led de 3 diferentes maneiras:

    * 1° Sequencial;
    * 2° Vai e volta;
    * 3° Efeito Espelhado;
---
```ruby
    * GPIO_TypeDef *LED_GPIO_PORTS[] = {LED0_GPIO_Port,LED1_GPIO_Port, LED2_GPIO_Port, LED3_GPIO_Port, LED4_GPIO_Port,LED5_GPIO_Port, LED6_GPIO_Port, LED7_GPIO_Port};

    * uint32_t LED_PINS[] = {LED0_Pin, LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin,LED5_Pin, LED6_Pin, LED7_Pin};


1° Animação

	  for (int i = 0; i < 8; i++) {
		  HAL_Delay(500); // Atraso de 500ms
		  HAL_GPIO_WritePin(LED_GPIO_PORTS[i], LED_PINS[i], GPIO_PIN_RESET); // Leds em PullDown

		  if(i>0){
			  HAL_GPIO_WritePin(LED_GPIO_PORTS[i-1], LED_PINS[i-1], GPIO_PIN_SET);
		  }
		  if(i==0){
			  HAL_GPIO_WritePin(LED_GPIO_PORTS[7], LED_PINS[7], GPIO_PIN_SET);
		  }
	  }
	  

2° Animação

	  for (int i = 0; i <= 7; i++) {
		  HAL_Delay(200); // Atraso de 200ms
		  HAL_GPIO_WritePin(LED_GPIO_PORTS[i], LED_PINS[i], GPIO_PIN_RESET); // Leds em PullDown

		  if(i>0){
			  HAL_GPIO_WritePin(LED_GPIO_PORTS[i-1], LED_PINS[i-1], GPIO_PIN_SET);
		  }
	  }
	  for (int i = 7; i != 0; i--) {
		  HAL_Delay(200); // Atraso de 200ms
		  HAL_GPIO_WritePin(LED_GPIO_PORTS[i], LED_PINS[i], GPIO_PIN_RESET); // Leds em PullDown

		  if(i>=0){
			  HAL_GPIO_WritePin(LED_GPIO_PORTS[i+1], LED_PINS[i+1], GPIO_PIN_SET);
		  }
	  }
	  

3° Animação

	  for (int i = 0; i <= 7; i++) {
		  HAL_Delay(200); // Atraso de 200ms
		  HAL_GPIO_WritePin(LED_GPIO_PORTS[i], LED_PINS[i], GPIO_PIN_RESET); // Leds em PullDown
		  HAL_GPIO_WritePin(LED_GPIO_PORTS[7-i], LED_PINS[7-i], GPIO_PIN_RESET);

		  if(i>0){
			  HAL_GPIO_WritePin(LED_GPIO_PORTS[i-1], LED_PINS[i-1], GPIO_PIN_SET);
			  HAL_GPIO_WritePin(LED_GPIO_PORTS[8 - i], LED_PINS[8 - i], GPIO_PIN_SET);
		  }
	  }