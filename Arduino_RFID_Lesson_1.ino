#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//*****************************************************************************************//
void setup() 
{
  Serial.begin(9600);                                           // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  // Serial.println(F("Поднесите карту, чтобы прочесть UID и персональные данные с карты:"));    //shows in serial that it is ready to read
}

//*****************************************************************************************//
void loop() 
{
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) 
  key.keyByte[i] = 0xFF;

  //переменные, которые нам нужны
  byte block;
  byte len;
  
  String uidStr = "";
  String nameStr = "";
  String fNameStr = "";
  MFRC522::StatusCode status;
  //-------------------------------------------
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }



  
  //-------------------------------------------Здесь мы считываем UID нашей карты
for ( uint8_t i = 0; i < 4; i++) 
  {
    uidStr +=(String(mfrc522.uid.uidByte[i], HEX)); // Сложить эти 4 байта в одну переменную String
  }
  //-------------------------------------------




  byte buffer1[18];

  block = 4 ;
  len = 18;
  //------------------------------------------- GET FIRST NAME
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Ошибка аутентификации: ");  // Сюда написать код ошибки, который будет отправлен в Serial
    return;
  }
  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Ошибка чтения: "); // Сюда написать код ошибки, который будет отправлен в Serial. Это критическая ошибка, необходима переинициализация
    return;
  }
  //PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      fNameStr += (char)buffer1[i];
    }
  }
  //---------------------------------------- GET LAST NAME
  byte buffer2[18];
  block = 1;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Ошибка аутентификации: ");  // Сюда написать код ошибки, который будет отправлен в Serial
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Ошибка чтения: "); // Сюда написать код ошибки, который будет отправлен в Serial. Это критическая ошибка, необходима переинициализация
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  //PRINT LAST NAME
  for (uint8_t i = 0; i < 16; i++) 
  {
    nameStr += (char)buffer2[i];;
  }
  //----------------------------------------



  Serial.print(uidStr);
  //Serial.print(nameStr);
  //Serial.print(fNameStr);



  delay(1000); // Задержка перед тем, как мы след. раз опросим карту
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
