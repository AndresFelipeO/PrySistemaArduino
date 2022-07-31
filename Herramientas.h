// Librerias
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>
#include "AsyncTaskLib.h"

// Defines
#define ROJO 40
#define VERDE 34
#define  AZUL 32
#define beta 4090 //the beta of the thermistor
#define resistance 10 
#define clk 12
#define dt  13
#define btn 11
#define led 10
int posM = 0;


#define bt A8

//variables globlaes
int estado;
float currentMillis;
float previousMillis;

//metodos
void LED_RGB(int, int, int);
void ImprimirMensaje(String);
void textoEstados(String);
void textoFila1(String, float, String);
void textoFila2(char);
void limpiar();
void imprimirDatos(String, float, String);

// Types
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

/*F**************************************************************************
* NAME: LED_RGB
*----------------------------------------------------------------------------
* PARAMS: r La intensidad del color rojo.
* PARAMS: g La intensidad del color verde.
* PARAMS: b La intensidad del color azul.
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* Funcion que convina colores para la led. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void LED_RGB(int r,int g,int b){
  analogWrite(ROJO,r);
  analogWrite(VERDE,g);
  analogWrite(AZUL,b);
}


/*F**************************************************************************
* NAME: segundos
*----------------------------------------------------------------------------
* PARAMS: 
* return: segunods transcurridos.
*----------------------------------------------------------------------------
* PURPOSE:
* Funcion que calcula los segundos que han trancurrido. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
int segundos(){
  return ((currentMillis - previousMillis)/1000);
}


/*F**************************************************************************
* NAME: limpiar
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* Funcion limpia la pantalla de lcd. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void limpiar(){
  lcd.setCursor(0, 0);
  lcd.println("             ");
  lcd.setCursor(0, 1);
  lcd.println("                ");
  posM=0;
}
