<samp>
<h1 align="center"> Projeto Final :desktop_computer: </h1>
<p align="center"> Simulação de Sensoriamento para Prevenção de Deslizamentos de Terra
  
<h2 align="center"> Sobre :pencil: </h2>

<p align="justify"> &emsp;&emsp;&emsp;&emsp; Repositório com o objetivo de apresentar o projeto final para encerramento da 1ª fase da EmbarcaTech, residência de Sistemas Embarcados. Essa tarefa foi desenvolvida pela residente Isabela Madureira Argolo.

<h3> Descrição da tarefa </h3>

<p align="justify"> &emsp;&emsp;&emsp;&emsp; O projeto utiliza os seguintes componentes:

  1. LED RGB, com os pinos conectados às GPIOs 11, 12 e 13;
  2. Buzzer A conectado ao GPIO 21;
  3. Microfone conectado ao GPIO 28;
  4. Joystick conectado aos GPIOs 26 e 27;
  5. Botão A conectado ao GPIO 5;
  6. Display SSD1306 conectado via I2C (GPIO 14 e GPIO 15).

 &emsp;&emsp;&emsp;&emsp; O projeto possui as seguintes funcionalidades:

<p align="justify">  1. Medição dos dados obtidos pelo joystick para incrementar ou decrementar os valores do sensor de umidade do solo e do pluviômetro; </p> 
<p align="justify">  2. Medição dos dados obtidos pelo microfone para simular um acelerômetro; </p>
<p align="justify">  3. Tratamento da informação para conversão para unidade do sensor; </p>
<p align="justify">  4. Análise com condicionais dos cenários que apresentam cada um dos 4 tipos de risco: fraco, moderado, alto e crítico; </p>
<p align="justify">  5. Emissão de alerta visual do risco por meio do LED RGB usando PWM: verde, amarelo, laranja e vermelho; </p>
<p align="justify">  6. Emissão de alerta sonoro do risco crítico por meio do buzzer usando PWM; </p>
<p align="justify">  7. Exibição dos dados obtidos para cada sensor e do tipo de risco no display OLED SSD1306. </p>

<h3> Estruturação dos Arquivos </h3>

<p align="justify"> &emsp;&emsp;&emsp;&emsp; O projeto é composto pelos arquivos comuns a um projeto Pico W, além de uma pasta chamada 'include', que possui 7 arquivos na extensão .c, que são: animation.c (que organiza a matriz de bitmap para mensagem de boas-vindas), button.c (que organiza as funções relacionadas ao botão), buzzer.c (que organiza as funções relacionadas ao buzzer), joystick.c (que organiza as funções relacionadas ao joystick), led-rgb.c (que organiza as funções relacionadas ao LED RGB), microfone.c (que organiza as funções relacionadas ao microfone) e sdd1306.c (que organiza as funções relacionadas ao display), 8 arquivos na extensão .h, que são: animation.h, button.h, buzzer.h, font.h, joystick.h, led-rgb.h, microfone.h e ssd1306.h (em que ambos possuem as importações e o cabeçalho das funções), uma pasta chamada 'wokwi', com os arquivos diagram.json e wokwi.toml (para a simulação no Wokwi) e os arquivos projeto_final_embarcatech.c (que gerencia todo o projeto) e READ.ME (descrição do projeto).

<h2></h2>
<h2 align="center"> Como utilizar :computer_mouse: </h2>

<p align="justify"> &emsp;&emsp;&emsp;&emsp; Para utilizar o programa, o usuário deve:

- Clonar o repositório em uma pasta separada para recebê-lo;
- Abrir o projeto no VS Code, importando-o pela extensão Raspberry Pi Pico;
- Conectar a placa BitDogLab em modo BOOTSEL;
- Compilar e executar o projeto;
- Testar as funcionalidades descritas acima;
- Se quiser testar no simulador, basta abrir o arquivo diagram.json e rodar.

</samp>
