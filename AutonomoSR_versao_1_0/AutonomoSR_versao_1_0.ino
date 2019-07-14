#include <AFMotor.h>

/******************************************************************************************************/
  // inclusao de bibliotecas.    
  #include <Servo.h>    // inclui biblioteca de manipulacao de servos motores.    
  #include <AFMotor.h>   // inclui biblioteca de manipulacao de motores DCs.  
    
  //Definindo os pinos  
  #define trigPin A0 //Pino TRIG do sensor no pino analogico A0
  #define echoPin A1 //Pino ECHO do sensor no pino analogico A1
  #define BUZZER A2  // Define o pino do buzzer (Som) no pino ANALALOGICO A0  
  AF_DCMotor motor1(1);    // Define o motor1 ligado ao M1  
  AF_DCMotor motor2(2);    // Define o motor2 ligado ao M2  

  int TempoGirar = 1;//esse Ã© o tempo para o robÃ´ girar em 45Âº com uma bateria de 9v.
  int distanciaObstaculo = 30; //distÃ¢ncia para o robÃ´ parar e recalcular o melhor caminho
  int velocidadeMotores = 80; // velocidade que os motores funcionarÃ£o na bateria 9v. Para a bateria 9v a velocidade 80 Ã© ideal
  Servo servo_ultra_sonico; // nomeando o servo motor    
  //variÃ¡veis  para o sensor ultrassonico
  long duracao;
  long distancia_cm=0;
  int minimumRange=5; //tempo de resposta do sensor
  int maximumRange=200;
    
  // executado na inicializaÃ§Ã£o do Arduino    
  void setup(){    
    Serial.begin(9600); // inicializa a comunicaÃ§Ã£o serial para mostrar dados     
    servo_ultra_sonico.attach(10);  // Define o mini servo motor ligado no pino digital 10.    
    pinMode(trigPin, OUTPUT); //define o pino TRIG como saÃ­da
    pinMode(echoPin, INPUT);  //define o pino ECHO como entrada 
    pinMode(BUZZER,OUTPUT);   // Define o pino do buzzer como saÃ­da   
    motor1.setSpeed(velocidadeMotores);     // Define a velocidade para os motores. A velocidade mÃ¡xima Ã© 255. 
    motor2.setSpeed(velocidadeMotores);     //Usamos uma bateria de 9v 450mAh, com ela a velocidade ficou boa. Mas dependendo da bateria utilizada a velocidade deverÃ¡ ser utilizada. NÃ£o use pilhas, pois sÃ£o fracas
    servo_ultra_sonico.write(90);   // O servo do sensor se inicia a 90 graus (meio)    
    rotacao_Parado;  //inica com os motores parados     
  }    
    
  // FunÃ§Ã£o principal do Arduino    
  void loop(){    
    pensar(); //inicia a funÃ§Ã£o pensar  
  }    
    
  // FunÃ§Ã£o para chamar outras funÃ§Ãµes e definir o que o robÃ´ farÃ¡  
  void pensar(){    
   reposicionaServoSonar(); //Coloca o servo para olhar a frente    
   int distancia = lerSonar(); // Ler o sensor de distÃ¢ncia  
   Serial.print("distancia em cm: "); 
   Serial.println(distancia);   // Exibe no serial monitor 
   if (distancia > distanciaObstaculo) {  // Se a distÃ¢ncia for maior que 20 cm  
     rotacao_Frente(); //robÃ´ anda para frente   
   }else{   
     rotacao_Parado();  //para o robÃ´  
     posicionaCarroMelhorCaminho(); //calcula o melhor caminho    
     pensar();    
   }   
  }  
    
  // FunÃ§Ã£o para ler e calcular a distÃ¢ncia do sensor ultrassÃ´nico    
  int lerSonar(){    
   digitalWrite(trigPin, LOW); //nÃ£o envia som
   delayMicroseconds(2);
   digitalWrite(trigPin,HIGH); //envia som 
   delayMicroseconds(10);
   digitalWrite(trigPin,LOW); //nÃ£o envia o som e espera o retorno do som enviado
   duracao = pulseIn(echoPin,HIGH); //Captura a duraÃ§Ã£o em tempo do retorno do som.
   distancia_cm = duracao/56; //Calcula a distÃ¢ncia
   delay(30);  
   return distancia_cm;             // Retorna a distÃ¢ncia  
  }   
    
  // FunÃ§Ã£o para calcular a distÃ¢ncia do centro    
  int calcularDistanciaCentro(){    
   servo_ultra_sonico.write(90);    
   delay(20);   
   int leituraDoSonar = lerSonar();  // Ler sensor de distÃ¢ncia  
   delay(500);   
   leituraDoSonar = lerSonar();   
   delay(500);   
   Serial.print("Distancia do Centro: "); // Exibe no serial  
   Serial.println(leituraDoSonar);   
   return leituraDoSonar;       // Retorna a distÃ¢ncia  
  }    
    
  // FunÃ§Ã£o para calcular a distÃ¢ncia da direita    
  int calcularDistanciaDireita(){    
   servo_ultra_sonico.write(0);   
   delay(200);  
   int leituraDoSonar = lerSonar();   
   delay(500);   
   leituraDoSonar = lerSonar();   
   delay(500);   
   Serial.print("Distancia da Direita: ");  
   Serial.println(leituraDoSonar);   
   return leituraDoSonar;    
  }    
    
  // FunÃ§Ã£o para calcular a distÃ¢ncia da esquerda    
  int calcularDistanciaEsquerda(){    
   servo_ultra_sonico.write(180);   
   delay(200);  
   int leituraDoSonar = lerSonar();   
   delay(500);   
   leituraDoSonar = lerSonar();   
   delay(500);   
   Serial.print("Distancia Esquerda: ");  
   Serial.println(leituraDoSonar);   
   return leituraDoSonar;    
  }    
    
  // FunÃ§Ã£o para captar as distÃ¢ncias lidas e calcular a melhor distÃ¢ncia.   
  char calculaMelhorDistancia(){    
   int esquerda = calcularDistanciaEsquerda();    
   int centro = calcularDistanciaCentro();    
   int direita = calcularDistanciaDireita();    
   reposicionaServoSonar();    
   int maiorDistancia = 0;   
   char melhorDistancia = '0';     
     
   if (centro > direita && centro > esquerda){    
     melhorDistancia = 'c';    
     maiorDistancia = centro;    
   }else   
   if (direita > centro && direita > esquerda){    
     melhorDistancia = 'd';    
     maiorDistancia = direita;    
   }else  
   if (esquerda > centro && esquerda > direita){    
     melhorDistancia = 'e';    
     maiorDistancia = esquerda;    
   }    
   if (maiorDistancia <= distanciaObstaculo) { //distÃ¢ncia limite para parar o robÃ´   
     rotacao_Re();    
     posicionaCarroMelhorCaminho();    
   }    
   reposicionaServoSonar();  
   return melhorDistancia;    
  }    
    
  // FunÃ§Ã£o para colocar o carrinho na melhor distÃ¢ncia, isto Ã©, girÃ¡-lo para a melhor distÃ¢ncia    
  void posicionaCarroMelhorCaminho(){    
   char melhorDist = calculaMelhorDistancia();     
   Serial.print("melhor Distancia em cm: ");  
   Serial.println(melhorDist);  
   if (melhorDist == 'c'){    
     pensar();    
   }else if (melhorDist == 'd'){    
     rotacao_Direita();    
   }else if (melhorDist == 'e'){    
     rotacao_Esquerda();     
   }else{    
     rotacao_Re();    
   }    
   reposicionaServoSonar();    
  }    
    
  // FunÃ§Ã£o para deixar o sensor "olho" do robÃ´ no centro    
  void reposicionaServoSonar(){    
   servo_ultra_sonico.write(90);   
   delay(200);   
  }    
    
  // FunÃ§Ã£o para fazer o carro parar    
  void rotacao_Parado()    
  {    
   Serial.println(" Motor: Parar ");
   motor1.run(RELEASE); // Motor para  
   motor2.run(RELEASE);  
  }    
    
  // FunÃ§Ã£o para fazer o robÃ´ andar para frente    
  void rotacao_Frente()    
  {    
   Serial.println("Motor: Frente ");   
   motor1.run(FORWARD); // Roda vai para frente  
   motor2.run(FORWARD);   
   delay(50);    
  }    
    
  // FunÃ§Ã£o que faz o robÃ´ andar para trÃ¡s e emite som quando ele dÃ¡ rÃ©    
  void rotacao_Re()    
  {    
   Serial.println("Motor: rÃ© ");  
   for (int i=0; i <= 3; i++){
      digitalWrite(BUZZER, HIGH); // Liga o som
      delay(100);
      motor1.run(BACKWARD);    // Roda vai para trÃ¡s  
      motor2.run(BACKWARD);    // Roda vai para trÃ¡s  
      delay(100);  
      digitalWrite(BUZZER, LOW); // Desliga o som  
      delay(100);
   } 
   rotacao_Parado();    
  }    
    
  // FunÃ§Ã£o que faz o robÃ´ virar Ã  direita,
  void rotacao_Direita()    
  {    
   digitalWrite(BUZZER, HIGH); // Liga o som
   delay(100);
   motor1.run(BACKWARD);    //o robÃ´ dÃ¡ uma rÃ© para nÃ£o colidir ao girar 
   motor2.run(BACKWARD);      
   delay(50);  
   digitalWrite(BUZZER, LOW); // Desliga o som  
   delay(100);
   Serial.println(" Para a direita ");  
   motor1.run(FORWARD); // Roda vai para frente  
   motor2.run(BACKWARD); // Roda vai para trÃ¡s   
   delay(TempoGirar);    
  }    
    
  // FunÃ§Ã£o que faz o robÃ´ virar Ã  esquerda    
  void rotacao_Esquerda()    
  {    
   digitalWrite(BUZZER, HIGH); // Liga o som
   delay(100);
   motor1.run(BACKWARD);    // // O robÃ´ dÃ¡ uma rÃ© para nÃ£o colidir ao girar 
   motor2.run(BACKWARD);   
   delay(50);  
   digitalWrite(BUZZER, LOW); // Desliga o som  
   delay(100);
   Serial.println(" Para a esquerda ");  
   motor1.run(BACKWARD); // Roda vai para trÃ¡s  
   motor2.run(FORWARD); // Roda vai para frente  
   delay(TempoGirar);    
  }  
