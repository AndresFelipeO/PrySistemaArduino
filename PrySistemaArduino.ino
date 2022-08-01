/**
  * @archivo PrySistemaArduino.ino
  *
  * Proyecto de ejemplo de Arquitectura
  *
  * @sección descripción Descripción
  * Maquina de estados que valida una contraseña y te da acceso a las configuraciones o a las tareas asincronicas
  *
  * @section bibliotecas Bibliotecas
  * - PrySistemaArduino (https://github.com/AndresFelipeO/PrySistemaArduino.git)
  * - Interactúa con la IMU integrada.
  *
  * @notas de sección Notas
  * - Los comentarios son compatibles con Doxygen.
  *
  * @autor de la sección Autor
  * - Creado por Andres Felipe Ocampo Chaguendo y Michael Stevens Diaz Beltran el 27/07/2022.
  *
  * Copyright (c) 2022 Taller de Arquitectura. Reservados todos los derechos.
  */
// Librerias
#include "Herramientas.h"
#include "Menu.h"
#include "Seguridad.h"
#include "Tareas.h"
#include "Alarma.h"

// Types

/** An enum type. 
 *  define states in State Machine 
 */
enum State
{
  PosicionA,
  PosicionB,
  PosicionC,
  PosicionD
};

/** An enum Input. 
 *  El tipo de enumeración de nivel de Input.
 */
enum Input
{
  Unknown,
  Reset,
  Forward,
  Backward,
  Block
};

// Variables globales
State currentState;
Input currentInput;
int incomingOpcion = 5;


/*F**************************************************************************
* NAME: stateA
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Acciones del estado A y condiciones de transiciones. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void stateA()
{
  if (currentInput == Input::Forward)
    changeState(State::PosicionB);
  if (currentInput == Input::Backward)
    changeState(State::PosicionC);
}

/*F**************************************************************************
* NAME: stateB
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Acciones del estado B y condiciones de transiciones. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void stateB()
{
  if (currentInput == Input::Backward)
    changeState(State::PosicionA);
  if (currentInput == Input::Forward)
    changeState(State::PosicionC);
}

/*F**************************************************************************
* NAME: stateC
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Acciones del estado C y condiciones de transiciones. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void stateC()
{
   if (currentInput == Input::Backward)
    changeState(State::PosicionB);
  if (currentInput == Input::Forward)
    changeState(State::PosicionD);
}

/*F**************************************************************************
* NAME: stateD
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Acciones del estado D y condiciones de transiciones. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void stateD()
{
    if (currentInput == Input::Forward)
    changeState(State::PosicionB);
}

/*F**************************************************************************
* NAME: outputA
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Salida A asociadas a las transiciones. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void outputA()
{
  
  incomingOpcion = seguridad();
  //if(incomingOpcion==1)
    incomingOpcion = MeNu();
  Serial.println(incomingOpcion);
}

/*F**************************************************************************
* NAME: outputB
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Salida B asociadas a las transiciones. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void outputB()
{
  //Serial.print("Estado configuracion ");
  incomingOpcion = configMenu();
  
  //Serial.println(incomingOpcion);
}


/*F**************************************************************************
* NAME: outputC
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Salida C asociadas a las transiciones. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void outputC()
{
  incomingOpcion =Temp();
}


/*F**************************************************************************
* NAME: outputD
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Salida D asociadas a las transiciones. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void outputD()
{
  incomingOpcion = Alarm();
}

/*F**************************************************************************
* NAME: setup
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* La función de instalación estándar de Arduino utilizada para las tareas de instalación y configuración. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void setup()
{
  // Initialize serial bus (Serial Monitor)
  Serial.begin(9600);
  // Initialize Lcd bus (Lcd Monitor)
  //lcd_Seg.begin(16,2);
  lcd.begin(16,2);
  //lcdT.begin(16,2);
  // inicialización tareas asincronicas
  asyncTaskTemp.Start();
  asyncTaskLuz.Start();
  asyncTaskMicro.Start();  
  currentState = PosicionA;
  outputA();
  // Pin configuration
  pinMode(ROJO,OUTPUT);
  pinMode(AZUL,OUTPUT);
  pinMode(VERDE,OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  pinMode(btn, INPUT);
  pinMode(BUZZER_PASIVO, OUTPUT); // pin 8 como salida
}

/*F**************************************************************************
* NAME: loop
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* La función de bucle Arduino estándar utilizada para tareas repetitivas. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void loop() 
{
  readInput();
  updateStateMachine();
}

/*F**************************************************************************
* NAME: updateStateMachine
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Actualiza el estado de la maquina. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void updateStateMachine()
{
  switch (currentState)
  {
    case PosicionA: stateA(); break;
    case PosicionB: stateB(); break;
    case PosicionC: stateC(); break;
    case PosicionD: stateD(); break;
  }
}

/*F**************************************************************************
* NAME: readInput
*----------------------------------------------------------------------------
* PARAMS:
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Lee la entrada por puerto serie. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void readInput()
{
  currentInput = Input::Unknown;

  Serial.println(incomingOpcion);
  Serial.println(currentInput);
    switch (incomingOpcion)
    {
      
    case 0: currentInput = Input::Backward; Serial.println(currentInput);  break;
    case 1: currentInput = Input::Forward; Serial.println(currentInput); break;
    default: break;
    }
}

/*F**************************************************************************
* NAME: readInput
*----------------------------------------------------------------------------
* PARAMS: newState el valor del nuevo estado.
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Funcion que cambia el estado y dispara las transiciones. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void changeState(State newState)
{
  currentState = newState;

  switch (currentState)
  {
    case State::PosicionA: outputA();   break;
    case State::PosicionB: outputB();   break;
    case State::PosicionC: outputC();   break;
    case State::PosicionD: outputD();   break;
    default: break;
  }
}
