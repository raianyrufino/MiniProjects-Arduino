int led = 9; // pino onde o LED vai estar ligado 
int brilho = 0; // variável que armazena a quatidade de brilho do LED. 
int fade = 5; // determina a variação entre cada degradê que o LED será acesso 

void setup() {   
// configura o pino 9 como saída digital   
pinMode(led, OUTPUT); 
}  

void loop() {   
// Envia um PWM proporcional ao valor do brilho para o LED ligado ao pino 9:   
analogWrite(led, brilho);   
// Altera o brilho para no próximo loop alterar o brilho do LED  
brilho = brilho + fade;   
// Inverte a direção do Fade caso ultrapasse os limites da escala do analogWrite()   
if (brilho == 0 || brilho == 255) {     
fade = -fade ;    
}        
// Espera 30 milisegundos para controlar e dar o efeito de fade   
delay(70); 

}

