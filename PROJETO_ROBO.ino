/* CIRCUITOS MICROPROCESSADOS - 2025/2
PROFESSOR: JEAN SCHMITH
ALUNOS: JULIA LENZ GOETZ E LUCAS SCHERER
PROJETO GRAU B - CARRINHO*/

enum ESTADO: uint8_t { SEGUIR_LINHA=0, CURVA=1, PARADA=2 };
enum DIRECAO: uint8_t { NENHUMA=0, ESQUERDA=1, DIREITA=2 };

#define VEL_ESQUERDA_BASE 200
#define VEL_DIREITA_BASE 180
#define VEL_CORRECAO 20

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

ESTADO estado = SEGUIR_LINHA;
DIRECAO direcao = NENHUMA;
int iCount = 0;
int isObstaculo = 0; 
int SENSOR_ESQUERDO = 0;
int SENSOR_DIREITO = 0;

const uint8_t COR_BRANCA = 0;
const uint8_t COR_PRETA = 1;

unsigned long tInicioCurva = 0;

void setup()
{
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

  // attachInterrupt(digitalPinToInterrupt(IR_CONTADOR_LINHA), contador_linha, RISING);
}

void andar_frente() 
{
  digitalWrite(MOTOR_ESQUERDO_IN_0, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_ESQUERDO, VEL_ESQUERDA_BASE);

  digitalWrite(MOTOR_DIREITO_IN_0, LOW);
  digitalWrite(MOTOR_DIREITO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_DIREITO, VEL_DIREITA_BASE);
}

void andar_tras() 
{
  digitalWrite(MOTOR_ESQUERDO_IN_0, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_ESQUERDO, VEL_ESQUERDA_BASE);

  digitalWrite(MOTOR_DIREITO_IN_0, HIGH);
  digitalWrite(MOTOR_DIREITO_IN_1, LOW);
  analogWrite(PWM_MOTOR_DIREITO, VEL_DIREITA_BASE);
}

void curvar_esquerda_suave() {
 
  digitalWrite(MOTOR_ESQUERDO_IN_0, HIGH);
  digitalWrite(MOTOR_ESQUERDO_IN_1, LOW);
  analogWrite(PWM_MOTOR_ESQUERDO, (VEL_ESQUERDA_BASE - VEL_CORRECAO));

  digitalWrite(MOTOR_DIREITO_IN_0, LOW);
  digitalWrite(MOTOR_DIREITO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_DIREITO, VEL_DIREITA_BASE);
}

void curvar_direita_suave() {
 
  digitalWrite(MOTOR_ESQUERDO_IN_0, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_ESQUERDO, VEL_ESQUERDA_BASE);

  digitalWrite(MOTOR_DIREITO_IN_0, HIGH);
  digitalWrite(MOTOR_DIREITO_IN_1, LOW);
  analogWrite(PWM_MOTOR_DIREITO, (VEL_DIREITA_BASE - VEL_CORRECAO));
}

void parar()
{
  analogWrite(PWM_MOTOR_DIREITO, 0);
  analogWrite(PWM_MOTOR_ESQUERDO, 0); 

  digitalWrite(MOTOR_DIREITO_IN_0, LOW);
  digitalWrite(MOTOR_DIREITO_IN_1, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_0, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_1, LOW);
}


void lerSensoresLinha(void) {
  SENSOR_ESQUERDO = digitalRead(IR_SEGUIDOR_ESQUERDO);
  SENSOR_DIREITO = digitalRead(IR_SEGUIDOR_DIREITO);
}

void seguidor_de_linha()
{
  lerSensoresLinha();
  if (SENSOR_ESQUERDO == COR_PRETA && SENSOR_DIREITO == COR_BRANCA) {
    curvar_esquerda_suave(); 
  }
  else if (SENSOR_ESQUERDO == COR_BRANCA && SENSOR_DIREITO == COR_PRETA) {
    curvar_direita_suave(); 
  } 
  else {
    andar_frente();
  }
}

void curvar_esquerda()
{
  digitalWrite(MOTOR_ESQUERDO_IN_0, HIGH);
  digitalWrite(MOTOR_ESQUERDO_IN_1, LOW);
  analogWrite(PWM_MOTOR_ESQUERDO, VEL_ESQUERDA_BASE); 

  digitalWrite(MOTOR_DIREITO_IN_0, LOW);
  digitalWrite(MOTOR_DIREITO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_DIREITO, VEL_DIREITA_BASE); 

  lerSensoresLinha(); 

  while(SENSOR_ESQUERDO == COR_BRANCA && SENSOR_DIREITO == COR_BRANCA) {
    lerSensoresLinha(); 
  }
 
  parar();
  estado = SEGUIR_LINHA;
  direcao = NENHUMA;
}

void contador_linha()
{
  iCount++;

  if(iCount == 3){
    estado = PARADA;
    iCount = 0; 
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

void loop()
{
  switch(estado)
  {
    case SEGUIR_LINHA:
      seguidor_de_linha();
      break;

    case CURVA:
      if (direcao == ESQUERDA) {
        curvar_esquerda();
      }
      else if (direcao == DIREITA) {
        curvar_esquerda();
      }
      break;

    case PARADA:
      parar();
      break;
  }
}