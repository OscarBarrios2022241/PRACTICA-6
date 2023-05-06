#include <Wire.h>//eLI
#include <LiquidCrystal_I2C.h>//Libreria para controlar el Modulo I2C
#include <RTClib.h>//Libreria para controlar el reloj de tiempo
#include <OneWire.h>//Libreria para el sensor de temperatura
#include <DallasTemperature.h>//Libreria para el sensor de temperatura

RTC_DS1307 rtc;//Se crea la variable para el modulo del reloj
LiquidCrystal_I2C lcd(0x27, 16, 2);//ÑLa direccion I2C y la cantidad de filas y columnas de la LCD

#define ONE_WIRE_BUS 2//Pin del sensor
OneWire oneWire(ONE_WIRE_BUS);//Se crea una variable para el snesor de temperatura
DallasTemperature sensors(&oneWire);//Se crea una variable para el snesor de temperatura

int buzzer = 8; //Pin para el buzzer
int rele = 9; //Pin para el relé de dos canales y la bomba de agua

void setup() {
  pinMode(buzzer, OUTPUT);//Se pone el pin del buzzer como salida
  pinMode(rele, OUTPUT);//Se pone el pin del del rele como salida

  digitalWrite(rele, LOW); //Indica que el rele este apagado al inicio

  lcd.init();//Inicializa la LCD
  lcd.backlight();//Enciende la luz de la LCD

  sensors.begin();//Se inicia el sensor de temperatura

  if (! rtc.begin()) {
    lcd.print("Error en el RELOJ DE TIEMPO");//Si no está funcionando muestra un mensaje de error en la LCD
    while (1);//Entra en un bucle
  }

  if (! rtc.isrunning()) {
    lcd.print("RELOJ DE TIEMPO no esta funcionando");// Si no tiene una hora establecida muestra un mensaje en la pantalla LCD
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//ajusta la hora del Reloj de tiempo
  }
}

void loop() {
  DateTime now = rtc.now();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatura: ");

  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  lcd.print(temp);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print(now.year(),DEC);
  lcd.print('/');
  lcd.print(now.month(),DEC);
  lcd.print('/');
  lcd.print(now.day(),DEC);
  lcd.print(' ');
  lcd.print(now.hour(),DEC);
  lcd.print(':');
  lcd.print(now.minute(),DEC);
  lcd.print(':');
  lcd.print(now.second(),DEC);

  if (now.hour() == 21 && now.minute() == 21) {//Aqui se cambian las horas y los minutos
    lcd.clear();
    lcd.setCursor(0, 0);//Es donde quiero que comience el texto en la LCD
    lcd.print("RIEGO EN 20 SEG.");//Muestra "RIEGO EN 20 SEG en la LCD"
    digitalWrite(rele, LOW);//Apaga el rele
    delay(20000);//Tiempo de 20 seg
    digitalWrite(rele, HIGH); //Enciende el rele

    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);


  } else{

    digitalWrite(rele, HIGH);
  }


}
