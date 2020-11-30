/*
Botões:

STOP:  pino 6
START:  pino 7


Led's:

Sistema ligado: pino 3
Tanque cheio: pino 4


TIP120:

Aquecedor:  pino 9
Misturador: pino 10
Bomba1:   pino 11
Bomba2:   pino 12 
Bomba3:   pino 13
*/

//I/O's
  
int tmp = 0;  
#define on 3
#define cheio 4
#define stop 5
#define start 6
float sigPin = 7;
#define aquecedor  9
#define misturador  10
#define bomba1 11
#define bomba2 12
#define bomba3 13

//Estados

#define inicio 0
#define encheA 1
#define aqueceA 2
#define repouso 3
#define encheB 4
#define aqueceAB 5
#define mistura 6
#define eliminacao1 7
#define aqueceAB2 8
#define eliminacao2 9
#define checagem 10
#define esvazia 11


int estados;

float litros;  
float cm;
float temperatura; 

void setup()
{
  Serial.begin(9600);
  
 pinMode(stop, INPUT);
 pinMode(start, INPUT);
  
 pinMode(on, OUTPUT); 
 pinMode(cheio, OUTPUT);
 pinMode(aquecedor, OUTPUT);  
 pinMode(misturador, OUTPUT); 
 pinMode(bomba1, OUTPUT);   
 pinMode(bomba2, OUTPUT);
 pinMode(bomba3, OUTPUT);
  
 estados = inicio;
}

void leituras(){
  
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, LOW);
  delayMicroseconds(5);
  
  digitalWrite(sigPin, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(sigPin, LOW);

  pinMode(sigPin, INPUT);
  
  float duracao = pulseIn(sigPin, HIGH);
  cm = (0.0343*duracao)/2;

  Serial.print("Volume: ");
  Serial.print(litros);
  Serial.print("L   ");
  
  litros = (7.8539*(300-cm));

  
  
  int leitura = analogRead(tmp);
  float tensao = leitura*(5000/1024.0);
  temperatura = (tensao - 500)/10;
  
  Serial.print("Temperatura: "); 
  Serial.print(temperatura);
  Serial.print("\xB0");
  Serial.print("C   ");
    
  delay(100);
}  

