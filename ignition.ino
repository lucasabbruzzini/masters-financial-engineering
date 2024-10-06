                     
                                /* 29/01
                                   Lançamento do foguete, técnica para início da queima
                                   Autores: Aviônica-EPTA 2018-1
                                */ 

                    // Variáveis
                    
                    const int K = 4;                          // número de leds/estados/botões -> definido como 4
                    int LED[K];                               // pinos dos leds
                    int BOT[K];                               // pino dos botões
                    int RES = 30;                             // pino para reset
                    bool DGR[K+1];                            // DigitalRead(pino)
                    bool REF[K+1];                            // Auxiliar de tempo lógico
                    bool RFW[K];                              // Permite o LIGA() e o ACIONA()
                    const int BUZ = 3;                        // Buzzer
                    const int REL = 2;                        // Relé
                    int i;                                    // Iterador
                    unsigned long CONTA[K+1];                 // Auxiliar de tempo numérico- [ms]
                    unsigned long MIN = 300;                  // Tempo mínimo que deve ser pressionado - [ms]
                    unsigned long dt = 15000;                 // intervalo entre cada recomeço [ms]
                    unsigned long RESTART = dt;               // Tempo de recomeço - [ms]
                    unsigned long inf = 250;                  // Intervalo entre a informação- [ms]
                    unsigned long TEMPO;                      // Recebe o tempo atual- [ms]
                  
                    // Funções
                  
                    void TESTA() ,LIGA() ,ACIONA() ,INFORMA(), RESETA();
                  
                    void setup() 
                    { 
                      for(i=0;i<4;i++)
                      {
                        LED[i] = 7 - i;                       // 7 a 4
                        pinMode(LED[i],OUTPUT);
                        digitalWrite(LED[i],LOW);
                        BOT[i] = 22 + 2*i;                    // 22 a 28
                        pinMode(BOT[i],INPUT_PULLUP);         // Resistores de pullup internos ativados -> normalmente alto
                        REF[i] = false;
                        RFW[i] = false;
                        CONTA[i] = 0;
                      }
                      REF[K] = false;
                      pinMode(RES,INPUT_PULLUP);
                      pinMode(BUZ,OUTPUT);
                      pinMode(REL,OUTPUT);
                      noTone(BUZ);
                      digitalWrite(REL,LOW);
                      Serial.begin(9600);
                      delay(2000);
                    }
                    
                    void loop() 
                    {
                      TEMPO = millis();
                      TESTA();
                      LIGA();
                      ACIONA();
                      INFORMA();
                      if(TEMPO > RESTART)
                      {
                        RESETA();
                      }
                    }
                  
                    void TESTA()
                    {
                      for(i=0;i<3;i++)
                      {
                        DGR[i] = digitalRead(BOT[i]); // precisa entrar no loop
                        if(DGR[i] == HIGH)
                        {
                          REF[i] = false;
                        }
                        if((DGR[i] == LOW) && (REF[i] == false))
                        {
                          REF[i] = true;
                          CONTA[i] = millis();
                        }
                        if ((REF[i] == true) && (TEMPO > (CONTA[i] + MIN)))
                        {
                          RFW[i] = true;
                        }
                      }
                      i = 3; // último botão
                      DGR[i] = digitalRead(BOT[i]);
                        if(DGR[i] == HIGH)
                        {
                          REF[i] = false;
                        }
                        if((DGR[i] == LOW) && (REF[i] == false) && (RFW[0] == true) && (RFW[1] == true) && (RFW[2] == true))
                        {
                          REF[i] = true;
                          CONTA[i] = millis();
                        }
                        if ((REF[i] == true) && (TEMPO > (CONTA[i] + MIN)))
                        {
                          RFW[i] = true;
                        }
                       DGR[K] = digitalRead(RES); // botão de reset
                        if(DGR[K] == HIGH)
                        {
                          REF[K] = false;
                        }
                        if((DGR[K] == LOW) && (REF[K] == false))
                        {
                          REF[K] = true;
                          CONTA[K] = millis();
                        }
                        if ((REF[K] == true) && (TEMPO > (CONTA[K] + MIN)))
                        {
                          RESETA();
                        }
                    }
                  
                    void LIGA()
                    {
                      for(i=0;i<3;i++) // LEDs de 0 a 2
                      {
                       if (RFW[i] == true)
                       {
                         digitalWrite(LED[i], HIGH);
                       }
                      }
                    }
                  
                    void ACIONA()
                    {
                      i = 3; // LED final, buzzer e acionamento
                      if (RFW[i] == true)
                      {
                        digitalWrite(LED[i], HIGH);
                        tone(BUZ, 200);
                        digitalWrite(REL, HIGH);
                        RESTART = 9999999;  // evita que o sistema reinicie
                      }
                    }
                  
                    void INFORMA()
                    {
                      if(TEMPO % inf == 0)
                      {
                        Serial.print(TEMPO);
                        Serial.print("\t");
                        for(i=0;i<4;i++)
                        {
                          Serial.print(DGR[i]);
                          Serial.print(digitalRead(LED[i]));
                          Serial.print(" ");
                        }
                        Serial.print(DGR[K]);
                        Serial.print(" ");
                        if(digitalRead(LED[3]) == HIGH) // LEITURA DO LED 3 determina se o sistema está ativo ou não
                        {
                          Serial.println("\tACIONADO");
                        } else
                        {
                          Serial.println("\tDESLIGADO");
                        }
                      }
                    }
                  
                  void RESETA()
                  {
                    for(i=0;i<4;i++)
                      {
                        digitalWrite(LED[i],LOW);
                        RFW[i] = false;
                      }
                    REF[K] = false;
                    noTone(BUZ);
                    digitalWrite(REL,LOW);
                    RESTART = TEMPO + dt;
                    Serial.println("\t\tRESETADO");
                  }










  
