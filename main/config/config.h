
// CONFIG DO SENSOR DS18B20
#define CONFIG_ONE_WIRE_GPIO 27 // pino do sensor DS18B20
#define DS18B20_RESOLUTION   DS18B20_RESOLUTION_12_BIT // resolução do sensor DS18B20

//CONFIG DO LCD
#define LCD_RS     GPIO_NUM_5
#define LCD_EN     GPIO_NUM_4
#define LCD_D4     GPIO_NUM_16
#define LCD_D5     GPIO_NUM_17
#define LCD_D6     GPIO_NUM_18
#define LCD_D7     GPIO_NUM_19

//CONFIG PELTIER
#define PELTIER_1   GPIO_NUM_32 //32
#define PELTIER_2   GPIO_NUM_33 //33
#define PELTIER_ON_TIME 15000 

//CONFIG BOTAO
#define BUTTON_GPIO GPIO_NUM_21 

//CONFID GLOBAL
#define SAMPLING_RATE 1000 // taxa de amostragem em milissegundos