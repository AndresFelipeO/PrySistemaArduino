// Defines
#define C4  262
#define CS4 277
#define D4  294
#define DS4 311
#define E4  330
#define F4  349
#define FS4 370
#define G4  392
#define GS4 415
#define A4  440
#define AS4 466
#define B4  494
#define C5  523
#define CS5 554
#define D5  587
#define PULSADOR 2    // pulsador en pin 2
#define BUZZER_PASIVO 11    // buzzer pasivo en pin 8

// Types
// Global Vectores Melodia molinos de viento autor mago de oz
int melodia[] = {   // array con las notas de la melodia
  G4,G4,G4,G4,G4,G4,A4,G4,FS4,G4,G4,FS4,E4,G4,G4,G4,G4,G4,G4,A4,G4,FS4,G4,G4,FS4,D4,//26
  FS4,FS4,FS4,FS4,FS4,FS4,G4,FS4,FS4,E4,FS4,G4,C4,C4,C4,E4,G4,D4,D4,D4,FS4,A4,//22
  G4,E4,G4,E4,G4,E4,FS4,G4,A4,FS4,D4,E4,FS4,G4,E4,G4,E4,//17
  G4,E4,G4,E4,G4,E4,FS4,G4,A4,FS4,D4,E4,FS4,G4,E4,G4,E4,//17
  B4,G4,B4,G4,B4,G4,B4,C5,D5,A4,FS4,G4,A4,B4,G4,A4,B4,//17
    B4,G4,B4,G4,B4,G4,B4,C5,D5,A4,FS4,G4,A4,B4,G4,A4,B4//17
};
//duracion de las notas de la melodia
int duraciones[] = {    // array con la duracion de cada nota
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
 7,8,7,8,7,8,8,8,8,7,8,8,8,8,7,8,5,
  7,8,7,8,7,8,8,8,8,7,8,8,8,8,7,8,5,
  7,8,7,8,7,8,8,8,8,7,8,8,8,8,7,8,5,
  7,8,7,8,7,8,8,8,8,7,8,8,8,8,7,8,5
};

/*F**************************************************************************
* NAME: Alarm
*----------------------------------------------------------------------------
* PARAMS:
* return: el resulto de que ha terminado la cancion.
*----------------------------------------------------------------------------
* PURPOSE:
* Reproduce la melodia en el sensor (buzzer). 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
int Alarm() {
    for (int i = 0; i < 116; i++) {     // bucle repite 25 veces
      int duracion = 1000 / duraciones[i];    // duracion de la nota en milisegundos
      tone(BUZZER_PASIVO, melodia[i], duracion);  // ejecuta el tono con la duracion
      int pausa = duracion * 1.30;      // calcula pausa
      delay(pausa);         // demora con valor de pausa
      noTone(BUZZER_PASIVO);        // detiene reproduccion de tono
    }
    return 1;
}
