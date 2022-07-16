
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>

//variables defenidas
#define ROJO 40
#define VERDE 34
#define  AZUL 32
#define beta 4090 //the beta of the thermistor
#define resistance 10 
#define clk 12
#define dt  13
#define btn 11
#define led 10

//pantalla pines
LiquidCrystal lcd(2, 3, 7, 6, 5, 4);
//teclad pines y asignacion de teclas
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','='}
};
byte rowPins[ROWS] = {47, 45, 43, 41}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {39, 37, 35}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//vatiables
int soundPin=A2;
String opciones[] = {"1.UTempHigh", "2.UTempLow", "3.ULuz", "4.USonido", "5.Reset","6.Salir"};
String opcionesEsc[] = {"TempHigh", "TempLow", "Luz", "Sonido", "Reset","Saliendo...."};
int max_opciones = 6;
int state_clk_old;
int state_btn_old;
int count = 0;
int pos = 0;
//funcion de configuracion temperatura
void Temperatura(void);
int addresTH=0;
char numeroTemp[3]="00";
float tempHig=25.0f;
int addresTL=1;
char numeroTempL[3]="00";
float tempLow=18.0f;
//funcion de configuracion luz
void Luz(void);
int addresL=2;
char numeroLuz[4]="000";
float luzValor=300.0f;
//funcion de configuracion microfono
void Microfono(void);
int addresM=3;
char numeroMicro[3]="00";
float sonido=30.0f;
int buttonState = 1;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  //lcd.backlight();
  pinMode(led, OUTPUT);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  pinMode(btn, INPUT);
  state_clk_old = digitalRead(clk);
  state_btn_old = digitalRead(btn);
  LecturaEeprom();
  mostrar_menu();
  
}

void LecturaEeprom(){
   tempHig = EEPROM.read(0);
   tempLow = EEPROM.read(1);
   luzValor = (EEPROM.read(2)*10)+EEPROM.read(3);
   sonido = EEPROM.read(4);
}

void reinicio(){
       EEPROM.write(0, 25); 
       EEPROM.put(1, 18); 
       EEPROM.put(2, 300/10); 
       EEPROM.write(3, 300%10);
       EEPROM.write(4, 30); 
}


void guardarEeprom(){
       EEPROM.write(0, tempHig);   
       EEPROM.write(1, tempLow); 
       EEPROM.write(2, luzValor/10); 
       EEPROM.write(3, (int)luzValor%10);
       EEPROM.write(4, sonido); 
}
void loop() {
  int state_btn = digitalRead(btn);
  encoder();
  char key = keypad.getKey();
  if (key){
    if((key- '0')==count+1 ||(key- '0')==count+2)
      run_option(key);
  }
}

//funcion que ejecuta la opcion del menu

