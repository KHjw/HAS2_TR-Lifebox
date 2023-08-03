# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Lifebox\\HAS2_TR-Lifebox.ino"
 /*

 *

 * @file HAS2_TR-Lifebox.ino

 * @author 홍진우 (kevinlike@naver.com)

 * @brief

 * @version 1.0

 * @date 2023-05-30

 *

 * @copyright Copyright (c) 2023

 *

 */
# 13 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Lifebox\\HAS2_TR-Lifebox.ino"
# 14 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Lifebox\\HAS2_TR-Lifebox.ino" 2

void setup(){
  Serial.begin(115200);
  Serial.println("=============================ESP SETUP=============================");
  has2_mqtt.Setup(callback); // tp-link 접속; ALL, myMAC 구독
  TimerInit();
  RfidInit();
  NeopixelInit();
  NextionInit();
  Serial.println("===================================================================");
}

void loop() {
  device_ptr();
  BlinkTimer.run();
  TakechipTimer.run();
}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Lifebox\\mqtt.ino"
void callback(char* topic, byte* payload, unsigned int length){
  String input_data = "";

  for(int i=0; i<length; i++)
    input_data += (char)payload[i];
  has2_mqtt.SaveByTopic(topic, input_data);

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  Serial.println(input_data);

  Mqtt_myDN(input_data); // myDN 등록
  Mqtt_updateDS(has2_mqtt.GetData(myDN, "DS"));
}

void Mqtt_myDN(String input_data){
  if((char)myDN[1] != 'R'){
    for(int i=0; i<3; i++)
      myDN += (char)input_data[i];
  }
}

void Mqtt_updateDS(String myDS){
  if(myDS == "scenario")
    Mqtt_updateSCN(has2_mqtt.GetData(myDN, "SCN"));
  if(myDS != device_ptr_state){
    if(myDS == "OTA") has2_mqtt.FirmwareUpdate("lifebox");
    else if(myDS == "setting") device_ptr = Device_Setting;
    else if(myDS == "ready") device_ptr = Device_Ready;
    else if(myDS == "used") device_ptr = Device_Used;
    else if(myDS == "manual") device_ptr = Device_Manual;
    else if(myDS == "minigame") device_ptr = Device_MiniGame;
  }
  Device_ptrPrint(myDS);
}

void Mqtt_updateSCN(String mySCN){
  if(mySCN != current_scenario){
    if(mySCN == "t42") device_ptr = Device_GhostLogin;
    else if(mySCN == "t") device_ptr = Device_PlayerLogin;
    else if(mySCN == "tt") device_ptr = Device_Used;
    else device_ptr = VoidFunc;
    current_scenario = mySCN;
  }
}

//****************************************device_ptr Function****************************************
void Device_Manual(){
  Serial.println("DeviceMode Set :: Manual");
  IsMachineUsed = false;
  AllNeoColor(YELLOW);
  RfidLoop("manual");
}

void Device_MiniGame(){
  Serial.println("DeviceMode Set :: MiniGame");
  IsMachineUsed = false;
  AllNeoColor(YELLOW);
  RfidLoop("player");
}

void Device_Setting(){
  AllNeoColor(WHITE);
}

void Device_Ready(){
  AllNeoColor(RED);
}

void Device_Stelth(){
  AllNeoColor(BLACK);
}

void Device_GhostLogin(){
  BlinkTimer.deleteTimer(BlinkTimerId);
  AllNeoColor(YELLOW);
  BlinkTimerStart(MID, WHITE); // 태그부 흰색 점멸 시작
  RfidLoop("ghost");
}

void Device_PlayerLogin(){
  AllNeoColor(YELLOW);
  RfidLoop("player");
}

void Device_Used(){
  AllNeoColor(BLUE);
  RfidLoop("used");
}

// 포인터 초기화용 빈 함수
void VoidFunc(){}

// 게임 포인터 상태 SerialPrint
void Device_ptrPrint(String print){
  if(print != device_ptr_state){
    if((char)print[1] == 'c' && (char)print[2] == 'n')
      Serial.println("Training Scenario :: " + print);
    else
      Serial.println("Device State :: " + print);
    device_ptr_state = print;
  }
}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Lifebox\\neopixel.ino"
void NeopixelInit(){
  for(int i=0; i<NeoNum; i++){
    pixels[i].begin();
  }
}

void AllNeoColor(int color_code){
  for(int n=0; n<NeoNum; n++){
    pixels[n].lightColor(color[color_code]);
  }
}

