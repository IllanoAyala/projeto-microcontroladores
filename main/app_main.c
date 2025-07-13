#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ds18b20_config.h"
#include "lcd_config.h"
#include "peltier_config.h"
#include "config.h"
#include "convert_temperature.h"
#include "send_data.h"

// typedef struct {
//     float Kp;
//     float Ki;
//     float Kd;
//     float prev_error;
//     float integral;
// } PIDController;

// float pid_compute(PIDController *pid, float setpoint, float current, float dt) {
//     float error = setpoint - current;
//     pid->integral += error * dt;
//     float derivative = (error - pid->prev_error) / dt;
//     pid->prev_error = error;

//     float output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;
//     return output;
// }

volatile bool button_pressed = false;

static void IRAM_ATTR button_isr_handler(void* arg) {
    button_pressed = true;
}

void button_init() {
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << BUTTON_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&io_conf);

    gpio_install_isr_service(0); // 0 = padrão, sem flags
    gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);
}

volatile bool peltier1_flag = false;
volatile bool peltier2_flag = false;

float temperature = 0.0f;
float set_point = 40.0f; 

void task_read_temp(void *pvParameters) {
    while (1) {
        temperature = ds18b20_read();
        lcd_set_cursor(0, 0);
        lcd_write_string_and_temp("Temp: ", temperature, current_unit);
        lcd_set_cursor(0, 1);
        lcd_write_string_and_temp("Set P: ", set_point, current_unit);

        if (temperature < set_point - 3.0f && peltier_state == PELTIER_OFF) {
            peltier_state = PELTIER_1_ON;
        } else if (temperature > set_point + 3.0f && peltier_state == PELTIER_OFF) {
            peltier_state = PELTIER_2_ON;
        }

        send_data_sdout(temperature, set_point);

        vTaskDelay(pdMS_TO_TICKS(SAMPLING_RATE)); 
    }
}

void task_control_peltier1(void *pvParameters) {
    gpio_config_t conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PELTIER_1),
    };
    gpio_config(&conf);

    while (1) {
        if (peltier_state == PELTIER_1_ON) {
            printf(">> Aquecendo: Peltier 1 ligado\n");
            gpio_set_level(PELTIER_1, 1);
            vTaskDelay(pdMS_TO_TICKS(PELTIER_ON_TIME));
            gpio_set_level(PELTIER_1, 0);
            peltier_state = PELTIER_OFF;
            printf(">> Aquecimento concluído: Peltier 1 desligado\n");
        }
        vTaskDelay(pdMS_TO_TICKS(50)); // Espera 50 ms antes de verificar novamente
    }
}

void task_control_peltier2(void *pvParameters) {
    gpio_config_t conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PELTIER_2),
    };
    gpio_config(&conf);

    while (1) {
        if (peltier_state == PELTIER_2_ON) {
            printf(">> Resfriando: Peltier 2 ligado\n");
            gpio_set_level(PELTIER_2, 1);
            vTaskDelay(pdMS_TO_TICKS(PELTIER_ON_TIME));
            gpio_set_level(PELTIER_2, 0);
            peltier_state = PELTIER_OFF;
            printf(">> Resfriamento concluído: Peltier 2 desligado\n");
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// void task_send_data(void *pvParameters) {
//     while (1) {
//         send_data_sdout(temperature, set_point); 
//         vTaskDelay(pdMS_TO_TICKS(SAMPLING_RATE));
//     }
// }

void setup(void)
{
    ds18b20_sensor_init();  
    lcd_init();
    button_init();             
}

void app_main(void)
{
    setup();

    // PIDController pid = {
    //     .Kp = 2.0f,  // Ganho proporcional
    //     .Ki = 0.5f,  // Ganho integral
    //     .Kd = 1.0f,  // Ganho derivativo
    //     .prev_error = 0.0f,
    //     .integral = 0.0f
    // };

    xTaskCreate(task_read_temp, "task_read_temp", 2048, NULL, 5, NULL); 
    // xTaskCreate(task_send_data, "task_send_data", 2048, NULL, 5, NULL);

    xTaskCreate(task_control_peltier1, "task_control_peltier1", 2048, NULL, 5, NULL);
    xTaskCreate(task_control_peltier2, "task_control_peltier2", 2048, NULL, 5, NULL);

    // char str[20];
    // snprintf(str, sizeof(str), "Set Point: %.2f", set_point);
    // lcd_scroll_text(str, 1, 1000); 
    

    while (1)
    {
        if (button_pressed) {
            button_pressed = false; 
            current_unit = (current_unit + 1) % TEMPERATURE_UNIT_COUNT;
            lcd_clear(); // Alterna entre as unidades
        }

        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    
    

    // float set_point;
    // float dt = 1.0f; // Intervalo de tempo em segundos
    
    // printf("Digite a temperatura desejada: ");
    // scanf("%f", &set_point);

    // if (set_point < 0 || set_point > 100) {
    //     printf("Temperatura fora do intervalo permitido (0-100 graus).\n");
    //     return;
    // }   
    
    // while (1) {
    //     float temperature = ds18b20_read();  
    //     lcd_set_cursor(0, 0);                            
    //     lcd_write_string_and_temp("Temp: ", temperature);

    //     peltier1_on(5000);  // Desliga Peltier 1
    //     peltier2_on(5000);  // Desliga Peltier 2
        
    //     // if (temperature < set_point + 2.0f && temperature > set_point - 2.0f) {
    //     //     lcd_set_cursor(0, 1);
    //     //     lcd_write_string_and_temp("Aquecendo...", temperature);
    //     //     peltier1_on(1000); // Liga o Peltier 1 por 1 segundo
    //     // } else if (temperature > set_point + 2.0f || temperature < set_point - 2.0f) {
    //     //     lcd_set_cursor(0, 1);
    //     //     lcd_write_string_and_temp("Resfriando...", temperature);
    //     //     peltier2_on(1000); // Liga o Peltier 2 por 1 segundo
    //     // }

    //     vTaskDelay(pdMS_TO_TICKS(SAMPLING_RATE));  // Espera pela taxa de amostragem definida
    // }

    // Controle PID (comentado para evitar conflito com o loop acima)

    // while (1) {
    //     float temperature = ds18b20_read();
    //     lcd_set_cursor(0, 0);
    //     lcd_write_string_and_temp("Temp: ", temperature);

    //     float control_output = pid_compute(&pid, set_point, temperature, dt);

    //     // Limita a saída entre -1000 a +1000 milissegundos
    //     if (control_output > 1000.0f) control_output = 1000.0f;
    //     if (control_output < -1000.0f) control_output = -1000.0f;

    //     lcd_set_cursor(0, 1);
    //     if (control_output > 0) {
    //         lcd_write_string("Aquecendo...");
    //         peltier1_on((int)control_output);  // Ativa aquecimento
    //     } else {
    //         lcd_write_string("Resfriando...");
    //         peltier2_on((int)(-control_output));  // Ativa resfriamento
    //     }

    //     vTaskDelay(pdMS_TO_TICKS(SAMPLING_RATE));
    // }
}


