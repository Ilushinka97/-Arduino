#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
float freq, n1=100, n2=10000, k, n; // Оглашение переменных -- частота ( = 10000), диапазон (в Гц), Гц в сложении на 1 такт
int t=30; //(время качания) 
#define W_CLK A1 // Пин A1 Arduino - подключен к CLK
#define FQ_UD A2 // Пин A2 Arduino - подключен к FQ (FU)
#define DATA  A4 // Пин A3 Arduino - подключен к DATA
#define RESET A5 // Пин A4 Arduino - подключен к RST
#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }
//-------------------------------------------------------

void tfr_byte(byte data) // Функция побитной отправки байта
{                         // данных в модуль генератора
  for (int i=0; i<8; i++, data>>=1) 
  { 
   digitalWrite(DATA, data & 0x01); //Выставление на пинах напряжения
   pulseHigh(W_CLK);}} // Подача импульса на CLK после каждого бита
   
//-------------------------------------------------------
void sendFrequency(double frequency) // Преобразование и отправка
{                                     // значения частоты
  int32_t freq = frequency * 4294967295/125000000;  
  for (int i=0; i<4; i++, freq>>=8) 
{ tfr_byte(freq & 0xFF); }
  tfr_byte(0x000);   // Отправка завершательного контрольного байта
  pulseHigh(FQ_UD);}  // Обновление частоты генератора
//----------------------------------------------------

void setup()
{
lcd.begin(16, 2); // Старт библиотеки. Указанием количества символов и строк
pinMode(FQ_UD, OUTPUT);
pinMode(W_CLK, OUTPUT);
pinMode(DATA, OUTPUT);
pinMode(RESET, OUTPUT);
pulseHigh(RESET); // Отправка импульсов для запуска модуля генератора
pulseHigh(W_CLK);
pulseHigh(FQ_UD);
}
//----------------------------------------------------

void loop()
{
lcd.setCursor(0,0); 
n=n1/1000;
lcd.print (n,1);//("Swing Freq Gen-r"); // Обозначен диапазон
lcd.setCursor(3,0);
lcd.print ("-");
n=n2/1000;
lcd.setCursor(4,0);
lcd.print (n,0);
lcd.setCursor(7,0);
lcd.print ("kHz");

lcd.setCursor(11,0); // Секундомер

unsigned long tstamp = millis();
int s = (tstamp / 1000) % t;
String line = String(s) + "s";
int len = line.length();
    while(len < 16)
    {
        line += " ";
        len++;
    }
lcd.print ("T:");  // Вывод секундомера
    lcd.print(line);
lcd.setCursor(0,1); // Вывод значения частоты в данный момент времени
lcd.print ("Fq:      Hz");//lcd.print ("Rg: 100-10000 Hz"); 
k=(((n2-n1)/t)/4.583334); // 
freq=freq+k; //+72 200
if (freq>=n2)
 {   freq=n1; }

sendFrequency(freq);  // Вызов функции отправки частоты
lcd.setCursor(4,1); //отображение актуальной частоты на экране
lcd.print (freq,0);
delay (200); // Пауза 200 мс
}
