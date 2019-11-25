
/*=======================================================================================
 * ==============  DEFINITION DES PINS ET DES VARIABLES GLOBALES    =====================
 * ======================================================================================
 */ 

#define pinEnable 12 // Activation du driver/pilote
#define pinStep    9 // Signal de PAS (avancement)
#define pinDir     8 // Direction 
#define pinSwitch  2
bool isMotorEnabled = false;

/*=======================================================================================
 * ===========  SETUP : INITIALISATION DES VARIABLES ET DES FONCTIONS    ================
 * ======================================================================================
 */ 

void setup()
{
  Serial.begin(9600);
  Serial.println("Test DRV8825");

  // Gestion des pins driver et moteur
  pinMode( pinEnable, OUTPUT );
  pinMode( pinDir   , OUTPUT );
  pinMode( pinStep  , OUTPUT );

  digitalWrite( pinEnable, HIGH); // moteur "libre" par défaut
  digitalWrite( pinDir   , HIGH); // Direction avant
  digitalWrite( pinStep  , LOW);  // Initialisation de la broche step

  // Gestion des pins du switch
  pinMode( pinSwitch, INPUT_PULLUP);
}


/*=======================================================================================
 * ==============  LOOP : CODE PRINCIPAL ET UTILISATION DES FONCTIONS  ==================
 * ======================================================================================
 */ 

void loop(){
  
  // Avance de 200 pas lentement
  stepForward(200, 10);

  delay(1000); //on attend une seconde
  
  // Refaire 200 pas dans l'autre sens repidement
  stepBackwards(200, 1);
  
  // Pas de step et pas d'ordre... 
  // l'axe du moteur est donc bloqué 
  Serial.println("Axe bloqué + attendre 5 sec");
  delay( 5000 );
  
  // déblocage de l'axe moteur
  Serial.println("Deblocage axe");
  disableMotor();
  
  // Fin et blocage du programme
  // Presser reset pour recommander
  Serial.println("Fin de programme Moteur");
  
  delay(1000);
  Serial.println("Attente de consigne 'Serial'...");
  while( true )
  {
    if(readSerialInput() == 1)
    {
      Serial.println("J'ai reçu la consigne 1");
    }
    else if(readSerialInput() == 2)
    {
      Serial.println("J'ai reçu la consigne 2");
    }
    else if(readSerialInput() == 3)
    {
      Serial.println("J'ai reçu la consigne 3");
    }
  }; // boulce infinie qui "bloque le programme"
}


/*=======================================================================================
 * ======================  FONCTIONS DU PROJET    =======================================
 * ======================================================================================
 */ 

void stepForward(int numberOfSteps, int delayBetweenSteps)
{
  if(!isMotorEnabled)
  {
    enableMotor();
  }
  
  digitalWrite( pinDir   , LOW); // Direction avant
  delay(5); // laisser le temps au driver de changer la direction
  
  for(int i=0; i<numberOfSteps; i++){
    makeOneStep(delayBetweenSteps);
  } 
}

void stepBackwards(int numberOfSteps, int delayBetweenSteps)
{
  if(!isMotorEnabled)
  {
    enableMotor();
  }
  digitalWrite( pinDir   , HIGH); // Direction inversée
  delay(5); // laisser le temps au driver de changer la direction
  
  for(int i=0; i<numberOfSteps; i++)
  {
    makeOneStep(delayBetweenSteps);
  } 
}

void makeOneStep(int delayBetweenSteps)
{
    digitalWrite( pinStep, HIGH );
    delay(delayBetweenSteps);
    digitalWrite( pinStep, LOW );
    delay(delayBetweenSteps);
}

bool enableMotor()
{
  digitalWrite( pinEnable, LOW );
  isMotorEnabled = true; 
}

bool disableMotor()
{
  digitalWrite( pinEnable, HIGH );
  isMotorEnabled = false; 
}

bool isSwitchClosed(int switchPin)
{
  return digitalRead(switchPin) == LOW;
}

int readSerialInput()
{
  if(Serial.available() > 0)
  {
    char b = Serial.read();

    if(b == '1')
    {
      return 1;
    }
    else if(b == '2')
    {
      return 2;
    }
    else if(b == '3')
    {
      return 3;
    }
    else
    {
      return -1;
    }
  }
  else
  {
    return -1;
  }
}
