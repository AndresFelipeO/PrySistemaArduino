// Librerias
#include <LiquidCrystal.h>
#include "AsyncTaskLib.h"

// Defines
#define bt A8

//types
//variables globlaes
float tempC=0;

//metodos
void Temperatura(void);
void Luz(void);
void Microfono(void);

//tareas asincronicas
AsyncTask asyncTaskTemp(4000,true,Temperatura);
AsyncTask asyncTaskLuz(5000,true,Luz);
AsyncTask asyncTaskMicro(6000,true,Microfono);

/*F**************************************************************************
* NAME: Temp
*----------------------------------------------------------------------------
* PARAMS:
* return: retorna el estado de las tareas asincronicas.
*----------------------------------------------------------------------------
* PURPOSE:
* Asigna una tarea asincronica en un intervalo de tiempo.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
int Temp(){
  //previousMillis=millis();
  do{
    if(digitalRead(bt)==LOW){
      currentMillis = millis();
      Serial.println(currentMillis);
      Serial.println(digitalRead(bt));
       Serial.println(segundos());
      if(segundos()>=3){
        previousMillis=currentMillis;
        return 0;
      }
    }
    if(digitalRead(bt)==HIGH){
      previousMillis = millis();
      Serial.println(digitalRead(bt));
    }
    asyncTaskTemp.Update(asyncTaskLuz);
    asyncTaskLuz.Update(asyncTaskMicro);
    asyncTaskMicro.Update(asyncTaskTemp);
    delay(1000);
  }while(tempC<30);
  lcd.clear();
  imprimirDatos("Temperatura alta", tempC, "C");
  return 1;
}

/*F**************************************************************************
* NAME: Temperatura
*----------------------------------------------------------------------------
* PARAMS:
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* con el sensor muestra la temperatura registrada del lugar.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void Temperatura(void){
   //read thermistor value
  long a =1023 - analogRead(A0);
  //the calculating formula of temperature
  tempC = beta /(log((1025.0*10/a-10) / 10)+beta/298.0)-273.0;

  limpiar();
  imprimirDatos("Temperatura", tempC, "C");
    if(tempC>tempHig){
      LED_RGB(255,0,0);
    }else if(tempC>tempLow){
      LED_RGB(0,0,255);
      
    }else{
      LED_RGB(0,255,0);
      
    }
    //return tempC;
}

/*F**************************************************************************
* NAME: Luz
*----------------------------------------------------------------------------
* PARAMS:
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* con el sensor muestra la luz registrada del lugar.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void Luz(void){
  int luz=analogRead(A1);
  limpiar();
  imprimirDatos("Luz", luz, "p");
  if(luz>luzValor)
    LED_RGB(0,255,0);
  else
    LED_RGB(0,0,255);
}

/*F**************************************************************************
* NAME: Microfono
*----------------------------------------------------------------------------
* PARAMS:
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* funcion que prende una led dependiendo de la intencidad del sonido.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void Microfono(void){
  int value = analogRead(soundPin);
  limpiar();
  imprimirDatos("Sonido", value, "dB");
  if(value > sonido){
    LED_RGB(125,33,129);
    delay(500);
  }
  else
    LED_RGB(0,0,0);
}

/*F**************************************************************************
* NAME: imprimirDatos
*----------------------------------------------------------------------------
* PARAMS: titulo, titulo de la primera fila del lcd
* PARAMS: datoValor, el valor registrado por el sensor que se quiera imprimir
* PARAMS:tipoMedida, la medida dependiendo del tipo de dato
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* funcion que prende una led dependiendo de la intencidad del sonido.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void imprimirDatos(String titulo, float datoValor, String tipoMedida){
  lcd.setCursor(0, 0);
  lcd.print(titulo);
  lcd.print("              ");
  lcd.setCursor(0, 1);
  lcd.print(datoValor);
  lcd.print(tipoMedida);
  lcd.println("            ");
}
