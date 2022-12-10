Projeto desenvolvido no ambiente IAR Embeded Workbench IDE; ver.: 5.60.2 e implementado em um microcontrolador MSP430F1611

O projeto utiliza do capture and compare do microcontrolador MSP430F1611 para criar um dimmer digital.

O circuito é implementado juntamente a um teclado matricial 3x4, um display LCD 16x2 e uma lâmpada para tornar mais intuitiva a implementação do dimmer. 

Funcionamento do software deverá obedecer ao pressionar das seguintes teclas:

o "2" para aumentar o ângulo de disparo em incrementos unitários;

o "5" para diminuir o ângulo de disparo em decrementos unitários;

o "3" aumentar o ângulo de disparo em incrementos de 10 graus;

o "6" para diminuir o ângulo de disparo em decrementos de 10 graus;

o "Entra" para ligar o Dimmer, ligando a lâmpada;

o "Anula" para desligar o Dimmer, desligando a lâmpada;

o Qualquer outra tecla: nada deve acontecer.

o Ao pressionar as teclas “Anula” ou “Entra” não se deve alterar o valor do ângulo de disparo.

Ainda, o valor atual do ângulo de disparo e o estado da lâmpada (ligada ou desligada) deve ser exibida no display.
