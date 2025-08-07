# _Controle de Temperatura por Efeito Termoelétrico com Módulos Peltier_

## Sobre o Projeto

Este projeto é o trabalho final da disciplina de Microcontroladores do curso de Engenharia Mecatrônica do CEFET-MG. O sistema desenvolvido propõe uma solução automatizada e de baixo custo para o controle de temperatura de um objeto sólido, utilizando módulos Peltier gerenciados por um microcontrolador ESP32. O controle térmico é feito por meio do efeito termoelétrico, permitindo tanto o aquecimento quanto o resfriamento.

## Objetivo

O principal objetivo do projeto é criar um sistema de controle de temperatura capaz de manter a temperatura de um objeto dentro de uma faixa predefinida pelo usuário. A solução foi projetada para ser eficiente, compacta e de fácil implementação.

## Requisitos Operacionais

Para garantir a operação correta do sistema, os seguintes requisitos foram definidos e cumpridos[cite: 17, 27]:

* **Definição da Faixa de Temperatura**: O usuário configura a temperatura desejada (Set Point) por meio de uma interface serial. O sistema define automaticamente os limites mínimo e máximo para ativação dos módulos Peltier.
* **Leitura de Temperatura (DS18B20)**: A medição da temperatura é realizada por um sensor digital DS18B20 com leituras a cada 1 segundo.
* **Controle Termoelétrico com Módulo Peltier**: Dois módulos Peltier são utilizados, um para resfriamento e outro para aquecimento, acionados por transistores MOSFET controlados pelo ESP32.
* **Interface Gráfica**: Uma interface front-end em Python captura os dados de temperatura via comunicação serial, exibindo um gráfico em tempo real para monitoramento contínuo.

## Tecnologias Utilizadas

O sistema integra conceitos de automação, eletrônica e controle térmico, utilizando as seguintes tecnologias:

* **Microcontrolador**: ESP32, escolhido por sua arquitetura dual-core, conectividade Wi-Fi/Bluetooth e excelente relação custo-benefício.
* **Sensor de Temperatura**: DS18B20, conhecido por sua confiabilidade e comunicação digital via protocolo 1-Wire.
* **Elementos de Atuação**: Módulos Peltier para controle térmico ativo (aquecimento e resfriamento).
* **Acionamento**: Transistores MOSFET, usados como elementos de chaveamento para os módulos Peltier.
* **Visualização**: Um display LCD para exibir a temperatura atual e o Set Point, e uma interface gráfica em Python para visualização em tempo real.

## Arquitetura do Sistema

O diagrama abaixo apresenta a arquitetura de alto nível do sistema, mostrando os principais blocos funcionais e suas interconexões.

O sistema é alimentado por uma fonte dedicada para as células Peltier e via USB para o ESP32 e demais componentes.

## Desenvolvimento

### Circuito Eletrônico e PCB

O circuito foi inicialmente montado e testado em uma protoboard. Posteriormente, o projeto da placa de circuito impresso (PCB) foi desenvolvido no software EasyEDA para a montagem final do sistema.
<img width="749" height="492" alt="Captura de tela 2025-08-07 142314" src="https://github.com/user-attachments/assets/68972d65-6b50-4536-9602-959418568c23" />


### Software Embarcado

A lógica de controle ON/OFF foi implementada no ESP32. Além disso, uma interface gráfica em Python foi desenvolvida para exibir a curva de temperatura em tempo real.
![Imagem do WhatsApp de 2025-07-13 à(s) 12 53 27_cdac8a8d](https://github.com/user-attachments/assets/7837cee3-ec21-41eb-9cfe-de9fab3f50ef)

## Resultados

O sistema demonstrou um desempenho satisfatório durante os testes de aquecimento, alcançando a temperatura máxima de 40°C de forma consistente. O tempo necessário para aquecer o objeto variou entre 3 e 5 minutos em condições desfavoráveis de isolamento térmico. No modo de resfriamento, a temperatura foi reduzida em apenas 4°C, o que foi considerado um resultado limitado, possivelmente devido à alta inércia térmica do objeto de aço.

A estratégia de controle ON/OFF com MOSFETs provou ser adequada para os requisitos do projeto, garantindo um funcionamento estável e simplificando a implementação.

## Conclusão

O projeto demonstrou ser uma solução viável, funcional e de baixo custo, atendendo aos objetivos iniciais de forma autônoma e eficiente. O uso de tecnologias acessíveis e replicáveis como o ESP32, DS18B20 e módulos Peltier permitiu uma aplicação prática e educativa no contexto de sistemas embarcados.

## Autores

* [cite_start]Caique Eduardo Fonseca Ferreira 
* [cite_start]Illano Ayala Alves Ferreira 
* [cite_start]João Eduardo Freitas de Albuquerque 
* [cite_start]Miguel Coelho Nogueira 

## Licença

Este projeto está licenciado sob os termos da licença MIT.
