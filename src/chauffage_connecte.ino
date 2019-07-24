#define BLYNK_PRINT Serial
#include <Servo.h>

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>


//char auth[] = "c3582923b35d4e619ee1d8c914184f0f";//Token Iphone Gust
char auth[] = "28296c3f45cc461aaf20be18d7d75cd3";//Token Android Amadou

char ssid[] = "Augustin_jr";
char pass[] = "MotdePass";


#define EspSerial Serial1


#define ESP8266_BAUD 115200
int autom=0;
static int positionMin = 0;
static int positionMax=180;
ESP8266 wifi(&EspSerial);

BlynkTimer timer;
Servo servo;

BLYNK_WRITE(V7)
{
  //si le mode autonome est activé
  if(autom==1){
  servo.write(param.asInt());
  }
}
//Activation du mode autonome depuis l'appli mobile
BLYNK_WRITE(V1)
{
  autom=param.asInt();
}


void myTimerEvent()
{
  float voltage= analogRead(A0);
   // Convertion de la lecture en tension
  float tension=voltage*5.0;
  tension/=1024.0;
   // Convertir la tension (mv) en temperature
  float temperature =((tension * 1000.0)-500)/10;
  //Ecriture de la temperature dans notre application 
  Blynk.virtualWrite(V5,temperature);
  //si le mode autonome est desactivé et que la temperature est superieur a 20deg
     if(temperature>20 and autom==0)
    {
      Serial.println(positionMin);
      if(positionMin<=positionMax)
      {
        //On tourne le bouton du chauffage et on attend 10 seconde pour voir si la temperature a baissée
          positionMin+=30;
          servo.write(positionMin);                  
          delay(10000);
      }
       
    }
}

void setup()
{
  servo.attach(7);
  // Debug console
  Serial.begin(115200);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);

  // Appel de la fonction toutes les secondes
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initialisation BlynkTimer
}
