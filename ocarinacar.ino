
#include "arduinoFFT.h"    //Es necesario instalar la libreria de arduinoFFT desde el propio gestor de librerias de arduino, he instalado la versin de Enrique Condes.

const uint16_t samples = 128;
const uint16_t umbral = 100;

double vReal[samples];
double vImag[samples];

boolean grabar = false;

int i=0;

int datoanterior=0;
int ruido=0;

int posmaxant=0;

arduinoFFT FFT = arduinoFFT();

void setup(){
  
  Serial.begin(115200);

  pinMode(2,OUTPUT);
  pinMode(14,OUTPUT);
  
  delay(2000);

  ruido=0;
  for(i=0;i<100;i++) {
    ruido+=analogRead(4);
    delay(10);
  }

  ruido=ruido/100;
   
}

void loop(){

   int dato=analogRead(4);   
   if((dato<ruido+10)&&(dato>ruido-10)) { return;}

   //Serial.println(dato);
   //return;
    
   grabar=true;
 //  Serial.println("Grabando");


   while(grabar) {

   for(int i=0;i<samples;i++){
      vReal[i] = analogRead(4);
      vImag[i] = 0;
      delayMicroseconds(100);
   }
   
   FFT.Windowing(vReal,samples,FFT_WIN_TYP_RECTANGLE,FFT_FORWARD);
   FFT.Compute(vReal,vImag,samples,FFT_FORWARD);
   FFT.ComplexToMagnitude(vReal,vImag,samples);

   boolean superior=false;
   for(uint16_t j = 2;j<(samples/2);j++){  //La posicion 0,1 y 2 la quitamos por que siempre es superior a 100 , y no nos da información relevante.
      if(vReal[j]>umbral) { superior=true;break; }      
   }
   
   if(superior) {

    
    int maximo=0;
    int posmax=0;
    int maximos[32];
    int posmaximos[32];    
    int contador=0;
    
    for(i=0;i<32;i++){ maximos[i]=0;}
    
       
    for(uint16_t j=2;j<(samples/2);j++){  
      
        if(vReal[j]>umbral) {
          
          if(vReal[j]>=maximos[contador]){                         
            
            maximos[contador]=vReal[j];
            posmaximos[contador]=j;
            
          }else if(maximos[contador]!=0) { 
            contador++;  
          }               
          
        }else if(maximos[contador]!=0) { 
            contador++;  
        }

        if(vReal[j]>maximo) { maximo=vReal[j];posmax=j;}
        
    }


    int max2=0;
    int posmax2=0;
    
    for(i=0;i<contador;i++){
      if((maximos[i]>max2)&&(posmaximos[i]!=posmax)) {
        max2=maximos[i];
        posmax2=posmaximos[i];
      }
    }

    int max3=0;
    int posmax3=0;
    
    for(i=0;i<contador;i++){
      if((maximos[i]>max3)&&(posmaximos[i]!=posmax)&&(posmaximos[i]!=posmax2)) {
        max3=maximos[i];
        posmax3=posmaximos[i];
      }
    }

    if(posmax==posmaxant) {
      if(posmax==11) { 
        Serial.println("Parar"); 
        digitalWrite(2,LOW);
        digitalWrite(14,LOW);
      }else if(posmax==8) { 
        Serial.println("Adelante"); 
        digitalWrite(2,HIGH);
        digitalWrite(14,HIGH);
      }else if(posmax==9) { 
        Serial.println("Izquierda"); 
        digitalWrite(2,HIGH);
        digitalWrite(14,LOW);
      }else if(posmax==10) { 
        Serial.println("Derecha"); 
        digitalWrite(2,LOW);
        digitalWrite(14,HIGH);
      }
    }

    posmaxant=posmax;
 /*
    Serial.print("F1 :");
    Serial.print(posmax); 
    Serial.print("     P1 :");
    Serial.println(maximo); 
        
    Serial.print("F2 :");
    Serial.print(posmax2);     
    Serial.print("     P2 :");
    Serial.println(max2); 
    
    Serial.print("F3 :");
    Serial.print(posmax3);     
    Serial.print("     P3 :");
    Serial.println(max3); 
    
*/

      
    //Se muestran todos los máximos 
/*    
    for(i=0;i<contador;i++){
        Serial.print("Máximo ");
        Serial.print(i);
        Serial.print(" : ");
        Serial.println(posmaximos[i]);           
    }
*/        
 
     //Serial.println("");      
     //Serial.println("");      
     
    // Serial.println("-");    
       
   }else {   //No hay ningúna frecuencia destacada

     if(grabar) { 
     //   Serial.println("Dejamos de grabar");      
        grabar=false;
     }
    
   }

   }//while(grabar)
   
}
