#include <EEPROM.h>
#include "DHT.h"

const int pino_dht = 9;
DHT dht(pino_dht, DHT11);

// Arrays para armazenar as 10 amostras
int tempLeituras[10];
int umidLeituras[10];
int indice = 0; // índice atual de leitura

// Endereço inicial da EEPROM para armazenar os dados
int enderecoEEPROM = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

// Simula leitura de temperatura (int)
int leituraSensorTemperatura() {
  return (int)dht.readTemperature();
}

// Simula leitura de umidade (int)
int leituraSensorUmidade() {
  return (int)dht.readHumidity();
}

void loop() {
  delay(2000); // 2 segundos entre amostras

  int temp = leituraSensorTemperatura();
  int umid = leituraSensorUmidade();

  // Verifica se a leitura é válida
  if (isnan(temp) || isnan(umid)) {
    Serial.println("Falha na leitura do sensor.");
    return;
  }

  // Armazena as leituras no array
  tempLeituras[indice] = temp;
  umidLeituras[indice] = umid;
  indice++;

  // Quando atingir 10 amostras, processa
  if (indice == 10) {
    int somaTemp = 0, somaUmid = 0;
    int maxTemp = tempLeituras[0], minTemp = tempLeituras[0];
    int maxUmid = umidLeituras[0], minUmid = umidLeituras[0];

    for (int i = 0; i < 10; i++) {
      somaTemp += tempLeituras[i];
      somaUmid += umidLeituras[i];

      if (tempLeituras[i] > maxTemp) maxTemp = tempLeituras[i];
      if (tempLeituras[i] < minTemp) minTemp = tempLeituras[i];
      if (umidLeituras[i] > maxUmid) maxUmid = umidLeituras[i];
      if (umidLeituras[i] < minUmid) minUmid = umidLeituras[i];
    }

    int mediaTemp = somaTemp / 10;
    int mediaUmid = somaUmid / 10;

    // Grava na EEPROM: temperatura (média, min, max), umidade (média, min, max)
    EEPROM.update(enderecoEEPROM++, mediaTemp);
    EEPROM.update(enderecoEEPROM++, minTemp);
    EEPROM.update(enderecoEEPROM++, maxTemp);
    EEPROM.update(enderecoEEPROM++, mediaUmid);
    EEPROM.update(enderecoEEPROM++, minUmid);
    EEPROM.update(enderecoEEPROM++, maxUmid);

    Serial.println("Dados processados e armazenados:");
    Serial.print("Temperatura - Média: "); Serial.print(mediaTemp);
    Serial.print(", Mín: "); Serial.print(minTemp);
    Serial.print(", Máx: "); Serial.println(maxTemp);

    Serial.print("Umidade - Média: "); Serial.print(mediaUmid);
    Serial.print(", Mín: "); Serial.print(minUmid);
    Serial.print(", Máx: "); Serial.println(maxUmid);

    // Reinicia o índice
    indice = 0;
  }
}
