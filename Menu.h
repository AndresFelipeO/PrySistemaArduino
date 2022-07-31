// variables globales
int soundPin=A2;
String opciones[] = {"1.UTempHigh", "2.UTempLow", "3.ULuz", "4.USonido", "5.Reset","6.Guardar y cont.","7.Guardar y Sal."};
String opcionesEsc[] = {"TempHigh", "TempLow", "Luz", "Sonido", "Reset","Entrando....","Saliendo...."};
int max_opciones = 7;
int state_clk_old;
int state_btn_old;
int count = 0;


//funcion de configuracion temperatura
char numeroTemp[3]="00";
float tempHig=25.0f;

char numeroTempL[3]="00";
float tempLow=18.0f;

//funcion de configuracion luz
char numeroLuz[4]="000";
float luzValor=300.0f;

//funcion de configuracion microfono
char numeroMicro[3]="00";
float sonido=30.0f;

int buttonState = 1;
//metodos
void mostrar_menu();
void encoder();
void run_option(char);
void LuzVar();
void SonidoVar();
float numer(char[], int);
void TemperaturaHigh();
void TemperaturaLow();

/*F**************************************************************************
* NAME: LecturaEeprom
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* asigna en las variables globales lo que este en la EEPROM. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void LecturaEeprom(){
   tempHig = EEPROM.read(0);
   tempLow = EEPROM.read(1);
   luzValor = (EEPROM.read(2)*10)+EEPROM.read(3);
   sonido = EEPROM.read(4);
}

/*F**************************************************************************
* NAME: reinicio
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* reinicia a los valores por defecto en la memoria de la EEPROM. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void reinicio(){
       EEPROM.write(0, 25); 
       EEPROM.put(1, 18); 
       EEPROM.put(2, 300/10); 
       EEPROM.write(3, 300%10);
       EEPROM.write(4, 30); 
}

/*F**************************************************************************
* NAME: guardarEeprom
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* Guarda lo que este en las variables goblaes en la EEPROM. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void guardarEeprom(){
       EEPROM.write(0, tempHig);   
       EEPROM.write(1, tempLow); 
       EEPROM.write(2, luzValor/10); 
       EEPROM.write(3, (int)luzValor%10);
       EEPROM.write(4, sonido); 
}

/*F**************************************************************************
* NAME: configMenu
*----------------------------------------------------------------------------
* PARAMS: 
* return: retorna el estado de menu de la maquina.
*----------------------------------------------------------------------------
* PURPOSE:
* Menu despegable de configuracion. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
int configMenu() {
  LecturaEeprom();
  estado = 5;
  count = 0;
  mostrar_menu();
  do{
    int state_btn = digitalRead(btn);
    encoder();
    char key = keypad.getKey();
    if (key){
      if((key- '0')==count+1 ||(key- '0')==count+2)
       run_option(key);
    }
  }while(estado != 0 && estado != 1);
 return estado; 
}


/*F**************************************************************************
* NAME: run_option
*----------------------------------------------------------------------------
* PARAMS: 
* return: opc El valor de la opcion.
*----------------------------------------------------------------------------
* PURPOSE:
* funcion que ejecuta la opcion del menu. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void run_option(char opc){
  switch (opc){
  case '1':
    //estado = 0;
    TemperaturaHigh();
    break;
  case '2':
    //estado = 1;
    TemperaturaLow();
    break;
  case '3':
    LuzVar();
    break;
  case '4':
  SonidoVar();
    break;
  case '5':
    reinicio();
    LecturaEeprom();
    textoEstados(opcionesEsc[4]); 
    break;
  case '6':
    estado = 1;
    guardarEeprom();
    textoEstados(opcionesEsc[5]); 
    break;
  case '7':
    estado = 0;
    guardarEeprom();
    textoEstados(opcionesEsc[6]); 
    break;
  }
  mostrar_menu();  
}
/*F**************************************************************************
* NAME: TemperaturaHigh
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* configuracion de temperatura high. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void TemperaturaHigh(){
  char ban='x';
  limpiar();
  while(ban!='*'){
    textoFila1(opcionesEsc[0], tempHig, "c");
    char key = keypad.getKey();
    if (key){
      if(key=='='){
        if(tempLow<numer(numeroTemp,posM))
          tempHig=numer(numeroTemp,posM);
        limpiar();
      }else if(key=='*')
        ban='*';
      else{
        numeroTemp[posM] = key;
        textoFila2(key);
      }
    }
    if(posM == 3)
      posM=0;
  }
  posM=0;
}

/*F**************************************************************************
* NAME: TemperaturaLow.
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* configuracion de temperatura low. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void TemperaturaLow(){
  char ban='x';
  limpiar();
  while(ban!='*'){    
    textoFila1(opcionesEsc[1], tempLow, "c");
    char key = keypad.getKey();
    if (key){
      if(key=='='){
        if(numer(numeroTempL,posM)<tempHig && numer(numeroTempL,posM)!=0)
          tempLow=numer(numeroTempL,posM);
        limpiar();
      }else if(key=='*')
        ban='*';
       else{
        numeroTempL[posM] = key;
        textoFila2(key);
      }
    }
    if(posM == 3)
      posM=0;
  }
  posM=0;
}

/*F**************************************************************************
* NAME: LuzVar
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* configuracion de la luz. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void LuzVar(){
  char ban='x';
  limpiar();
  while(ban!='*'){
    textoFila1(opcionesEsc[2], luzValor, "p");
    char key = keypad.getKey();
    if (key){
      if(key=='='){
        if(numer(numeroLuz,posM)>0)
          luzValor=numer(numeroLuz,posM);
        limpiar();
      }
      else if(key=='*')
        ban='*';
      else{
        numeroLuz[posM] = key;
        textoFila2(key);
      }
    }
    if(posM == 4)
      posM=0;
  }
  posM=0;
}

/*F**************************************************************************
* NAME: SonidoVar
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* configuracion del sonido. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void SonidoVar(){
  char ban='x';
  limpiar();
  while(ban!='*'){
    textoFila1(opcionesEsc[3], sonido, "dB");
    char key = keypad.getKey();
    if (key){
      if(key=='='){
        if(numer(numeroMicro,posM)>0)
          sonido=numer(numeroMicro,posM);
        limpiar();
      }else if(key=='*')
        ban='*';
      else{
        numeroMicro[posM] = key;
        textoFila2(key);
      }
    }
    if(posM == 3)
      posM=0;
    }
  posM=0;
}

/*F**************************************************************************
* NAME: numer
*----------------------------------------------------------------------------
* PARAMS: caracter   El vector que se va a convertir en un numero flotante.
* PARAMS: tam  El tamaño del vector.
* return: El numero ingreaso en el vector.
*----------------------------------------------------------------------------
* PURPOSE:
* Funcion que convierte un arreglo de caracteres en un numero flotante
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
float numer(char caracter[],int tam){//convierte una cadena de caracteres en numero
    float comoEntero=0;
    for(int i=0;i<tam;i++){
        comoEntero+= caracter[i] - '0';
        if(i<tam-1)
            comoEntero*=10;
    }
    return comoEntero;
}

/*F**************************************************************************
* NAME: encoder
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* funcion que le da un valor a count dependiendo del encoder.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
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

/*F**************************************************************************
* NAME: mostrar_menu
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* funcion que muestra el menu.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void mostrar_menu(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(opciones[count]);
  Serial.println(opciones[count]);
    
  lcd.setCursor(0,1);
  lcd.print(opciones[count+1]);
  Serial.println(opciones[count+1]);
}

/*F**************************************************************************
* NAME: textoEstados
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* funcion el texto de los estados.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void textoEstados(String OpcionElegida){
  lcd.setCursor(0,0);
  lcd.print(OpcionElegida);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  delay(2000);
}

/*F**************************************************************************
* NAME: textoFila1
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* funcion que muestra texto en la fila 1 del lcd.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void textoFila1(String opcionEl, float valorDato, String tipoMedida){
  lcd.setCursor(0,0);
  lcd.print(opcionEl);
  lcd.print(":");
  lcd.print(valorDato);
  lcd.print(tipoMedida);
  lcd.println("          ");
}

/*F**************************************************************************
* NAME: textoFila2
*----------------------------------------------------------------------------
* PARAMS: 
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* funcion que muestra texto en la fila 2 del lcd.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void textoFila2(char valorDato){
  lcd.setCursor(posM++, 1);
  lcd.print(valorDato);
  lcd.print("               ");
}
