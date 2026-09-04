#include <Arduino.h>

#define IR 12
#define motorEsqPin  19 
#define motorDirPin  18 

//============================= Configurações de Hardware ================================================
const int pinosSensores[8] = {34, 35, 32, 13, 25, 26, 27, 14}; 
const int pesos[8] = {-100, -50, -35, -25, 25, 35, 50, 100}; 

// Tamanho do filtro (Quantas leituras passadas guardar). 
// Valores entre 4 e 8 costumam ser ideais. Muito alto gera atraso (delay) na resposta física.
#define TAMANHO_FILTRO 5 

// Matriz para armazenar o histórico: 8 sensores x N leituras passadas
int historicoLeituras[8][TAMANHO_FILTRO];
int indiceFiltro = 0; // Aponta para a posição atual da matriz a ser subscrita

const int VELOCIDADE_BASE = 150; 

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

int leiturasFiltradas[8]; // Guardará as leituras suavizadas após a estatística

unsigned long tempo_anterior = 0;

void configBoard();
void ConfigPwm();
void pid_init(PIDController *pid, float kp, float ki, float kd, float min, float max);
float pid_calcula(PIDController *pid, float setpoint, float atual, float dt);
void LeiturasComFiltro();
float calcular_posicao();
void AcionarMotores(int correcao);

