int frequencia = 0;
int Pinofalante = 10;
int tempo = 0;

void setup()
{
  pinMode(Pinofalante,OUTPUT); //PINO DO BUZZER
}

void loop()
{
  for (frequencia = 150; frequencia < 1800; frequencia += 1) 
  {
    tone(Pinofalante, frequencia, tempo); 
    delay(1);
  }
  for (frequencia = 1800; frequencia > 150; frequencia -= 1) 
  {
    tone(Pinofalante, frequencia, tempo); 
    delay(1);
  }

}