void NeoBlink(int neo_code, int color_code, int blink_num, int blink_time){
  for(int n=0; n<blink_num; n++){
    pixels[neo_code].lightColor(color[BLACK]);
    delay(blink_time);
    pixels[neo_code].lightColor(color[color_code]);
    delay(blink_time);
  }
}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Lifebox\\nextion.ino"
void NextionInit(){
  nexInit();
  nexHwSerial.begin(9600, 0x800001c, 39, 33);
}

void SendCmd(String command){ // 영문용 디스플레이 send
  String cmd = "";
  if(command.startsWith("page") && Language=="Eng"){
    cmd = "page E" + command.substring(5);
  }
  else{
    cmd = command;
  }
  sendCommand(cmd.c_str());
}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Lifebox\\rfid.ino"
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
  uint8_t uidLength[] = {0}; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t data[32];
  byte pn532_packetbuffer11[64];
  pn532_packetbuffer11[0] = 0x00;

  if (nfc.sendCommandCheckAck(pn532_packetbuffer11, 1)){ // rfid 통신 가능한 상태인지 확인
    if (nfc.startPassiveTargetIDDetection((0x00))){ // rfid에 tag 찍혔는지 확인용 //데이터 들어오면 uid정보 가져오기
      if (nfc.ntag2xx_ReadPage(7, data)){ // ntag 데이터에 접근해서 불러와서 data행열에 저장
        Serial.println("TAGGGED");
        for(int i = 0; i < 4; i++) //GxPx 데이터만 배열에서 추출해서 string으로 저장
          tagUser += (char)data[i];
        if(mode == "manual") Manual_PlayerCheck();
        else PlayerCheck(mode);
      }
    }
  }
}

//****************************************PlayerCheck Function****************************************
void Manual_PlayerCheck(){ //어떤 카드가 들어왔는지 확인용
  Serial.println("tag_user_data : " + tagUser); // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
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
      sendCommand("chip.vLang.txt=\"Eng\""); // Nextion 언어변수를 Eng로
      delay(10);
      sendCommand("page chip"); // 초기페이지 세팅
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
  Serial.println("tag_user_data : " + tagUser); // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
  if(tagUser == "MMMM") ESP.restart(); // 스태프카드 인식 시 초기화

  String tagUserRole = has2_mqtt.GetData(tagUser, "R"); // 2. Role 정보

  if(tagUserRole == "player"){ // 3. Role 판단
    Serial.println("Player Tagged");
    if(mode == "player"){
      if(!(IsTakingChip))
        Rfid_Player();
    }
    if(mode == "used") Rfid_Used();
  }
  else if(tagUserRole == "tagger"){
    Serial.println("Tagger Tagged");
  }
  else if(tagUserRole == "ghost"){
    Serial.println("Ghost Tagged");
    if(mode == "ghost") Rfid_Ghost();
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
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-Lifebox\\timer.ino"
void TimerInit(){
    BlinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);
    BlinkTimer.deleteTimer(BlinkTimerId);
    TakechipTimerId = TakechipTimer.setInterval(blinkTime,TakechipTimerFunc);
    TakechipTimer.deleteTimer(TakechipTimerId);
}

//****************************************Blink Timer****************************************
void BlinkTimerStart(int Neo, int NeoColor){
    blinkNeo = Neo;
    blinkColor = NeoColor;
    if(Neo == ALLNEO) BlinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkAllTimerFunc);
    else BlinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);
}

void BlinkTimerFunc(){
    Serial.println("Blink!");
    if(blinkOn == true){
        pixels[blinkNeo].lightColor(color[blinkColor]);
        blinkOn = false;
    }
    else{
        pixels[blinkNeo].lightColor(color[BLACK]);
        blinkOn = true;
    }
}

void BlinkAllTimerFunc(){
    Serial.println("ALL Blink!");
    if(blinkOn == true){
        for(int i=0; i<NeoNum; i++){
            pixels[i].lightColor(color[blinkColor]);
        }
        blinkOn = false;
    }
    else{
        for(int i=0; i<NeoNum; i++){
            pixels[i].lightColor(color[BLACK]);
        }
        blinkOn = true;
    }
}

//****************************************Takechip Timer****************************************
void TakechipTimerFunc(){
    if(takechipCNT>5){
        takechipCNT = 0;
        sendCommand("page no_chip");
        IsMachineUsed = true;
        IsTakingChip = false;
        device_ptr = Device_Used;
        TakechipTimer.deleteTimer(TakechipTimerId);
    }
    takechipCNT++;
}
