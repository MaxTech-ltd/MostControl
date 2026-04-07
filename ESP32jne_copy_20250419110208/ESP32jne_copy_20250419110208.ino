#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

const char* ssid = "Shadow";
const char* password = "41968096";

const char* botToken = "7748537788:AAGcm8kG79DPWloQALM3fMct7xWcHRz8090"; // ← вставь токен
const String chatId = "7302470355"; // ← вставь chat ID

WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

unsigned long lastTimeBotRan;
const int botRequestDelay = 2000;

void setup() {
  Serial.begin(9600); // Отправка на Arduino
  Serial2.begin(9600); // Если хочешь, можно использовать TX2 (GPIO17), но ты выбрал TX0 (через Serial)

  WiFi.begin(ssid, password);
  Serial.println("Подключение к Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi подключен");
  client.setInsecure(); // Разрешаем работу без проверки SSL

  Serial.println("ESP32 готов. Ожидание команд в Telegram...");
}

void loop() {
  if (millis() - lastTimeBotRan > botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      for (int i = 0; i < numNewMessages; i++) {
        String text = bot.messages[i].text;
        String fromName = bot.messages[i].from_name;

        Serial.println("Команда от Telegram: " + text);

        if (text == "/barrier") {
          Serial.println("Отправка команды Arduino: " + text);
          Serial.print(text);
          Serial.print("\n");  // обязательно новая строка
        }
        if (text == "/critical_light") {
          Serial.println("Отправка команды Arduino: " + text);
          Serial.print(text);
          Serial.print("\n");  // обязательно новая строка
        }
        if (text == "/take_data") {
          Serial.println("Запрос данных от Arduino.");
          Serial.println("/get_data"); // Запрос данных от Arduino
        }

        bot.sendMessage(chatId, "Команда получена: " + text, "");
      }

      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    message.trim();

    if (message == "предкритический уровень") {
      String warning = "⚠️ ВНИМАНИЕ: Предкритический уровень на мосту!";
      bot.sendMessage(chatId, warning, "");
      Serial.println("Сообщение отправлено в Telegram.");
    }
    if (message == "критический уровень") {
      String warning = "🚨 ВНИМАНИЕ: Авария на мосту уровень опасности критический!";
      bot.sendMessage(chatId, warning, "");
      Serial.println("Сообщение отправлено в Telegram.");
    }
    if (message.startsWith("Distance:")) {
      delay(1000);
      bot.sendMessage(chatId, "Distance: " + message, "");
      delay(1000);  // Задержка 1 секунда
    } 
    else if (message.startsWith("Soil Moisture:")) {
      bot.sendMessage(chatId, "Soil Moisture: " + message, "");
      delay(1000);  // Задержка 1 секунда
    }
    else if (message.startsWith("Acceleration X:")) {
      bot.sendMessage(chatId, "Acceleration X: " + message, "");
      delay(1000);  // Задержка 1 секунда
    }
    else if (message.startsWith("Acceleration Y:")) {
      bot.sendMessage(chatId, "Acceleration Y: " + message, "");
      delay(1000);  // Задержка 1 секунда
    }
    else if (message.startsWith("Acceleration Z:")) {
      bot.sendMessage(chatId, "Acceleration Z: " + message, "");
      delay(1000);  // Задержка 1 секунда
    }
    else if (message.startsWith("Flex Resistance:")) {
      bot.sendMessage(chatId, "Flex Resistance: " + message, "");
      delay(1000);  // Задержка 1 секунда
    }
    else if (message.startsWith("Weight:")) {
      bot.sendMessage(chatId, "Weight: " + message, "");
      delay(1000);  // Задержка 1 секунда
    }
    else if (message.startsWith("Force:")) {
      bot.sendMessage(chatId, "Force: " + message, "");
      delay(1000);  // Задержка 1 секунда
    }
  }
}
