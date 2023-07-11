# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Lifebox\\HAS2_TR-Lifebox.ino"
 /*

 *

 * @file TR_Lifebox.ino

 * @author 홍진우 (kevinlike@naver.com)

 * @brief

 * @version 1.0

 * @date 2023-05-30

 *

 * @copyright Copyright (c) 2023

 *

 */
# 13 "c:\\Github\\HAS2_TR\\HAS2_TR-Lifebox\\HAS2_TR-Lifebox.ino"
# 14 "c:\\Github\\HAS2_TR\\HAS2_TR-Lifebox\\HAS2_TR-Lifebox.ino" 2

void setup() {
  Serial.begin(115200);
  has2_mqtt.Setup(callback); // tp-link 접속; ALL, myMAC 구독
  // has2_mqtt.AddSubscirbe("EI2");
  TimerInit();
  RfidInit();
  NeopixelInit();
  NextionInit();
}

void loop() {
  game_ptr();
  BlinkTimer.run();
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Lifebox\\Game.ino"
//****************************************game_ptr Function****************************************
void Game_Void(){ //포인터 초기세팅용 void함수
}

void Game_ptrPrint(String print){ // 게임 포인터 상태 SerialPrint
  if(print != game_ptr_state){
    Serial.println("Game pointer at ::" + print);
    game_ptr_state = print;
  }
}

void Game_Manual(){
  Game_ptrPrint("Game_Manual");
  AllNeoColor(YELLOW);
  rfid_ptr = Mode_Mannual;
}

void Game_Setting(){
  Game_ptrPrint("Game_Setting");
  AllNeoColor(WHITE);
}

void Game_Ready(){
  Game_ptrPrint("Game_Ready");
  AllNeoColor(RED);
}

void Game_Selected(){
  Game_ptrPrint("Game_Selected");
  Serial.println("game_ptr :: Game_Selected");
  BlinkTimer.deleteTimer(blinkTimerId);
  BlinkTimerStart(ALLNEO, WHITE); // 전체 흰색 점멸 시작
  RfidLoop();
}

void Game_Login(){
  Game_ptrPrint("Game_Login");
  AllNeoColor(RED);
}

void Game_Used(){
  Game_ptrPrint("Game_Used");
  AllNeoColor(BLUE);
}`-

//****************************************rfid_ptr Function****************************************
void Mode_Mannual(){
  Serial.println("Player Tagged");
  if(!machine_used){
    sendCommand("page life_chip_send");
    delay(5000);
    sendCommand("page no_chip");
    AllNeoColor(BLUE);
    machine_used = true;
  }
  else{
    sendCommand("disable.en=1");
  }
  delay(1000);
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Lifebox\\mqtt.ino"
void callback(char* topic, byte* payload, unsigned int length)
{
  static bool start = false;
  if(!start){
    start = true;
    has2_mqtt.Publish(my_topic, "start");
    return ;
  }

  String input_data = "";

  for (int i = 0; i < length; i++)
    input_data += (char)payload[i];

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  Serial.println(input_data);

  if(input_data == "OTA") has2_mqtt.FirmwareUpdate("itembox");
  else if(input_data == "Manual") game_ptr = Game_Manual;
  else if(input_data == "Setting") game_ptr = Game_Setting;
  else if(input_data == "Ready") game_ptr = Game_Ready;
  else if(input_data == "Activate") game_ptr = Game_OUTPN532_login;
  else if(input_data == "Selected") game_ptr = Game_Selected;
  else if(input_data == "Login") game_ptr = Game_Login;
  else if(input_data == "Used") game_ptr = Game_Used;

  if(topic == ALL){
  }
  else if(topic == my_topic){
  }
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Lifebox\\neopixel.ino"
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
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Lifebox\\nextion.ino"
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

void ExpSend(){
  SendCmd("picExp.pic=2");
  SendCmd("pgItemOpen.vExp.val=50"); // 경험치 +50 차있는걸로 기본세팅
}

void BatteryPackSend(){
  SendCmd("picBatteryPack.pic=5");
  SendCmd("pgItemOpen.vBatteryPack.val=3"); // 배터리팩 3개 차있는걸로 기본세팅
}
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Lifebox\\rfid.ino"
void RfidInit(){
  RestartPn532:
  for (int i = 0; i < rfid_num; ++i)
  {
    nfc[i].begin();
    if (!(nfc[i].getFirmwareVersion()))
    {
      Serial.print("PN532 연결실패 : ");
      Serial.println(i);
      AllNeoColor(WHITE);
      Serial.println("pn532 INIT 재실행");
      goto RestartPn532;
    }
    else
    {
      nfc[i].SAMConfig();
      Serial.print("PN532 연결성공 : ");
      Serial.println(i);
      rfid_init_complete[i] = true;
      AllNeoColor(RED);
    }
    delay(100);
  }
}

void RfidLoop(){
  int pn532_code = 1; // RFID 한개만 사용

  uint8_t uid[3][7] = {{0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0}}; // Buffer to store the returned UID
  uint8_t uidLength[] = {0}; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t data[32];
  byte pn532_packetbuffer11[64];
  pn532_packetbuffer11[0] = 0x00;

  if (nfc[pn532_code].sendCommandCheckAck(pn532_packetbuffer11, 1)){ // rfid 통신 가능한 상태인지 확인
    if (nfc[pn532_code].startPassiveTargetIDDetection((0x00))){ // rfid에 tag 찍혔는지 확인용 //데이터 들어오면 uid정보 가져오기
      if (nfc[pn532_code].ntag2xx_ReadPage(7, data)){ // ntag 데이터에 접근해서 불러와서 data행열에 저장
        Serial.println("TAGGGED");
        CheckingPlayers(data);
      }
    }
  }
}

void CheckingPlayers(uint8_t rfidData[32]){ //어떤 카드가 들어왔는지 확인용
  String tagUser = "";
  for(int i = 0; i < 4; i++) //GxPx 데이터만 배열에서 추출해서 string으로 저장
    tagUser += (char)rfidData[i];
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
      rfid_ptr();
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
# 1 "c:\\Github\\HAS2_TR\\HAS2_TR-Lifebox\\timer.ino"
void TimerInit(){
    blinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);
    BlinkTimer.deleteTimer(blinkTimerId);
}

//****************************************Blink Timer****************************************
void BlinkTimerStart(int Neo, int NeoColor){
    blinkNeo = Neo;
    blinkColor = NeoColor;
    if(Neo == ALLNEO) blinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkAllTimerFunc);
    else blinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);
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
