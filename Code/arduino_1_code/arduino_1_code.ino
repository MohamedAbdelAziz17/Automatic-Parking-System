/*
 * this arduino reads the card using RFID,
 * then send the number of card to arduino 2,   
*/

#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above


uint8_t rxBuffer_uart = 0;


LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

uint8_t id = 0;        /*Data of each card*/

bool flag_user0 = false;
bool flag_user1 = false;
bool flag_user2 = false;
bool flag_user3 = false;

void setup() {
  /*  UART init */
  Serial.begin(9600);
  
  /*  lcd i2c init   */
    lcd.init();                     
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print(" Auto  Parking");
    lcd.setCursor(5,1);
    lcd.print("System");
  /*  RFID  init  */
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                           // Init MFRC522 card

    delay(5000);
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Welcome");
    delay(2000);
    lcd.clear();
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  /***** Get card ID****/
   //Serial.print(mfrc522.uid.uidByte[0],DEC);
    id = mfrc522.uid.uidByte[0];  // read first number in CARD ID
  /*********************/
  
  /****Write Logic Code****/

  //send this data of card to arduino_2 using UART
    Serial.write(id);
    
  /*****write anything on LCD here*****/
  if(id == 89 && flag_user0 == false)
  {
    lcd.clear();
    lcd.print(" Car is parking");
    lcd.setCursor(0,1);
    lcd.print("happy shopping:)");
    flag_user0 = true;
  }
  else if(id == 89 && flag_user0 == true)
  {
    lcd.clear();
    lcd.print("welcome back :)");
    lcd.setCursor(0,1);
    lcd.print("you car ontheway");
    flag_user0 = false;
  }

  if(id == 3 && flag_user1 == false)
  {
    lcd.clear();
    lcd.print(" Car is parking");
    lcd.setCursor(0,1);
    lcd.print("happy shopping:)");
    flag_user1 = true;
  }
  else if(id == 3 && flag_user1 == true)
  {
    lcd.clear();
    lcd.print("welcome back :)");
    lcd.setCursor(0,1);
    lcd.print("you car ontheway");
    flag_user1 = false;
  }
  if(id == 131 && flag_user2 == false)
  {
    lcd.clear();
    lcd.print(" Car is parking");
    lcd.setCursor(0,1);
    lcd.print("happy shopping:)");
    flag_user2 = true;
  }
  else if(id == 131 && flag_user2 == true)
  {
    lcd.clear();
    lcd.print("welcome back :)");
    lcd.setCursor(0,1);
    lcd.print("you car ontheway");
    flag_user2 = false;
  }
  if(id == 57 && flag_user3 == false)
  {
    lcd.clear();
    lcd.print(" Car is parking");
    lcd.setCursor(0,1);
    lcd.print("happy shopping:)");
    flag_user3 = true;
  }
  else if(id == 57 && flag_user3 == true)
  {
    lcd.clear();
    lcd.print("welcome back :)");
    lcd.setCursor(0,1);
    lcd.print("you car ontheway");
    flag_user3 = false;
  }
  
  

  
  /************************************/
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}









/*
  if(Serial.available() > 0)
  {
    rxBuffer_uart = Serial.read();
    
    if(rxBuffer_uart == '1')
    {
      
    }
    
    if(rxBuffer_uart == '0')
    { 

    }

     if(rxBuffer_uart == '2')
    {
      
    }
    
  }
  */








/*
  if(Serial.available() > 0)
  {
    rxBuffer_uart = Serial.read();

    if(rxBuffer_uart == '1')
    {
      Serial.println("data_sended ('1')");
      radio.write(&rxBuffer_uart, sizeof(uint8_t));
    }

    else if(rxBuffer_uart == '0')
    {
       Serial.println("data_sended ('0')");
      radio.write(&rxBuffer_uart, sizeof(uint8_t));
    }
    }
*/
