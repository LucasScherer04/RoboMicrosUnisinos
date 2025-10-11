/* CIRCUITOS MICROPROCESSADOS - 2025/2
PROFESSOR: JEAN SCHMITH
ALUNOS: JULIA LENZ GOETZ E LUCAS SCHERER
PROJETO GRAU B - CARRINHO*/

//Estados da Maquina de Estados
#define SEGUIR_LINHA 0
#define CURVA 1
#define PARADA 2

//Definição da Velocidade
#define VELOCIDADE_DIREITA 100
#define VELOCIDADE_ESQUERDA 50
#define VELOCIDADE_CURVA 50

//Pinos
#define LED_DEBUG 13
#define IR_CONTADOR_LINHA 3
#define IR_OBSTACULO 2
#define IR_SEGUIDOR_DIREITO 11
#define IR_SEGUIDOR_ESQUERDO 12
#define PWM_MOTOR_DIREITO 5
#define PWM_MOTOR_ESQUERDO 6
#define MOTOR_DIREITO_IN_0 8
#define MOTOR_DIREITO_IN_1 7
#define MOTOR_ESQUERDO_IN_0 10
#define MOTOR_ESQUERDO_IN_1 9

//Variaveis globais
int estado = SEGUIR_LINHA;
int iCount = 0;
int isObstaculo = 0;

void setup()
{
  //Configuração dos Pinos
  pinMode(LED_DEBUG, OUTPUT);
  pinMode(IR_CONTADOR_LINHA, INPUT);
  pinMode(IR_OBSTACULO, INPUT);
  pinMode(IR_SEGUIDOR_DIREITO, INPUT);
  pinMode(IR_SEGUIDOR_ESQUERDO, INPUT);
  pinMode(PWM_MOTOR_DIREITO, OUTPUT);
  pinMode(PWM_MOTOR_ESQUERDO, OUTPUT);
  pinMode(MOTOR_DIREITO_IN_0, OUTPUT);
  pinMode(MOTOR_DIREITO_IN_1, OUTPUT);
  pinMode(MOTOR_ESQUERDO_IN_0, OUTPUT);
  pinMode(MOTOR_ESQUERDO_IN_1, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(IR_OBSTACULO), obstaculo, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IR_CONTADOR_LINHA), contador_linha, RISING);
}

void loop()
{
  switch(estado)
  {
    case SEGUIR_LINHA:
      seguidor_de_linha();
      break;

    case CURVA:
      if (direcao == ESQUERDA) {
        curva_esquerda();
      }
      else if (direcao == DIREITA) {
        curva_direita();
      }
      break;

    case PARADA:
      parar();
      break;
  }
}

void andar_para_frente() 
{
  // Configura ambos os motores no mesmo sentido → carrinho anda reto
  // Faixa de velocidade 0-255

  // Motor Direito
  digitalWrite(MOTOR_DIREITO_IN_0, HIGH);
  digitalWrite(MOTOR_DIREITO_IN_1, LOW);
  analogWrite(PWM_MOTOR_DIREITO, VELOCIDADE); 

  // Motor Esquerdo
  digitalWrite(MOTOR_ESQUERDO_IN_0, HIGH);
  digitalWrite(MOTOR_ESQUERDO_IN_1, LOW);
  analogWrite(PWM_MOTOR_ESQUERDO, VELOCIDADE); 
}

void seguidor_de_linha()
{
  // Neste processo, armazenamos o valor lido pelo sensor na variável SENSOR.
  int SENSOR_ESQUERDO = digitalRead(IR_SEGUIDOR_ESQUERDO);
  int SENSOR_DIREITO = digitalRead(IR_SEGUIDOR_DIREITO);

  //Para a cor branca atribuímos o valor 0 e, para a cor preta, o valor 1.
  if (SENSOR_ESQUERDO == 0 && SENSOR_DIREITO == 1) {
    //Autocorreção para direita
    estado == CURVA_DIREITA
  }
  else if (SENSOR_ESQUERDO == 1 && SENSOR_DIREITO == 0) {
  // Autocorreção para esquerda
    estado = CURVA_ESQUERDA; 
  }
  else {
  andar_para_frente()
  }
}

void esquerda() {
  // Motor Direito para frente
  digitalWrite(MOTOR_DIREITO_IN_0, HIGH);
  digitalWrite(MOTOR_DIREITO_IN_1, LOW);
  analogWrite(PWM_MOTOR_DIREITO, VELOCIDADE); 
  // Motor Esquerdo para trás
  digitalWrite(MOTOR_ESQUERDO_IN_0, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_ESQUERDO, VELOCIDADE); 
}

void direita() {
  // Motor Direito para trás
  digitalWrite(MOTOR_DIREITO_IN_0, LOW);
  digitalWrite(MOTOR_DIREITO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_DIREITO, VELOCIDADE_CURVA); 
  // Motor Esquerdo para frente
  digitalWrite(MOTOR_ESQUERDO_IN_0, HIGH);
  digitalWrite(MOTOR_ESQUERDO_IN_1, LOW);
  analogWrite(PWM_MOTOR_ESQUERDO, VELOCIDADE_CURVA); 
}

void realizar_a_curva()
{
  //Fazer a curva
  estado = SEGUIR_LINHA;
}

void parar()
{
  analogWrite(PWM_MOTOR_DIREITO, 0);  //velocidade zerada
  analogWrite(PWM_MOTOR_ESQUERDO, 0); //velocidade zerada
  
  digitalWrite(MOTOR_DIREITO_IN_0, LOW);
  digitalWrite(MOTOR_DIREITO_IN_1, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_0, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_1, LOW);
}

void contador_linha()
{
  iCount++;
  
  if(iCount == 3){
    estado = CURVA;
    iCount = 0; //zera contagem deste trecho 
  }
}

void obstaculo()
{
  isObstaculo =! isObstaculo;
  
  if(isObstaculo)
    estado = PARADA;
  else
    estado = SEGUIR_LINHA;
}