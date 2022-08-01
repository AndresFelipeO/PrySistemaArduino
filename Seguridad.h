//variables globales
char claveCorrecta[8] ="1234567";
char claveIngreso[8];
int estado_Seg = 0;
int comprobarContra();
int pos = 0;
int contaError = 0;

/*F**************************************************************************
* NAME: MeNu
*----------------------------------------------------------------------------
* PARAMS: 
* return: retorna el estado de menu.
*----------------------------------------------------------------------------
* PURPOSE:
* Menu despegable de tareas o configuracion de tareas.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
int MeNu() {
  lcd.setCursor(0, 0);
  lcd.println("                                ");
  lcd.setCursor(0, 1);
  lcd.println("                              ");
  lcd.setCursor(0, 0);
  lcd.println("1.Tareas                    "); 
  lcd.setCursor(0, 1);
  lcd.println("2.Configuracion               ");
  int opc=5;
  count = 0;
  do{
    char key = keypad.getKey();
    if (key){
      opc= (key- '0')-1;
    }
  }while(opc!=0 && opc!=1);
 return opc; 
}


/*F**************************************************************************
* NAME: seguridad
*----------------------------------------------------------------------------
* PARAMS: 
* return: etorna el estado de seguridad.
*----------------------------------------------------------------------------
* PURPOSE:
* Funcion que valida que la contraseña ingresada por el usuario sea la correcta.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
int seguridad(){
  estado_Seg = 0;
  lcd.setCursor(0, 0);
  Serial.println("                                ");
  lcd.setCursor(0, 1);
  lcd.println("                              ");
  Serial.println("Seguridad");
  lcd.setCursor(0, 0);
  lcd.print("Ingrese clave:   ");
  //delay(2000); 
  do{
    currentMillis = millis();
    char key = keypad.getKey();
    claveIngreso[pos] = key;
    if (key){
    lcd.setCursor(pos++, 1);
    lcd.println("*               ");
    previousMillis=currentMillis;
  }
  if(pos == 7){
      pos = 0;
      if(comprobarContra()==1){
          //Prender led verde
          LED_RGB(0,255,0);
          ImprimirMensaje("Clave Correcta");
          estado_Seg = 1;
          delay(2000);
          LED_RGB(0,255,0);
          delay(1000);
          LED_RGB(0,0,255);
          delay(1000);
          LED_RGB(255,0,0);
          delay(1000);
          LED_RGB(0,0,0);
          ImprimirMensaje("                ");
          }else{
            //Prender led amarillo
            LED_RGB(0,0,255);
            ImprimirMensaje("Clave Incorrecta");
          delay(2000);  
          ImprimirMensaje("                ");
          LED_RGB(0,0,0);
            contaError++;
          }
    }
    if(contaError == 3){
      //Activar led rojo y esperar 10 seg
      //Despues reiniciar
      LED_RGB(255,0,0);
      ImprimirMensaje("Block System");
      delay(10000);//se bloquea por 10 segundos
      contaError=0;
      ImprimirMensaje("                ");
      LED_RGB(0,0,0);
    }
    if(segundos() >= 7){
      pos = 0;
      lcd.setCursor(pos, 1);
      lcd.println("                ");
    }
  }while(estado_Seg != 1);
  return estado_Seg;
}

/*F**************************************************************************
* NAME: comprobarContra
*----------------------------------------------------------------------------
* PARAMS: 
* return: etorna el estado de seguridad.
*----------------------------------------------------------------------------
* PURPOSE:
* la validacion de la contraseña.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
int comprobarContra(){
  int contaOk = 0;
  for(int i=0; i < 7; i++)
    if(claveIngreso[i]==claveCorrecta[i])    
        contaOk++;
   
  if(contaOk == 7)
    return 1;
   return 0;
    
}

/*F**************************************************************************
* NAME: ImprimirMensaje
*----------------------------------------------------------------------------
* PARAMS: mensaje   El valor del mensaje.
* return: none.
*----------------------------------------------------------------------------
* PURPOSE:
* funcion que imprime un mensaje en la pantalla lcd.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void ImprimirMensaje(String mensaje){
  lcd.setCursor(0, 1);
  lcd.print(mensaje);
}
