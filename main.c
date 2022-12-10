
/* 
Prog 07 - Dimmer Digital:

funcionamento do software deverá obedecer ao pressionar das seguintes teclas:
o "2" para aumentar o ângulo de disparo em incrementos unitários;
o "5" para diminuir o ângulo de disparo em decrementos unitários;
o "3" aumentar o ângulo de disparo em incrementos de 10 graus;
o "6" para diminuir o ângulo de disparo em decrementos de 10 graus;
o "Entra" para ligar o Dimmer, ligando a lâmpada;
o "Anula" para desligar o Dimmer, desligando a lâmpada;
o Qualquer outra tecla: nada deve acontecer.
o Lembre-se de que ao pressionar as teclas “Anula” ou “Entra” não se deve alterar o valor do ângulo de disparo.
*/

// Possibilidades do estado lâmpada
#define Ligado 1            
#define Desligado 0

// Variáveis Globais
char Tecla = 0;        // Contem o código ASCII da varredura do teclado
int Angulo = 90;    // Determina a intensidade da luminosidade
int Estado = 0;    // Determina o estado da lâmpada

#include "io430.h"
#include "MLCD_lib.h"
#include "kb_lib.h"
#include "stdlib.h"

// Declaração de funções
void itoa();

// main
void main( void )
{ 
  //Define mensagens a ser enviadas 
   unsigned char MSG_Ligado[] = "Ligado   ";
   unsigned char MSG_Desligado[]  = "Desligado";
  
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
 
  // Configurações dos periféricos
  Init_Display();
  Config_Display();
  tec_config_rti();
  
  P1SEL |= (BIT1+BIT2);
  P1DIR &= ~BIT1;
  P1DIR |= BIT2;
  
  BCSCTL1 &= ~XT2OFF;                          //Ativa o cristal de 4MHz
  BCSCTL2 |= SELS;                             // Seleciona a fonte do SMCLK
  TACCTL0 = CM_1 + CCIS_0 + CAP + SCS + CCIE;  // Configuração do capture and compare
  // CM seleciona a captura para borda de subida
  // CCIS seleciona a entrada do CC
  // CAP seleciona modo de captura ou compare
  // SCS seleciona modo síncrono ou assíncrono
  // CCIE habilita a interrupção
  TACCTL1 = OUTMOD_3;                          // Seleciona modo de SET/RESET 
  
  // Exibe as mensagens iniciais: estado desligado e ângulo = 90°
  EnviaString("Angulo = 090\xDF"); 
  EnviaComando(0xC0);  // Muda para início da 2a linha
  EnviaString(MSG_Desligado); 
  
  
  __enable_interrupt();    // Habiliatação geral das INTs
    
    while (1)
  {
    if (Tecla)   // Verifica se uma tecla foi pressionada
    {
      switch(Tecla)
      {
        // Situação para tecla 5 pressionada
      case '5':                    // '5' equivale a 0x35
        if (Angulo > 5)
        {
          Angulo--;               // Executa a operação de decremento da variável ângulo
          EnviaComando(0x8D);     // Muda para início da 1a linha
          itoa();                 // Executa a função itoa
        }
        break;
        
        // Situação para tecla 6 pressionada
      case '6':
        if (Angulo >= 15)
        {
         Angulo -= 10;
         EnviaComando(0x8D);
         itoa(); 
        }
        break;
        
       // Situação para tecla 2 pressionada
      case '2':
        if (Angulo < 175)
        {
          Angulo++;
          EnviaComando(0x8D);
          itoa();
        }
        break;
        
       // Situação para tecla 3 pressionada
       case '3':
        if (Angulo <= 165)
        {
          Angulo += 10;
          EnviaComando(0x8D);
          itoa();
        }
        break;
        
        // Situação para tecla A pressionada
        case 'A':                    
        if (Estado != Desligado)
        {
          Estado = Desligado;
          EnviaComando(0xC0);  
          EnviaString(MSG_Desligado);  
          TACTL = TASSEL_2 + MC_0 + TACLR;
        }
        break;
        
        // Situação para tecla E pressionada
      case 'E':
        if (Estado != Ligado)
        {
          Estado = Ligado;                    // Define o estado para ligado
          EnviaComando(0xC0);                 // Muda para início da 1a linha
          EnviaString(MSG_Ligado);            // Envia estado ao display
          TACTL = TASSEL_2 + MC_2 + TACLR;    // Configura timer para seu funcionamento
          // TACTL define o SMCLK como fonte do clock
          // TASSEL define o oscilador utilizado como fonte
          // MC define o modo de contagem do timer
          // TACLR faz o clear do timer A, reseta TAR
        }
        break;
      }
      Tecla = 0;    // A tarefa de atendimento do pressionar da tecla já foi executada
    }
    
  } 
}

// Função que converte o valor int para ASCII
void itoa()
{
  // Define valores de centena, dezena e unidade
  int centena = Angulo/100;
  int dezena = (Angulo - centena*100)/10;
  int unidade = (Angulo - centena*100 - dezena*10);
  
   
  // Aloca cada grandeza em sua respectiva posição
  EnviaComando(0x89);
  EnviaDado(centena+0x30);
  EnviaComando(0x8A);
  EnviaDado(dezena+0x30);
  EnviaComando(0x8B);
  EnviaDado(unidade+0x30);
}

// Interrupção do capture and compare
#pragma vector=TIMERA0_VECTOR
__interrupt void captura()
{
  TACCR1 = TACCR0 + Angulo*182;  // Define o valor de referência para o ângulo de 90°
  TACCR0 = TACCR1 + 200;         // Define um pulso de 500ms para o gate
  TACCTL0 &= ~CCIFG;             // Habilita a interrupção
}

// Interrupção da varredura do teclado
#pragma vector=PORT2_VECTOR
 __interrupt void Teclado_rti()
 {
   // Desabilita as INTs de P2
   P2IE &= ~(BIT0+BIT1+BIT2);
   // Periféricos com múltiplas fontes ressetado ao fonal da RTI (em inglês ISR = rotina de tratamento de INT)
   P2IFG &= ~(BIT0+BIT1+BIT2);
   
   __enable_interrupt(); // Reabilitação geral das INTS
   
 // Aguarda a passagem do tempo correspondente à trepidação da tecla
 // Normalmente, um tempo de delay de 5ms é suficiente.
 __delay_cycles((long)(Fclk*5e-3));


 // Passada a trepidação, só faz a varredura se alguma coluna estiver ativa.
 // Isto quer dizer que a INT ocorreu no fechamento da tecla
 if (P2IN &(BIT0+BIT1+BIT2))
 {
 Tecla = tec_rd_rti();
 }

 // Periféricos com múltiplas fontes de INT devem ter o flag correspondente
 // Resetado ao final da RTI (em inglês ISR = rotina de tratamento de INT)
 P2IFG &= ~(BIT0+BIT1+BIT2);
 
 // Reabilita as INTS de P2
 P2IE |= (BIT0+BIT1+BIT2);
 }