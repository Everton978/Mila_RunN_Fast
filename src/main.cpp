#include <Arduino.h>

#define IR 12
#define motorEsqPin  19 
#define motorDirPin  18 

//============================= Configurações de Hardware ================================================
const int pinosSensores[8] = {34, 35, 32, 13, 25, 26, 27, 14}; 
const int pesos[8] = {-100, -50, -35, -25, 25, 35, 50, 100}; 

const int LimiarSensorS = 3000;
const int LimiarSensorI = 2300;

const int VELOCIDADE_BASE = 25; 


uint8_t Leituras[8]; 
uint8_t SensorAtivoF[8];

int soma_peso = 0;
int soma_ativa = 0;


unsigned long tempo_anterior = 0;

//====================Tipos personalizados=================================

typedef struct {
  uint16_t Frequencia;
  uint16_t DutyCicle_0;
  uint16_t DutyCicle_1;
  uint8_t Resolution;
  uint8_t CanalEsq;
  uint8_t CanalDir;
  uint8_t Motor_0;
  uint8_t Motor_1;
  

} PwmConfig;

PwmConfig acionarPWM;

typedef struct {
    float kp;       
    float ki;       
    float kd;       
    float erro_anterior;
    float integral; 
    float saida_min; 
    float saida_max; 
} PIDController;

PIDController ControlPID;

//==========================||||||||||||||||||=================================

//============================= Protoripos de Funções ================================================

void configBoard();
void ConfigPWM(PwmConfig *pwm, uint16_t freq, uint16_t Dc0, uint16_t Dc1, uint8_t res, uint8_t chan0, uint8_t chan1,  uint8_t Motor_0, uint8_t Motor_1);
void pid_init(PIDController *pid, float kp, float ki, float kd, float min, float max);
float pid_calcula(PIDController *pid, float setpoint, float atual, float dt);
void LeiturasComFiltro();
float calcular_posicao();
void AcionarMotores(int correcao);


//==========================||||||||||||||||||=================================



void setup() {
  Serial.begin(115200);

  analogReadResolution(12);

  configBoard();
  ConfigPWM(&acionarPWM, 1000, 0, 0, 12, 0, 1, motorEsqPin, motorDirPin);
  
  pid_init(&ControlPID, 2.5f, 0.0f, 0.5f, -150.0f, 150.0f); 
}

void loop() {


  digitalWrite(IR, HIGH); 
  
  LeiturasComFiltro(); // Faz a leitura física, joga na matriz e calcula a média móvel
  
  unsigned long tempo_atual = millis();
  float dt = (tempo_atual - tempo_anterior) / 1000.0f;
  if (dt <= 0.0f) dt = 0.001f; 
  tempo_anterior = tempo_atual;

  float posicao_linha = calcular_posicao();
  float setpoint = 0.0f; 
  
  float correcao = pid_calcula(&ControlPID, setpoint, posicao_linha, dt);
  
  AcionarMotores((int)correcao);

  delay(2); // Pequeno delay apenas para estabilidade do ADC (300Hz a 500Hz de taxa de amostragem)
}

//==========================Funcoes Personalizadas==============================================

void configBoard() {
  pinMode(IR, OUTPUT);

  pinMode(motorEsqPin, OUTPUT);

  pinMode(motorDirPin, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(pinosSensores[i], INPUT);
  }


}


void ConfigPWM(PwmConfig *pwm, uint16_t freq, uint16_t Dc0, uint16_t Dc1, uint8_t res, uint8_t chan0, uint8_t chan1,  uint8_t motor_0, uint8_t motor_1){

  pwm -> Frequencia = freq;
  pwm -> DutyCicle_0 = Dc0;
  pwm -> DutyCicle_1 = Dc1;
  pwm -> Resolution = res;
  pwm -> CanalEsq = chan0;
  pwm -> CanalDir = chan1;
  pwm -> Motor_0 = motor_0;
  pwm -> Motor_1 = motor_1;

  
  ledcSetup(pwm ->CanalEsq, pwm->Frequencia,pwm->Resolution); //configurção virtual do pwm
  ledcSetup(pwm ->CanalDir, pwm->Frequencia,pwm->Resolution); //configurção virtual do pwm

  ledcAttachPin(pwm -> Motor_0, pwm ->CanalEsq);
  ledcAttachPin(pwm -> Motor_1, pwm ->CanalDir);


}

// Nova função de leitura estruturada em Matriz
void LeiturasComFiltro() {
  // 1. Lê os valores físicos e armazena na coluna atual da matriz de histórico
  for(uint8_t i = 0; i < 8; i++) {
    Leituras[i] = analogRead(pinosSensores[i]);
    SensorAtivoF[i] = (Leituras[i] > LimiarSensorI  && Leituras[i] < LimiarSensorS)? 1 : 0;
    soma_peso += SensorAtivoF[i] * pesos[i];
    soma_ativa += SensorAtivoF[i];

  }


  for (int i = 0; i < 8; i++) {
    Serial.print("Posicao [");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(Leituras[i]);
    delay(600);
  }
}

