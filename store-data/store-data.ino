#include <SPI.h>
#include <MFRC522.h>

const uint8_t RST_PIN = D3;
const uint8_t SS_PIN = D4;
//--------------------------------------------------
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;        
//--------------------------------------------------
int blockNum = 4;
byte block_data[16];
/* Create array to read data from Block */
/* Length of buffer should be 4 Bytes 
more than the size of Block (16 Bytes) */
byte bufferLen = 18;
byte readBlockData[18];
//--------------------------------------------------
MFRC522::StatusCode status;
//--------------------------------------------------



void setup() 
{
  //------------------------------------------------------
  //Initialize serial communications with PC
  Serial.begin(9600);
  //------------------------------------------------------
  //Initialize SPI bus
  SPI.begin();
  //------------------------------------------------------
  //Initialize MFRC522 Module
  mfrc522.PCD_Init();
  Serial.println("Scan a MIFARE 1K Tag to write data...");
  //------------------------------------------------------
}


void loop()
{
  //------------------------------------------------------------------------------
  /* Prepare the ksy for authentication */
  /* All keys are set to FFFFFFFFFFFFh at chip delivery from the factory */
  for (byte i = 0; i < 6; i++){
    key.keyByte[i] = 0xFF;
  }
  //------------------------------------------------------------------------------
  /* Look for new cards */
  /* Reset the loop if no new card is present on RC522 Reader */
  if ( ! mfrc522.PICC_IsNewCardPresent()){return;}
  //------------------------------------------------------------------------------
  /* Select one of the cards */
  if ( ! mfrc522.PICC_ReadCardSerial()) {return;}
  //------------------------------------------------------------------------------
  Serial.print("\n");
  Serial.println("**Card Detected**");
  /* Print UID of the Card */
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++){
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.print("\n");
  /* Print type of card (for example, MIFARE 1K) */
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
    
  //------------------------------------------------------------------------------
  
  blockNum = 4;
  toBlockDataArray("21001001117"); //Student ID
  WriteDataToBlock(blockNum, block_data);
  ReadDataFromBlock(blockNum, readBlockData);
  dumpSerial(blockNum, readBlockData);


  blockNum = 5;
  toBlockDataArray("sunny"); //First Name
  WriteDataToBlock(blockNum, block_data);
  ReadDataFromBlock(blockNum, readBlockData);
  dumpSerial(blockNum, readBlockData);

  
  blockNum = 6;
  toBlockDataArray("yadav"); //Last Name
  WriteDataToBlock(blockNum, block_data);
  ReadDataFromBlock(blockNum, readBlockData);
  dumpSerial(blockNum, readBlockData);


  blockNum = 8;
  toBlockDataArray("uncle ji"); //Father's Name
  WriteDataToBlock(blockNum, block_data);
  ReadDataFromBlock(blockNum, readBlockData);
  dumpSerial(blockNum, readBlockData);


  blockNum = 9;
  toBlockDataArray("29-05-2003"); //Date of Birth
  WriteDataToBlock(blockNum, block_data);
  ReadDataFromBlock(blockNum, readBlockData);
  dumpSerial(blockNum, readBlockData);


  blockNum = 10;
  toBlockDataArray("+91 9599999955"); //Phone Number
  WriteDataToBlock(blockNum, block_data);
  ReadDataFromBlock(blockNum, readBlockData);
  dumpSerial(blockNum, readBlockData);
}



void WriteDataToBlock(int blockNum, byte blockData[]) 
{
   Serial.print("Writing data on block ");
   Serial.print(blockNum);
  //------------------------------------------------------------------------------
  /* Authenticating the desired data block for write access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK){
    Serial.print("Authentication failed for Write: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  //------------------------------------------------------------------------------
  else {
    Serial.println("Authentication success");
  }
  //------------------------------------------------------------------------------
  /* Write data to the block */
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Writing to Block failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {Serial.println("Data was written into Block successfully");}
  //------------------------------------------------------------------------------
}





/****************************************************************************************************
 * ReadDataFromBlock() function
 ****************************************************************************************************/
void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{
   Serial.print("Reading data from block ");
   Serial.println(blockNum);
  //------------------------------------------------------------------------------
  /* Authenticating the desired data block for Read access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  //------------------------------------------------------------------------------
  if (status != MFRC522::STATUS_OK){
   Serial.print("Authentication failed for Read: ");
   Serial.println(mfrc522.GetStatusCodeName(status));
   return;
  }
  else {
    Serial.println("Authentication success");
  }
  //------------------------------------------------------------------------------
  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK){
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else {
    Serial.println("Block was read successfully");  
  }
}



void dumpSerial(int blockNum, byte blockData[]) 
{
   Serial.print("\n");
   Serial.print("Data in Block:");
   Serial.print(blockNum);
   Serial.print(" --> ");
   for (int j=0 ; j<16 ; j++){
     Serial.write(readBlockData[j]);
   }
   Serial.print("\n");Serial.print("\n");
}


void toBlockDataArray(String str) 
{
  byte len = str.length();
  if(len > 16) len = 16;
  for (byte i = 0; i < len; i++) block_data[i] = str[i];
  for (byte i = len; i < 16; i++) block_data[i] = ' ';
} 