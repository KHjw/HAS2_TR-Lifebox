void RfidInit(){
  RestartPn532:
  nfc.begin();
  if (!(nfc.getFirmwareVersion())){
    Serial.println("PN532 연결실패");
    AllNeoColor(WHITE);
    Serial.println("pn532 INIT 재실행");
    delay(50);
    goto RestartPn532;
  }
  else{
    nfc.SAMConfig();
    Serial.println("PN532 연결성공");
    AllNeoColor(RED);
  }
  delay(100);
}

void RfidLoop(String mode){
  uint8_t uid[3][7] = {{0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0}}; // Buffer to store the returned UID
  uint8_t uidLength[] = {0};                   // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t data[32];
  byte pn532_packetbuffer11[64];
  pn532_packetbuffer11[0] = 0x00;

  if (nfc.sendCommandCheckAck(pn532_packetbuffer11, 1)){              // rfid 통신 가능한 상태인지 확인
    if (nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)){   // rfid에 tag 찍혔는지 확인용 //데이터 들어오면 uid정보 가져오기
      if (nfc.ntag2xx_ReadPage(7, data)){                             // ntag 데이터에 접근해서 불러와서 data행열에 저장
        Serial.println("TAGGGED");
        for(int i = 0; i < 4; i++)                                    //GxPx 데이터만 배열에서 추출해서 string으로 저장
          tagUser += (char)data[i];
        if(mode == "manual")        Manual_PlayerCheck();
        else PlayerCheck(mode);
      }
    }
  }
}

//****************************************PlayerCheck Function****************************************
void Manual_PlayerCheck(){ //어떤 카드가 들어왔는지 확인용
  Serial.println("tag_user_data : " + tagUser);     // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
  switch(tagUser[3]){
    case '1':
      Serial.println("Tagger Tagged");
      break;
    case '2':
      Serial.println("Ghost Tagged");
      break;
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
      Serial.println("Player Tagged");
      Rfid_Manual();
      break;
    case 'M':
      ESP.restart();
      break;
    case 'E':
      AllNeoColor(PURPLE);
      sendCommand("chip.vLang.txt=\"Eng\"");          // Nextion 언어변수를 Eng로
      delay(10);
      sendCommand("page chip");                       // 초기페이지 세팅
      Serial.println("Language set to English");
      delay(10);
      ESP.restart();
      break;
    default: 
    Serial.println("Wrong TAG");
    break;
  }
}

void PlayerCheck(String mode){
  Serial.println("tag_user_data : " + tagUser);     // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
  if(tagUser == "MMMM")   ESP.restart();            // 스태프카드 인식 시 초기화

  String tagUserRole = has2_mqtt.GetData(tagUser, "R");    // 2. Role 정보

  if(tagUserRole == "player"){                      // 3. Role 판단
    Serial.println("Player Tagged");
    if(mode == "player"){
      if(!(IsTakingChip))
        Rfid_Player();
    }
    if(mode == "used")    Rfid_Used();
  }
  else if(tagUserRole == "tagger"){
    Serial.println("Tagger Tagged");
  }
  else if(tagUserRole == "ghost"){
    Serial.println("Ghost Tagged");
    if(mode == "ghost")   Rfid_Ghost();
  }
  else{
    Serial.println("Wrong TAG");
  }
}

//****************************************rfid_ptr Function****************************************
void Rfid_Manual(){
  if(!IsMachineUsed){
    sendCommand("page life_chip_send");
    delay(5000);
    sendCommand("page no_chip");
    AllNeoColor(BLUE);
    IsMachineUsed = true;
  }
  else{
    sendCommand("disable.en=1");
  }
  delay(1000);
}

void Rfid_Ghost(){
  has2_mqtt.Situation("tag", tagUser);
  device_ptr = Device_PlayerLogin;
}

void Rfid_Player(){
  has2_mqtt.Situation("tag", tagUser);
  sendCommand("page life_chip_send");
  IsTakingChip = true;
  TakechipTimer.deleteTimer(TakechipTimerId);
  TakechipTimerId = TakechipTimer.setInterval(blinkTime,TakechipTimerFunc);
}

void Rfid_Used(){
  sendCommand("disable.en=1");
  delay(1000);
}

void Rfid_MiniGame(){

}