float calcular_posicao() {
  long soma_ponderada = 0;
  long soma_leituras = 0;

  for (int i = 0; i < 8; i++) {
    soma_ponderada += (long)Leituras[i] * pesos[i];
    
  }

  if (soma_leituras == 0) return 0.0f; 
  
  return (float)soma_ponderada;
}

void pid_init(PIDController *pid, float kp, float ki, float kd, float min, float max) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->erro_anterior = 0.0f;
    pid->integral = 0.0f;
    pid->saida_min = min;
    pid->saida_max = max;
}

float pid_calcula(PIDController *pid, float setpoint, float atual, float dt) {
    float erro = setpoint - atual;
    float p = pid->kp * erro;
    
    pid->integral += erro * dt;
    float i = pid->ki * pid->integral;
    
    float derivativo = (erro - pid->erro_anterior) / dt;
    float d = pid->kd * derivativo;
    
    float saida = p + i + d;
    
    if (saida > pid->saida_max) {
        saida = pid->saida_max;
        pid->integral -= erro * dt; 
    } else if (saida < pid->saida_min) {
        saida = pid->saida_min;
        pid->integral -= erro * dt;
    }
    
    pid->erro_anterior = erro;
    return saida;
}

void AcionarMotores(int correcao) {
  int velocidadeEsq = VELOCIDADE_BASE + correcao;
  int velocidadeDir = VELOCIDADE_BASE - correcao;

  velocidadeEsq = constrain(velocidadeEsq, 0, 255);
  velocidadeDir = constrain(velocidadeDir, 0, 255);

  analogWrite(motorEsqPin, velocidadeEsq);
  analogWrite(motorDirPin, velocidadeDir);
}



