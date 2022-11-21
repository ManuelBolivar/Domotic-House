int salon_abrir = 2; //puerta salón , print 1
int salon_cerrar = 3; // print 0
int chimenea = 4; // on 2, off 3
int garaje_abrir = 5; // on 4
int garaje_cerrar = 6; //off 5
int fuente = 7; // on 6, off 7
int cocina = 53; // on 8, off 9
int encimera = 52; //on 10, off 11
int fogon1 = 51; // on 12, off 13
int fogon2 = 50; // on 14, off 15
int salon = 49; // on 16, off 17
int dormitorio = 48; // on 18, off 19
int bano = 47; // on 20, off 21
int piscina = 46; // on 22, off 23
boolean encender = LOW;
boolean apagar = HIGH;
int Received2;
char Received1;
String Received;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  pinMode(salon_abrir, OUTPUT);
  pinMode(salon_cerrar, OUTPUT);
  pinMode(chimenea, OUTPUT);
  pinMode(garaje_abrir, OUTPUT);
  pinMode(garaje_cerrar, OUTPUT);
  pinMode(fuente, OUTPUT);
  pinMode(cocina, OUTPUT);
  pinMode(encimera, OUTPUT);
  pinMode(fogon1, OUTPUT);
  pinMode(fogon2, OUTPUT);
  pinMode(salon, OUTPUT);
  pinMode(dormitorio, OUTPUT);
  pinMode(bano, OUTPUT);
  pinMode(piscina, OUTPUT);
  digitalWrite(salon_abrir, apagar);
  digitalWrite(salon_cerrar, apagar);
  digitalWrite(chimenea, apagar);
  digitalWrite(garaje_abrir, apagar);
  digitalWrite(garaje_cerrar, apagar);
  digitalWrite(fuente, apagar);
  digitalWrite(cocina, apagar);
   digitalWrite(encimera, apagar);
   digitalWrite(fogon1, apagar);
   digitalWrite(fogon2, apagar);
   digitalWrite(salon, apagar);
   digitalWrite(dormitorio, apagar);
   digitalWrite(bano, apagar);
   digitalWrite(piscina, apagar);
}