void loop(){

  switch(estados)
  {
 
    case inicio:
    
    leituras();
    Serial.println("Inicio");
    digitalWrite(on, LOW);
  digitalWrite(cheio, LOW);  
  digitalWrite(aquecedor, LOW);  
  digitalWrite(misturador, LOW);  
  digitalWrite(bomba1, LOW);  
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba3, LOW);

    
    if (digitalRead (start)==HIGH && digitalRead(stop) == LOW)
    {
     estados = encheA;
    }
    
    if((litros > 0.0) && digitalRead(start) == HIGH && digitalRead(stop) == LOW)
    {
     estados = esvazia;
    }
    break;
    
    case encheA:
    
    leituras();
    Serial.println("Enchendo de A");
    digitalWrite(on, HIGH);
  digitalWrite(cheio, LOW);  
  digitalWrite(aquecedor, LOW);  
  digitalWrite(misturador, LOW);  
  digitalWrite(bomba1, HIGH);  
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba3, LOW);
       
    if(litros >= 1200.0)
    {
     estados = aqueceA;
    }       
    
    if (digitalRead (stop)==HIGH)
    {
     estados = checagem;
    }
    break;
  
    case aqueceA:
    
    leituras();
    Serial.println("Aquecendo A");
    digitalWrite(on, HIGH);
  digitalWrite(cheio, LOW);  
  digitalWrite(aquecedor, HIGH);  
  digitalWrite(misturador, LOW);  
  digitalWrite(bomba1, LOW);  
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba3, LOW);
    
    if(temperatura >= 65.0)
    {
     estados = repouso;
    } 
    
    if (digitalRead (stop)==HIGH)
    {
     estados = checagem;
    }  
    break;
  
  case repouso:
    
    leituras();
    Serial.println("Repouso");   
    digitalWrite(on, HIGH);
  digitalWrite(cheio, LOW);  
  digitalWrite(aquecedor, LOW);  
  digitalWrite(misturador, LOW);  
  digitalWrite(bomba1, LOW);  
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba3, LOW);
    
    delay(5000);
    estados = encheB;  
    break;
  
    case encheB:
    
    leituras();
    Serial.println("Enchendo de B");    
    digitalWrite(on, HIGH);
  digitalWrite(cheio, LOW);  
  digitalWrite(aquecedor, LOW);  
  digitalWrite(misturador, LOW);  
  digitalWrite(bomba1, LOW);  
  digitalWrite(bomba2, HIGH); 
  digitalWrite(bomba3, LOW);
    
    if(litros >= 2000.0)
    {
     estados = aqueceAB;
    }
     
    if (digitalRead (stop)==HIGH)
    {
     estados = checagem;
    }   
    break;
  
    case aqueceAB:
    
    leituras();
    Serial.println("Aquecendo regentes");    
    digitalWrite(on, HIGH);
  digitalWrite(cheio, HIGH);  
  digitalWrite(aquecedor, HIGH);  
  digitalWrite(misturador, LOW);  
  digitalWrite(bomba1, LOW);  
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba3, LOW);
    
    if(temperatura >= 55.0)
    {
     estados = mistura;
    } 
     
    if (digitalRead (stop)==HIGH)
    {
     estados = checagem;
    }
    break;
    
    case mistura:
    
    leituras();
    Serial.println("Misturando reagentes");    
    digitalWrite(on, HIGH);
  digitalWrite(cheio, HIGH);  
  digitalWrite(aquecedor, LOW);  
  digitalWrite(misturador, HIGH);  
  digitalWrite(bomba1, LOW);  
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba3, LOW);
    
    delay(5000);
    estados = eliminacao1;
    
    if (digitalRead (stop)==HIGH)
    {
     estados = checagem;
    }
    break;
  
    case eliminacao1:
    
    leituras();
    Serial.println("Primeira eliminacao");    
    digitalWrite(on, HIGH);
  digitalWrite(aquecedor, LOW);  
  digitalWrite(misturador, LOW);  
  digitalWrite(bomba1, LOW);  
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba3, HIGH);
    
    if(litros <= 1400.0)
    {
     estados = aqueceAB2;
    } 
    
    if(cm <= 45.0)
    {
     digitalWrite(cheio, HIGH);
    }
  else
    {
     digitalWrite(cheio, LOW);
    }
    
    if (digitalRead (stop)==HIGH)
    {
     estados = checagem;
    }
        
    break;
  
    case aqueceAB2:
    
    leituras();
    Serial.println("Aquecendo a mistura");    
    digitalWrite(on, HIGH);
  digitalWrite(cheio, LOW);  
  digitalWrite(aquecedor, HIGH);  
  digitalWrite(misturador, LOW);  
  digitalWrite(bomba1, LOW);  
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba3, LOW);
    
    if(temperatura >= 55.0)
    {
     estados = eliminacao2;
    } 
    
    if (digitalRead (stop)==HIGH)
    {
     estados = checagem;
    }
    break;
  
    case eliminacao2:
    
    leituras();
    Serial.println("Segunda eliminacao");    
    digitalWrite(on, HIGH);
  digitalWrite(cheio, LOW);  
  digitalWrite(aquecedor, LOW);  
  digitalWrite(misturador, LOW);  
  digitalWrite(bomba1, LOW);  
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba3, HIGH);
    
    if(litros <= 0.0)
    {
     estados = inicio;
    } 
     
    if (digitalRead (stop)==HIGH)
    {
     estados = checagem;
    }   
    break;
  
    case checagem:
    
    leituras();
    Serial.println("Checagem de presenca de liquido");    
    digitalWrite(on, LOW);
  digitalWrite(cheio, LOW);  
  digitalWrite(aquecedor, LOW);  
  digitalWrite(misturador, LOW);  
  digitalWrite(bomba1, LOW);  
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba3, LOW);
    
    if((litros > 0.0) && digitalRead(start) == HIGH && digitalRead(stop) == LOW)
    {
     estados = esvazia;
    } 
    
    if((litros <= 0.0) && digitalRead(stop) == LOW && digitalRead(start) == HIGH )
    {
     estados = encheA;
    }     
    break;
  
    case esvazia:
    
    leituras();
    Serial.println("Esvaziamento do tanque");    
    digitalWrite(on, HIGH);
  digitalWrite(cheio, LOW);  
  digitalWrite(aquecedor, LOW);  
  digitalWrite(misturador, LOW);  
  digitalWrite(bomba1, LOW);  
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba3, HIGH);
     
    if(litros <= 0.0)
    {
     estados = encheA;
    }
    
    if (digitalRead (stop)==HIGH)
    {
     estados = checagem;
    }  
    break;
  }  
}