void run_option(char opc){
  switch (opc){
  case '1':
    TemperaturaHigh();
    break;
  case '2':
    TemperaturaLow();
    break;
  case '3':
    LuzVar();
    break;
  case '4':
  SonidoVar();
    break;
  case '5':
    lcd.setCursor(0,0);
    lcd.print(opcionesEsc[4]);
    lcd.print("               ");
    reinicio();
    LecturaEeprom();
    lcd.setCursor(0,1);
    lcd.print("                  ");
    delay(2000);
    break;
  case '6':
    lcd.setCursor(0,0);
    lcd.print(opcionesEsc[5]);
    lcd.print("               ");
    //guardarEeprom();
    lcd.setCursor(0,1);
    lcd.print("                  ");
    delay(2000);
    break;
  }
  mostrar_menu();  
}
//configuracion de temperatura high
void TemperaturaHigh(){
  char ban='x';
  lcd.setCursor(0, 1);
  lcd.println("                        ");
  while(ban!='*'){
    lcd.setCursor(0,0);
    lcd.print(opcionesEsc[0]);
    lcd.print(":");
    lcd.print(tempHig);
    lcd.print("c");
    lcd.println("                        ");
    char key = keypad.getKey();
    if (key){
      if(key=='='){
        if(tempLow<numer(numeroTemp,pos))
          tempHig=numer(numeroTemp,pos);
        lcd.setCursor(0, 1);
      lcd.println("                        ");
      pos=0;
      }else if(key=='*')
        ban='*';
      else{
        numeroTemp[pos] = key;
        lcd.setCursor(pos++, 1);
        lcd.print(key);
        lcd.print("                  ");
        
      }
    }
    if(pos == 3)
      pos=0;
  }
  pos=0;
}
//configuracion de temperatura low
void TemperaturaLow(){
  char ban='x';
  lcd.setCursor(0, 1);
  lcd.println("                        ");
  while(ban!='*'){    
    lcd.setCursor(0,0);
    lcd.print(opcionesEsc[1]);
    lcd.print(":");
    lcd.print(tempLow);
    lcd.print("c");
    lcd.println("                        ");
    char key = keypad.getKey();
    if (key){
      if(key=='='){
        if(numer(numeroTempL,pos)<tempHig && numer(numeroTempL,pos)!=0)
          tempLow=numer(numeroTempL,pos);
        lcd.setCursor(0, 1);
      lcd.println("                        ");
        pos=0;
      }else if(key=='*')
        ban='*';
       else{
        numeroTempL[pos] = key;
        lcd.setCursor(pos++, 1);
        lcd.print(key);
        lcd.print("                       ");
      }
    }
    if(pos == 3)
      pos=0;
  }
  pos=0;
}
//configuracion de luz
void LuzVar(){
  char ban='x';
  lcd.setCursor(0, 1);
  lcd.println("                        ");
  while(ban!='*'){
    lcd.setCursor(0,0);
    lcd.print(opcionesEsc[2]);
    lcd.print(":");
    lcd.print(luzValor);
    lcd.print("p");
    lcd.println("                        ");
    char key = keypad.getKey();
    if (key){
      if(key=='='){
        if(numer(numeroLuz,pos)>0)
          luzValor=numer(numeroLuz,pos);
        lcd.setCursor(0, 1);
      lcd.println("                        ");
        pos=0;
      }
      else if(key=='*')
        ban='*';
      else{
        numeroLuz[pos] = key;
        lcd.setCursor(pos++, 1);
        lcd.print(key);
        lcd.print("                       ");
      }
    }
    if(pos == 4)
      pos=0;
  }
  pos=0;
}
//configuracion de sonido
void SonidoVar(){
  char ban='x';
  lcd.setCursor(0, 1);
  lcd.println("                        ");
  while(ban!='*'){
    lcd.setCursor(0,0);
    lcd.print(opcionesEsc[3]);
    lcd.print(":");
    lcd.print(sonido);
    lcd.println("                        ");
    char key = keypad.getKey();
    if (key){
      if(key=='='){
        if(numer(numeroMicro,pos)>0)
          sonido=numer(numeroMicro,pos);
        lcd.setCursor(0, 1);
      lcd.println("                        ");
        pos=0;
      }else if(key=='*')
        ban='*';
      else{
        numeroMicro[pos] = key;
        lcd.setCursor(pos++, 1);
        lcd.print(key);
        lcd.print("                       ");
      }
    }
    if(pos == 3)
      pos=0;
    }
  pos=0;
}
//Funcion que convierte un arreglo de caracteres en un numero flotante
float numer(char caracter[],int tam){//convierte una cadena de caracteres en numero
    float comoEntero=0;
    for(int i=0;i<tam;i++){
        comoEntero+= caracter[i] - '0';
        if(i<tam-1)
            comoEntero*=10;
    }
    return comoEntero;
}
//funcion que le da un valor a count dependiendo del encoder
void encoder(){
  int state_clk = digitalRead(clk);
  int state_dt = digitalRead(dt);
  
  if(state_clk_old == HIGH && state_clk == LOW){
    if(state_dt == LOW){
      count--;
    }else{
      count++;
    }
    
    if(count < 0) count = max_opciones-2;
    else if(count > max_opciones-2) count = 0;
    
    mostrar_menu();
  }

  delay(5);
  state_clk_old = state_clk;
}
//funcion que muestra el menu
void mostrar_menu(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(opciones[count]);
    
  lcd.setCursor(0,1);
  lcd.print(opciones[count+1]);
}

//funcion que muestra la temperatura
void Temperatura(void){
  lcd.setCursor(0, 0);
  lcd.print("Temperatura                ");
   //read thermistor value
long a =1023 - analogRead(A0);
//the calculating formula of temperature
float tempC = beta /(log((1025.0*10/a-10) / 10)+beta/298.0)-273.0;
Serial.print("Temperatura: ");
Serial.println(tempC);
lcd.setCursor(0, 1);
      lcd.print(tempC);
      lcd.println("C               ");
    if(tempC>tempHig){
      LED_RGB(255,0,0);
    }else if(tempC>tempLow){
      LED_RGB(0,0,255);
      
    }else{
      LED_RGB(0,255,0);
      
    }
    
}
//funcion que muestra la luz
void Luz(void){
  lcd.setCursor(0, 0);
  lcd.print("Luz                             ");
  int luz=analogRead(A1);
Serial.print("Luz: ");
  Serial.println(luz);
  lcd.setCursor(0, 1);
  lcd.print(luz);
  lcd.println("p            ");
    if(luz>luzValor){
      LED_RGB(0,255,0);
    }else{
      LED_RGB(0,0,255);
      
      }
}
//funcion que prende una led dependiendo de la intencidad del sonido
void Microfono(void){
  int value = analogRead(soundPin);
  Serial.println(value);
  if(value > sonido){
    LED_RGB(125,33,129);
    delay(500);
  }
  else{
    LED_RGB(0,0,0);
  }
}

//funcion que prende la led y le da un color
void LED_RGB(int r,int g,int b){
  analogWrite(ROJO,r);
  analogWrite(VERDE,g);
  analogWrite(AZUL,b);
}