void loop() {

      int Received2 = Serial2.read();
      Serial2.println(Received2);
      delay(1000);
    

    //PUERTA DE SALÓN
   if (Received2 == '1') //abrir salón
    {
      digitalWrite(salon_cerrar, apagar);
      digitalWrite(salon_abrir, encender);
      delay(11000);
      digitalWrite(salon_abrir, apagar);
    }
    if (Received2 == '0') // cerrar salón
    {
      digitalWrite(salon_abrir, apagar);
      digitalWrite(salon_cerrar, encender);
      delay(11000);
      digitalWrite(salon_cerrar, apagar);
    }
    
 //SALÓN
    if (Received2 == '16')
    {
      digitalWrite(salon, encender);
    }
    if (Received2 == '17')
    {
      digitalWrite(salon, apagar);
    }
  
    //CHIMENEA
    if (Received2 == '2')
    {
      digitalWrite(chimenea, encender);
    }
    if (Received2 == '3')
    {
      digitalWrite(chimenea, apagar);
    }
    
      //COCINA
    if (Received2 == '8')
    {
      digitalWrite(cocina, encender);
    }
    if (Received2 == '9')
    {
      digitalWrite(cocina, apagar);
    }

    //ENCIMERA
    if (Received2 == '10')
    {
      digitalWrite(encimera, encender);
    }
    if (Received2 == '11')
    {
      digitalWrite(encimera, apagar);
    }

    //FOGÓN1
    if (Received2 == '12')
    {
      digitalWrite(fogon1, encender);
    } 
    if (Received2 == '13')
    {
      digitalWrite(fogon1, apagar);
    } 

    //FOGÓN2
    if (Received2 == '14')
    {
      digitalWrite(fogon2, encender);
    }
    if (Received2 == '15')
    {
      digitalWrite(fogon2, apagar);
    }
  

   
    //DORMITORIO
    if (Received2 == '18')
    {
      digitalWrite(dormitorio, encender);
    }
    if (Received2 == '19')
    {
      digitalWrite(dormitorio, apagar);
    }
    
    //BAÑO
    if (Received2 == '20')
    {
      digitalWrite(bano, encender);
    }
    if (Received2 == '21')
    {
      digitalWrite(bano, apagar);
    }
   
    //PUERTA DE GARAJE
    if (Received2 == '4' ) //abrir garaje 4
    {
      digitalWrite(garaje_cerrar, apagar);
      digitalWrite(garaje_abrir, encender);
      delay(11000);
      digitalWrite(garaje_abrir, apagar);
    }
    if (Received2 == '5') // cerrar garaje 5
    {
      digitalWrite(garaje_abrir, apagar);
      digitalWrite(garaje_cerrar, encender);
      delay(11000);
      digitalWrite(garaje_cerrar, apagar);
    }

    //FUENTE
    if (Received2 == '53')
    {
      digitalWrite(fuente, encender);
    }
    if (Received2 == '54')
    {
      digitalWrite(fuente, apagar);
    }

    //PISCINA
    if (Received2 == '55')
    {
      digitalWrite(piscina, encender);
    }
    if (Received2 == '23')
    {
      digitalWrite(piscina, apagar);
    }

  
    
  if (Serial.available() > 0)
  {
    String Received = Serial.readString();
      
    //PUERTA DE SALÓN
    if (int(Received[0]) == 1) //abrir salón
    {
      digitalWrite(salon_cerrar, apagar);
      digitalWrite(salon_abrir, encender);
      delay(11000);
      digitalWrite(salon_abrir, apagar);
    }
    if (int(Received[0]) == 0 ) // cerrar salón
    {
      digitalWrite(salon_abrir, apagar);
      digitalWrite(salon_cerrar, encender);
      delay(11000);
      digitalWrite(salon_cerrar, apagar);
    }

    //CHIMENEA
    if (int(Received[0]) == 2 )
    {
      digitalWrite(chimenea, encender);
    }
    if (int(Received[0]) == 3 )
    {
      digitalWrite(chimenea, apagar);
    }

    //PUERTA DE GARAJE
    if (int(Received[0]) == 4 ) //abrir garaje
    {
      digitalWrite(garaje_cerrar, apagar);
      digitalWrite(garaje_abrir, encender);
      delay(11000);
      digitalWrite(garaje_abrir, apagar);
    }
    if (int(Received[0]) == 5 ) // cerrar garaje
    {
      digitalWrite(garaje_abrir, apagar);
      digitalWrite(garaje_cerrar, encender);
      delay(11000);
      digitalWrite(garaje_cerrar, apagar);
    }

    //FUENTE
    if (int(Received[0]) == 6)
    {
      digitalWrite(fuente, encender);
    }
    if (int(Received[0]) == 7)
    {
      digitalWrite(fuente, apagar);
    }

    //COCINA
    if (int(Received[0]) == 8)
    {
      digitalWrite(cocina, encender);
    }
    if (int(Received[0]) == 9)
    {
      digitalWrite(cocina, apagar);
    }

    //ENCIMERA
    if (int(Received[0]) == 10)
    {
      digitalWrite(encimera, encender);
    }
    if (int(Received[0]) == 11)
    {
      digitalWrite(encimera, apagar);
    }

    //FOGÓN1
    if (int(Received[0]) == 12)
    {
      digitalWrite(fogon1, encender);
    } 
    if (int(Received[0]) == 13)
    {
      digitalWrite(fogon1, apagar);
    }

    //FOGÓN2
    if (int(Received[0]) == 14)
    {
      digitalWrite(fogon2, encender);
    }
    if (int(Received[0]) == 15)
    {
      digitalWrite(fogon2, apagar);
    }

    //SALÓN
    if (int(Received[0]) == 16 )
    {
      digitalWrite(salon, encender);
    }
    if (int(Received[0]) == 17)
    {
      digitalWrite(salon, apagar);
    }

    //DORMITORIO
    if (int(Received[0]) == 18)
    {
      digitalWrite(dormitorio, encender);
    }
    if (int(Received[0]) == 19)
    {
      digitalWrite(dormitorio, apagar);
    }

    //BAÑO
    if (int(Received[0]) == 20)
    {
      digitalWrite(bano, encender);
    }
    if (int(Received[0]) == 21)
    {
      digitalWrite(bano, apagar);
    }

    //PISCINA
    if (int(Received[0]) == 22 )
    {
      digitalWrite(piscina, encender);
    }
    if (int(Received[0]) == 23)
    {
      digitalWrite(piscina, apagar);
    }

  }
  if (Serial1.available() > 0)
  {
     char Received1 = Serial1.read();
    

    //PUERTA DE SALÓN
   if (Received1 == 'A') //abrir salón
    {
      digitalWrite(salon_cerrar, apagar);
      digitalWrite(salon_abrir, encender);
      delay(11000);
      digitalWrite(salon_abrir, apagar);
    }
    if (Received1 == 'B') // cerrar salón
    {
      digitalWrite(salon_abrir, apagar);
      digitalWrite(salon_cerrar, encender);
      delay(11000);
      digitalWrite(salon_cerrar, apagar);
    }
    
 //SALÓN
    if (Received1 == 'C')
    {
      digitalWrite(salon, encender);
    }
    if (Received1 == 'D')
    {
      digitalWrite(salon, apagar);
    }
  
    //CHIMENEA
    if (Received1 == 'E')
    {
      digitalWrite(chimenea, encender);
    }
    if (Received1 == 'F')
    {
      digitalWrite(chimenea, apagar);
    }
    
      //COCINA
    if (Received1 == 'G')
    {
      digitalWrite(cocina, encender);
    }
    if (Received1 == 'H')
    {
      digitalWrite(cocina, apagar);
    }

    //ENCIMERA
    if (Received1 == 'I')
    {
      digitalWrite(encimera, encender);
    }
    if (Received1 == 'J')
    {
      digitalWrite(encimera, apagar);
    }

    //FOGÓN1
    if (Received1 == 'M')
    {
      digitalWrite(fogon1, encender);
    } 
    if (Received1 == 'N')
    {
      digitalWrite(fogon1, apagar);
    } 

    //FOGÓN2
    if (Received1 == 'K')
    {
      digitalWrite(fogon2, encender);
    }
    if (Received1 == 'L')
    {
      digitalWrite(fogon2, apagar);
    }
  

   
    //DORMITORIO
    if (Received1 == 'Q')
    {
      digitalWrite(dormitorio, encender);
    }
    if (Received1 == 'R')
    {
      digitalWrite(dormitorio, apagar);
    }
    
    //BAÑO
    if (Received1 == 'O')
    {
      digitalWrite(bano, encender);
    }
    if (Received1 == 'P')
    {
      digitalWrite(bano, apagar);
    }
   
    //PUERTA DE GARAJE
    if (Received1 == 'W' ) //abrir garaje
    {
      digitalWrite(garaje_cerrar, apagar);
      digitalWrite(garaje_abrir, encender);
      delay(11000);
      digitalWrite(garaje_abrir, apagar);
    }
    if (Received1 == 'X') // cerrar garaje
    {
      digitalWrite(garaje_abrir, apagar);
      digitalWrite(garaje_cerrar, encender);
      delay(11000);
      digitalWrite(garaje_cerrar, apagar);
    }

    //FUENTE
    if (Received1 == 'S')
    {
      digitalWrite(fuente, encender);
    }
    if (Received1 == 'T')
    {
      digitalWrite(fuente, apagar);
    }

    //PISCINA
    if (Received1 == 'U')
    {
      digitalWrite(piscina, encender);
    }
    if (Received1 == 'V')
    {
      digitalWrite(piscina, apagar);
    }

  }
}