void setup() {
  Serial.begin(115200);
  configBoard();
  
  // Inicializa a matriz de histórico com zeros
  for(int s = 0; s < 8; s++) {
    for(int f = 0; f < TAMANHO_FILTRO; f++) {
      historicoLeituras[s][f] = 0;
    }
  }

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

void configBoard() {
  pinMode(IR, OUTPUT);
  pinMode(motorEsqPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  for (int i = 0; i < 8; i++) {
    pinMode(pinosSensores[i], INPUT);
  }
}


void ConfigPWM();

// Nova função de leitura estruturada em Matriz
void LeiturasComFiltro() {
  // 1. Lê os valores físicos e armazena na coluna atual da matriz de histórico
  for(uint8_t i = 0; i < 8; i++) {
    historicoLeituras[i][indiceFiltro] = analogRead(pinosSensores[i]);
  }

  // 2. Avança o índice da matriz de forma circular (quando chega no fim, volta para 0)
  indiceFiltro++;
  if(indiceFiltro >= TAMANHO_FILTRO) {
    indiceFiltro = 0;
  }

  // 3. Processamento Estatístico (Média Aritmética) do histórico de cada sensor
  for(uint8_t i = 0; i < 8; i++) {
    long soma = 0;
    for(uint8_t f = 0; f < TAMANHO_FILTRO; f++) {
      soma += historicoLeituras[i][f];
    }
    // Armazena o resultado suavizado (sem flutuações rápidas de ruído)
    leiturasFiltradas[i] = soma / TAMANHO_FILTRO;
  }


  for (int i = 0; i < 8; i++) {
    Serial.print("Posicao [");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(leiturasFiltradas[i]);
  }
}

// Modificado para usar o array estabilizado 'leiturasFiltradas'
float calcular_posicao() {
  long soma_ponderada = 0;
  long soma_leituras = 0;

  for (int i = 0; i < 8; i++) {
    soma_ponderada += (long)leiturasFiltradas[i] * pesos[i];
    soma_leituras += leiturasFiltradas[i];
  }

  if (soma_leituras == 0) return 0.0f; 
  
  return (float)soma_ponderada / soma_leituras;
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
#include <Arduino.h>

#define IR 12
#define motorEsqPin  19 
#define motorDirPin  18 

#define TAMANHO_FILTRO 5 

//============================= Configurações de Hardware ================================================
const int pinosSensores[8] = {34, 35, 32, 13, 25, 26, 27, 14}; 
const int pesos[8] = {-100, -50, -35, -25, 25, 35, 50, 100}; // Pesos para cálculo da posição
// Matriz para armazenar o histórico: 8 sensores x N leituras passadas
int historicoLeituras[8][TAMANHO_FILTRO];
int indiceFiltro = 0; // Aponta para a posição atual da matriz a ser subscrita

// Velocidade base dos motores (ajuste conforme necessário)
const int VELOCIDADE_BASE = 150; 
const int LimiarSensorBranco = 2500;

// Estrutura para armazenar os dados do PID
typedef struct {
    float kp;       
    float ki;       
    float kd;       
    float erro_anterior;
    float integral; 
    float saida_min; 
    float saida_max; 
} PIDController;

// Instanciação global do PID para não perder o histórico de erro/integral entre os ciclos
PIDController meu_pid;

int leituras[8]; 
unsigned long tempo_anterior = 0;

// Declaração das funções (Protótipos corrigidos)
void configBoard();
void pid_init(PIDController *pid, float kp, float ki, float kd, float min, float max);
float pid_calcula(PIDController *pid, float setpoint, float atual, float dt);
void Leituras();
float calcular_posicao();
void AcionarMotores(int correcao);

void setup() {
   Serial.begin(115200);
  configBoard();
  
  // Inicializa a matriz de histórico com zeros
  for(int s = 0; s < 8; s++) {
    for(int f = 0; f < TAMANHO_FILTRO; f++) {
      historicoLeituras[s][f] = 0;
    }
  }

  pid_init(&meu_pid, 2.5f, 0.0f, 0.5f, -150.0f, 150.0f); 
}

void loop() {
  digitalWrite(IR, HIGH); // Liga os emissores IR
  
  Leituras(); // Atualiza o array 'leituras'
  
  // Calcula o tempo decorrido dinamicamente (dt)
  unsigned long tempo_atual = millis();
  float dt = (tempo_atual - tempo_anterior) / 1000.0f;
  if (dt <= 0.0f) dt = 0.001f; // Evita divisão por zero
  tempo_anterior = tempo_atual;

  float posicao_linha = calcular_posicao();
  float setpoint = 0.0f; // O centro perfeito da linha é zero
  
  // O PID calcula a correção necessária baseada no desvio da linha
  float correcao = pid_calcula(&meu_pid, setpoint, posicao_linha, dt);
  
  AcionarMotores((int)correcao);
}

void configBoard() {
  pinMode(IR, OUTPUT);
  pinMode(motorEsqPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(pinosSensores[i], INPUT);
  }
}

void Leituras() {
  for(uint8_t i = 0; i < 8; i++) {
    leituras[i] = analogRead(pinosSensores[i]);
  }
}

// Calcula o centro de massa/posição do robô em relação à linha
float calcular_posicao() {
  long soma_ponderada = 0;
  long soma_leituras = 0;

  for (int i = 0; i < 8; i++) {
    // IMPORTANTE: Se a linha for preta, use a leitura direta. 
    // Se a linha for branca em fundo preto, pode ser necessário inverter (4095 - leituras[i])
    soma_ponderada += (long)leituras[i] * pesos[i];
    soma_leituras += leituras[i];
  }

  if (soma_leituras == 0) return 0.0f; // Evita divisão por zero se nenhum sensor ler nada
  
  return (float)soma_ponderada / soma_leituras;
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
    
    // Anti-windup e saturação
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
  // Aplica a correção diferencial nos motores
  int velocidadeEsq = VELOCIDADE_BASE + correcao;
  int velocidadeDir = VELOCIDADE_BASE - correcao;

  // Garante que os valores fiquem dentro do limite do PWM (0 a 255)
  velocidadeEsq = constrain(velocidadeEsq, 0, 255);
  velocidadeDir = constrain(velocidadeDir, 0, 255);

  // NOTA: Para controle real de velocidade, você deve substituir analogWrite por 
  // funções de PWM específicas caso esteja usando ESP32 (ledcWrite), 
  // mas mantive a estrutura básica nos pinos definidos.
  analogWrite(motorEsqPin, velocidadeEsq);
  analogWrite(motorDirPin, velocidadeDir);
}



#include <Arduino.h>

#define IR 12

#define motorEsqPin  19 //pinagem motor esquerdo

#define motorDirPin  18 //pinagem motor esquerdo

// Declaração das principais variaveis do programa

//=============================Configurações de Hardware ================================================

const int pinosSensores[8] = {34,35,32,13,25,26,27,14}; // sensores anaalogicos que estão alinhados na frente do robô

const int pesos[8] = {-100, -50, -35, -25, 25, 35, 50, 100}; // grau de reatividade em relção a posição do sensor.

//=============================||||||||||||||||||||||||||||||===========================================


// Estrutura para armazenar os dados do PID
typedef struct {
  //Determinação dos fatores de PID, Os valores do proporcional, integral e derivativo
    float kp;       // Ganho proporcional
    float ki;       // Ganho integral
    float kd;       // Ganho derivativo
    
    float erro; // Erro anterior (ou use "erro_anterior")
    float erro_anterior;
    float integral; // Acumulador do termo integral
    
    float saida_min; // Limite inferior da saída
    float saida_max; // Limite superior da saída
} PIDController;



int leituras[8]; //Leituras dos sensores frontais
int valor;


const int canalEsq = 0; // inicialização do pwm do esquerdo

const int canalDir = 1; 

void configBoard();
void pid_init(PIDController *pid, float kp, float ki, float kd, float min, float max);
float pid_calcula(PIDController *pid, float setpoint, float atual, float dt);
void SensorCalibrations();
void Leituras();
void configPwm(const uint8_t Canal, const uint16_t Frequencia, const uint8_t Resolucao);
void PID();
void AcionarMotores(const int i, const int j);

void setup() {
  // put your setup code here, to run once:

   Serial.begin(115200);

   configBoard();
 
  
  
 
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(12,HIGH);
  Leituras();
  AcionarMotores(motorEsqPin,motorDirPin);
}

void ConfigBoard(){

  pinMode(IR,OUTPUT);

  pinMode(motorEsqPin,OUTPUT);

  pinMode(motorDirPin,OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(pinosSensores[i], INPUT);
  }


}

void Leituras()
{

  for(uint8_t i = 0; i < 8; i++)
  {

    valor = analogRead(pinosSensores[i]);
    Serial.print(" S");
    Serial.print(i + 1); // i + 1 faz começar em S1 em vez de S0
    Serial.print("=");
    Serial.print(valor);
  

    // Adiciona uma vírgula e espaço entre as leituras, exceto no último elemento
    if (i < 7) {
      Serial.print(", ");
    }

    delay(500);
  }

  Serial.println();


}

void configPwm(){


}

void AcionarMotores(const int i, const int j){

  PIDController meu_pid;
    // Inicializa com Kp=2.0, Ki=0.5, Kd=0.1, saída mínima=0 e máxima=100
    pid_init(&meu_pid, 2.0f, 0.5f, 0.1f, 0.0f, 100.0f);
    
    float setpoint = 50.0f; // Valor desejado
    float valor_atual = 40.0f; // Valor lido do sensor
    float dt = 0.1f; // Tempo de amostragem em segundos (ex: 100ms)
    
    float controle = pid_calcula(&meu_pid, setpoint, valor_atual, dt);
    
    printf("Sinal de controle gerado: %.2f\n", controle);


  digitalWrite(i, HIGH);
  digitalWrite(j, HIGH);
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

// Função de cálculo do PID a cada ciclo
float pid_calcula(PIDController *pid, float setpoint, float atual, float dt) {
    // 1. Calcula o erro atual
    float erro = setpoint - atual;
    
    // 2. Termo Proporcional
    float p = pid->kp * erro;
    
    // 3. Termo Integral (acumula o erro no tempo)
    pid->integral += erro * dt;
    float i = pid->ki * pid->integral;
    
    // 4. Termo Derivativo (taxa de variação do erro)
    float derivativo = (erro - pid->erro_anterior) / dt;
    float d = pid->kd * derivativo;
    
    // 5. Soma os três termos para obter a saída
    float saida = p + i + d;
    
    // 6. Saturação (Anti-windup básico) para respeitar os limites do atuador
    if (saida > pid->saida_max) {
        saida = pid->saida_max;
        // Opcional: congelar o integrador para evitar windup excessivo
        pid->integral -= erro * dt; 
    } else if (saida < pid->saida_min) {
        saida = pid->saida_min;
        pid->integral -= erro * dt;
    }
    
    // 7. Salva o erro para o próximo ciclo
    pid->erro_anterior = erro;
    
    return saida;
}

float pid_calcula(PIDController *pid, float setpoint, float atual, float dt) {
    // 1. Calcula o erro atual
    float erro = setpoint - atual;
    
    // 2. Termo Proporcional
    float p = pid->kp * erro;
    
    // 3. Termo Integral (acumula o erro no tempo)
    pid->integral += erro * dt;
    float i = pid->ki * pid->integral;
    
    // 4. Termo Derivativo (taxa de variação do erro)
    float derivativo = (erro - pid->erro_anterior) / dt;
    float d = pid->kd * derivativo;
    
    // 5. Soma os três termos para obter a saída
    float saida = p + i + d;
    
    // 6. Saturação (Anti-windup básico) para respeitar os limites do atuador
    if (saida > pid->saida_max) {
        saida = pid->saida_max;
        // Opcional: congelar o integrador para evitar windup excessivo
        pid->integral -= erro * dt; 
    } else if (saida < pid->saida_min) {
        saida = pid->saida_min;
        pid->integral -= erro * dt;
    }
    
    // 7. Salva o erro para o próximo ciclo
    pid->erro_anterior = erro;
    
    return saida;
}*/