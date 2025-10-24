/* CIRCUITOS MICROPROCESSADOS - 2025/2
PROFESSOR: JEAN SCHMITH
ALUNOS: JULIA LENZ GOETZ E LUCAS SCHERER
PROJETO GRAU B - CARRINHO*/

//------------------------------ Estados e Direções -----------------------------------------------
enum ESTADO   : uint8_t { SEGUIR_LINHA=0, CURVA=1, PARADA=2 };
enum DIRECAO  : uint8_t { NENHUMA=0, ESQUERDA=1, DIREITA=2 };

//------------------------------ Definição da Velocidade (0-255) ---------------------------------
#define VEL_ESQUERDA_BASE 200    
#define VEL_DIREITA_BASE 180
#define VEL_CURVA_ESQ 100
#define VEL_CURVA_DIR 100
#define CURVA_MS 400 //duração da curva de 90° em ms

//------------------------------ Definição de Pinos ----------------------------------------------
#define LED_DEBUG 13
#define IR_CONTADOR_LINHA 3   //interrupção
#define IR_OBSTACULO 2        //interrupção
#define IR_SEGUIDOR_DIREITO 11   //digital  
#define IR_SEGUIDOR_ESQUERDO 12  //digital

#define PWM_MOTOR_DIREITO 5     //PWM
#define PWM_MOTOR_ESQUERDO 6    //PWM
#define MOTOR_DIREITO_IN_0 8
#define MOTOR_DIREITO_IN_1 7
#define MOTOR_ESQUERDO_IN_0 10
#define MOTOR_ESQUERDO_IN_1 9

//------------------------------ Variaveis Globais ----------------------------------------------
ESTADO estado = SEGUIR_LINHA;
DIRECAO direcao = NENHUMA;
int iCount = 0;
int isObstaculo = 0; // Logo, 1 = obstáculo presente.
int SENSOR_ESQUERDO = 0;
int SENSOR_DIREITO = 0;

const uint8_t COR_BRANCA = 0;
const uint8_t COR_PRETA  = 1;


//----------------------------------- SETUP -----------------------------------------------------
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

//----------------------------- MÁQUINA DE ESTADOS ------------------------------------------------
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
        curvar_direita();
      }
      break;

    case PARADA:
      parar();
      break;
  }
}

// ---------------------------- AÇÕES DE MOVIMENTO --------------------------------------------------
void andar_frente() 
{
  // Configura ambos os motores no mesmo sentido → carrinho anda reto para frente

  // Esquerdo
  digitalWrite(MOTOR_ESQUERDO_IN_0, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_ESQUERDO, VEL_ESQUERDA_BASE);
  // Direito
  digitalWrite(MOTOR_DIREITO_IN_0, LOW);
  digitalWrite(MOTOR_DIREITO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_DIREITO, VEL_DIREITA_BASE);
}

void andar_tras() 
{
  // Configura ambos os motores no mesmo sentido → carrinho anda reto para trás

  // Esquerdo
  digitalWrite(MOTOR_ESQUERDO_IN_0, HIGH);
  digitalWrite(MOTOR_ESQUERDO_IN_1, LOW);
  analogWrite(PWM_MOTOR_ESQUERDO, VEL_ESQUERDA_BASE);
  // Direito
  digitalWrite(MOTOR_DIREITO_IN_0, HIGH);
  digitalWrite(MOTOR_DIREITO_IN_1, LOW);
  analogWrite(PWM_MOTOR_DIREITO, VEL_DIREITA_BASE);
}

void curvar_esquerda() {
  // curva fechada: gira no eixo (E trás, D frente)

  // Esquerdo
  digitalWrite(MOTOR_ESQUERDO_IN_0, HIGH);
  digitalWrite(MOTOR_ESQUERDO_IN_1, LOW);
  analogWrite(PWM_MOTOR_ESQUERDO, VEL_ESQUERDA_BASE);
  // Direito
  digitalWrite(MOTOR_DIREITO_IN_0, LOW);
  digitalWrite(MOTOR_DIREITO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_DIREITO, VEL_DIREITA_BASE);
}

void curvar_direita() {
  // curva fechada: gira no eixo (E frente, D trás)

  // Esquerdo
  digitalWrite(MOTOR_ESQUERDO_IN_0, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_1, HIGH);
  analogWrite(PWM_MOTOR_ESQUERDO, VEL_ESQUERDA_BASE);
  // Direito
  digitalWrite(MOTOR_DIREITO_IN_0, HIGH);
  digitalWrite(MOTOR_DIREITO_IN_1, LOW);
  analogWrite(PWM_MOTOR_DIREITO, VEL_DIREITA_BASE);
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

//---------------------------- LEITURA DOS SENSORES DE LINHA ---------------------------------------

// Neste processo, armazenamos o valor lido pelo sensor na variável SENSOR.
void lerSensoresLinha(void) {
  SENSOR_ESQUERDO = digitalRead(IR_SEGUIDOR_ESQUERDO);
  SENSOR_DIREITO = digitalRead(IR_SEGUIDOR_DIREITO);
}

// ----------------------------- SEGUIDOR DE LINHA --------------------------------------------------
void seguidor_de_linha()
{
  lerSensoresLinha();
  if (SENSOR_ESQUERDO == COR_PRETA && SENSOR_DIREITO == COR_BRANCA) {
    // linha à esquerda → autocorreção à esquerda (curva suave)
    estado = CURVA;
    direcao = ESQUERDA;
  }
  
  else if (SENSOR_ESQUERDO == COR_BRANCA && SENSOR_DIREITO == COR_PRETA) {
    // linha à direita → autocorreção à direita (curva suave)
    estado = CURVA;
    direcao = DIREITA;
  } 
  
  else if (SENSOR_ESQUERDO == COR_PRETA && SENSOR_DIREITO == COR_PRETA) {
    // cruzamento / faixa cheia → avança simétrico
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

  } else {
    // ambos em branco → segue reto
    andar_reto();
  }
  
}

void realizar_a_curva()
{
  //Fazer a curva
  estado = SEGUIR_LINHA;
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