/*

// Declaração das principais variaveis do programa
const int pinosSensores[10] = {4, 5, 6, 7, 15, 16, 17, 18, 8, 3}; // sensores QRE1113 anaalogicos que estão alinhados na frente do robô

const int sensores_traseiros[3] = {9, 14, 10};// sensores QRE1113 anaalogicos que estão alinhados na frente do robô

const int pesos[10] = {-100, -50, -35, -25, -1, 1, 25, 35, 50, 100}; // grau de reatividade em relção a posição do sensor. 

const int limiarSensor = 4000; // valor de limitção dos sensoresajustado para faixa branca com fundo preto

float kp = 12.2, ki = 0.04, kd = 0.8; //Determinação dos fatores de PID, Os valores do proporcional, integral e derivativo

float erroAnterior = 0, integral = 0; // variaveis de "memoria"

bool ajusteHabilitado = true; // botão virtual para ativar ajuste automatico do PID

// Pinagem e configuração de alguns aspectos do esp32


const int motorEsqPin = 46;//pinagem motor esquerdo
const int motorDirPin = 12;//pinagem motor esquerdo

const int canalEsq = 0; // inicialização do pwm do esquerdo
const int canalDir = 1; // inicialização do pwm do direito

const int freq = 1000; // Frequência de emição de sinal pwm
const int resolution = 8; // resolução da leitura do pwm 8 bits

int leituras[10]; //Leituras dos sensores frontais
int leituras2[3]; //Leituras dos sensores traseiros

int sensoresTraseirosAtivos[3]; // Percepição dos sensores ativos da traseira
int sensoresAtivos[10]; // Percepição dos sensores ativos da frente

int ultimoSensorLido = 0; // Variavel de memoria para o carro voltar a linha caso os sensores saim da pista
int ativar_boost = 0; // variavel do "botão de nitro"

unsigned long tempoErroBaixo = 0;

// essa Função é usada para um ajuste automatico e dinamico dos parametros de PID durante a trajetoria
void ajustarPID(float erro, float derivada) {
  if (!ajusteHabilitado) return; // se ajustehabilitado=false, pula função

  if (abs(erro) > 30) kp += 0.1; // se o valor absoluto do erro for maior que 30, incrmentar um valor em kp
  else if (abs(erro) < 5) kp -= 0.1;// se o valor absoluto do erro menor que 5 subtrair um valor em kp

  if (abs(derivada) > 10) kd += 0.05;// se o valor absoluto da derivada for maior que 10, incrmentar um valor em kd
  else kd -= 0.05; // se não for maior que 10 subtrair um valor

  if (abs(erro) < 10 && abs(derivada) < 2) ki += 0.01; // ajuste do valor de ki
  else ki -= 0.01;

  // constrain força um determinado intervalo

  kp = constrain(kp, 1.0, 20.0);// limitador para os valores proprcionais, força o kp ficar no intervalo 1 < kp < 20
  ki = constrain(ki, 0.0, 1.0);// limitador para os valores integrais, força o ki ficar no intervalo 0.0 < ki < 1
  kd = constrain(kd, 0.0, 5.0);// limitador para os valores derivativos, força o kd ficar no intervalo 0.0 < kd <5.0

  // impressão serial de valores
  Serial.print(" kp ");
  Serial.print(kp);
  Serial.print(" ki ");
  Serial.print(ki);
  Serial.print(" kd ");
  Serial.print(kd);
}

// tentativa de Multithreading, uso isolados de nucleos de processamento para execução em paralelo de aplicações

void tarefa1(void *pvParameters) {
  int tempo = 0; // contador de tempo. Pra ter certeza que estamos em uma reta
  //Laço infinito
  while (true) {
    if (tempo >= 3) {
      digitalWrite(13, HIGH);// fecha o contato dos reles no circuito do booster
      tempo = 0;// zera o contador de tempo
    } else {
      digitalWrite(13, LOW);// se não atender um minimo de tempo, não ativa o circuito de booster
    }

    // Condição de alinhamento para iniciar o loop de contagem de tempo. Os dois sensores laterais da traseira devem ler preto e o central deve ler breanco para que isso ocorra
    if ((!sensoresTraseirosAtivos[0] && sensoresTraseirosAtivos[1] && !sensoresTraseirosAtivos[2]) && (sensoresAtivos[4] && sensoresAtivos[5]))
    {
      tempo++;
    } 
    else 
    {
      tempo = 0;
    }
    // impressão das leituras em umdeterminado nucleo de processamento
    Serial.print("| Traseiros: ");
    Serial.print(sensoresTraseirosAtivos[0]);
    Serial.print(" ");
    Serial.print(sensoresTraseirosAtivos[1]);
    Serial.print(" ");
    Serial.print(sensoresTraseirosAtivos[2]);
    Serial.print(" ");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// configuração da placa
void setup() {
  digitalWrite(20, HIGH);
  Serial.begin(115200); // valocidade de burn-rate
  analogReadResolution(12);//Configura as leituras analógicas para terem 12 bits de resolução (valores de 0 a 4095)

  ledcSetup(canalEsq, freq, resolution); //configurção virtual do pwm
  ledcSetup(canalDir, freq, resolution); //configurção virtual do pwm

  ledcAttachPin(motorEsqPin, canalEsq); //atribuição do pwm o pino e saida
  ledcAttachPin(motorDirPin, canalDir); //atribuição do pwm o pino e saida


  //Cria uma nova tarefa (thread) chamada tarefa1 que roda em paralelo com o loop()
  if (ativar_boost == 1) {
    xTaskCreatePinnedToCore(tarefa1, "ativaBoost", 2048, //espaço de memória (stack size).
    NULL,
      1,
      NULL,
      0
    );
  }
}

void loop() {
  digitalWrite(13, HIGH);

  int soma_peso = 0;
  int soma_ativa = 0;

  for (int i = 0; i < 10; i++) {
    leituras[i] = analogRead(pinosSensores[i]);
    sensoresAtivos[i] = (leituras[i] < limiarSensor) ? 1 : 0;
    soma_peso += sensoresAtivos[i] * pesos[i];
    soma_ativa += sensoresAtivos[i];
  }

  for (int i = 0; i < 3; i++) {
    leituras2[i] = analogRead(sensores_traseiros[i]);
    sensoresTraseirosAtivos[i] = (leituras2[i] < limiarSensor) ? 1 : 0;
  }

  int motoresq = 0;
  int motordir = 0;
  bool nenhumAtivo = (soma_ativa == 0);

  if (sensoresAtivos[0] == 1 && sensoresAtivos[1] == 0) {
    ultimoSensorLido = 1;
  } else if (sensoresAtivos[9] == 1 && sensoresAtivos[8] == 0) {
    ultimoSensorLido = 2;
  }

  float erro = 0;
  float pid = 0;

  if (!nenhumAtivo) {
    erro = soma_peso / (float)soma_ativa;
    integral += erro;
    float derivada = erro - erroAnterior;

    pid = constrain((kp * erro + ki * integral + kd * derivada), -1000, 1000);
    erroAnterior = erro;
    // Autoajuste dos parâmetros PID

    if (false) {

      digitalWrite(11, HIGH);

    } else {
      motoresq = constrain(255 + pid, 0, 255);
      motordir = constrain(255 - pid, 0, 255);
    }
  } else {
    if (ultimoSensorLido == 1) {
      digitalWrite(13, HIGH);
      motoresq = 0;
      motordir = 255;
      erroAnterior = 0;
    } else if (ultimoSensorLido == 2)
    {
      digitalWrite(13, HIGH);
      motoresq = 255;
      motordir = 0;
      erroAnterior = 0;
    }
    else
    {
    }
  }

  ledcWrite(canalEsq, motoresq);
  ledcWrite(canalDir, motordir);

  Serial.println(digitalRead(13));
  delayMicroseconds(500);
}
*/