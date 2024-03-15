// C++ code
/* 
Aproveitando a montaxe da práctica anterior, imos sustituír un dos motores 
para que o que queda realice a función de despregado (marcha directa) e 
pregado (marcha inversa), usando un pulsador. O toldo debe iniciarse no estado
de pregado, unha vez aceso. Por suposto, o control do pregado/despregado faise 
usando dous fins de carreira e o motor debe pasar por paro antes de invertir o xiro.

Por outra parte, o control da velocidade do pregado/despregado faise tamén 
empregando un sensor de luminosidade (LDR), de maneira que canto maior é a luminosidade,
máis veloz é o motor no despregado e canto menor é a luminosidade, menos veloz é o motor 
no pregado. O valor obtido do sensor de luminosidade categorízase en 5 niveis: 0, 1, 2,
3, 4 e 5, de maneira que no nivel 1 a velocidade do motor será do 20%, no nivel 2 do 40% 
e así sucesivamente até que no nivel 5 a velocidade do motor é do 100%. 
No nivel 0 a velocidade do motor é do 5%.

Autor: Pablo Cameán Garrido
Data: Marzo 2024
*/

//Declaración de pines
#define PULS 10       // Pulsador
#define INVS 3        // Inversor
#define MOTORUP 5     // Motor sube
#define MOTORDOWN 6   // Motor baixa
#define FC1 11        // Final de carreira 1
#define FC2 9         // Final de carreira 2
#define LDR A1        // Sensor de luminosidade

//Declaración de variables
int estado = 0;        // Variable para controlar o estado do sistema
int motorActivo = 0;   // Variable para controlar qué motor está activo
int valorLDR = 0;      // Almacena o valor lido do sensor de luminosidade
int rangomap = 0;      // Almacena o valor mapeado do sensor de luminosidade

void setup() {
  pinMode(PULS, INPUT_PULLUP);   // Pin do pulsador como entrada con pull-up
  pinMode(MOTORUP, OUTPUT);      // Pin do motor que sube como saída
  pinMode(MOTORDOWN, OUTPUT);    // Pin do motor que baixa como saída
  pinMode(5, OUTPUT);            // Pin 5 como saída
  pinMode(6, OUTPUT);            // Pin 6 como saída
  pinMode(A1, INPUT);            // Pin do sensor de luminosidade como entrada
  Serial.begin(9600);            // Inicializa a comunicación serial a 9600 baudios
}

void loop() {
  
  analogRead(LDR);    // Le os valores do sensor de luminosidade
  valorLDR = analogRead(LDR);
  rangomap = map(valorLDR, 54, 974, 0, 5);  // Mapea o valor lido do sensor de luminosidade a 5 niveis
  Serial.print("Valor LDR: "); Serial.println(rangomap);  // Imprime o valor mapeado do sensor de luminosidade
  
  // Control da velocidade do motor según o nivel de luminosidade
  if (rangomap == 0){
    analogWrite(INVS, 12.5);
  }
  if (rangomap == 1){
    analogWrite(INVS, 51);
  } 
  if (rangomap == 2){
    analogWrite(INVS, 102);
  }
  if (rangomap == 3){
    analogWrite(INVS, 153);
  }
  if (rangomap == 4){
    analogWrite(INVS, 204);
  }
  if (rangomap == 5){
    analogWrite(INVS, 255);
  }
  
  // Control da dirección do motor ao presionar o pulsador
  if (digitalRead(PULS) == 1) {
    
    // Apaga ambos motores ao cambiar o motor activo
    digitalWrite(MOTORUP, LOW);
    digitalWrite(MOTORDOWN, LOW);
    if (estado > 0){
      estado = 0; 
    }
    else  {
      motorActivo = !motorActivo;  // Cambia o estado do motor activo
      if (motorActivo == 1) {  
        digitalWrite(MOTORDOWN, HIGH);  // Activa o motor que baixa
        Serial.println("MotorDOWN activado");
      } 
      else {
        digitalWrite(MOTORUP, HIGH);  // Activa o motor que sube
        Serial.println("MotorUP activado");
      } 
      estado += 1;
    }
    // Espera a que se solte o pulsador
    while(digitalRead(PULS) == 1) { 
      delay(10);
    }
  }

  // Se hai un motor activo e se alcanza un final de carreira, apagase ese motor
  if (estado > 0) {
    if (digitalRead(FC1) && motorActivo){
      digitalWrite(MOTORUP, LOW);                
    } 
    else if (digitalRead(FC2) && !motorActivo) {
      digitalWrite(MOTORDOWN, LOW); 
    }
    delay(10);
  } 
  else {
    // Se no hai ningún motor activo, apaganse ambos motores
    digitalWrite(MOTORUP, LOW);
    digitalWrite(MOTORDOWN, LOW);
    Serial.println("Ambos motores apagados");
  }

  // Printea o estado actual do sistema
  Serial.print("Estado: ");
  Serial.println(estado);
}
