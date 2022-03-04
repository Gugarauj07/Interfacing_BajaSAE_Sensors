#include <SoftwareSerial.h>//include the library code
#include <LiquidCrystal_I2C.h>;
#include <Adafruit_MLX90614.h> //Sensor de temperatura

#define pinVEL 2 // Pino de interrupção para rotação da roda
#define RAIO_RODA 0.037 // Em metros

Adafruit_MLX90614 mlx = Adafruit_MLX90614(); 

SoftwareSerial HC12(6, 7); // HC-12 TX Pin, HC-12 RX Pin

// Define o endereço utilizado pelo Adaptador I2C
LiquidCrystal_I2C lcd(0x27,20,4);

unsigned long Velocidade_millisInicial = 0; //tempo inicial para velocidade 
unsigned long telemetria_millisInicial = 0; //tempo inicial para telemetria
unsigned long Temp_millisInicial = 0; //

volatile int pulsosVEL = 0; //contador de pulsos para RPM 

unsigned int RPMvel = 0; //frequencia de rotacoes da roda
unsigned int VEL = 0; //velocidade em km/h

double temp_amb; 
double temp_obj; 

int a;

String armazenar = "";

void setup() {
  // Inicializa Serial
  Serial.begin(9600);

  attachInterrupt (digitalPinToInterrupt(pinVEL), tacometro, RISING); //Interrupção para ler pulso do RPM

  HC12.begin(9600);

  mlx.begin(); //inicializa o sensor de temperatura

  lcd.init();
  lcd.backlight();
  
}

void loop() {
  velocidade();
  temperatura();

  hc12();
  
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print(" |");
  lcd.setCursor(5, 1);
  lcd.print(" |");
  lcd.setCursor(5, 2);
  lcd.print(" |");
  lcd.setCursor(5, 3);
  lcd.print(" |");
  lcd.setCursor(0, 0);
  lcd.print("SPEED");
  lcd.setCursor(0, 2);
  lcd.print(VEL);
  lcd.setCursor(8, 0);
  lcd.print("RPM");
  lcd.setCursor(8, 2);
  lcd.print("1500");

  lcd.setCursor(12, 0);
  lcd.print(" |");
  lcd.setCursor(12, 1);
  lcd.print(" |");
  lcd.setCursor(12, 2);
  lcd.print(" |");
  lcd.setCursor(12, 3);
  lcd.print(" |");

  lcd.setCursor(15, 0);
  lcd.print("TEMP");
  lcd.setCursor(15, 2);
  lcd.print(temp_obj);
  delay(1000);

}
