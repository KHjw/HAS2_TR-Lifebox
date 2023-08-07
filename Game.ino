//****************************************game_ptr Function****************************************
void Game_Void(){}      //포인터 초기세팅용 void함수

void Game_ptrPrint(String print){     // 게임 포인터 상태 SerialPrint
  if(print != game_ptr_state){
    Serial.println("Game pointer at :: " + print);
    game_ptr_state = print;
  }
}

void Game_Manual(){
  Game_ptrPrint("Game_Manual");
  RfidLoop();
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
  BlinkTimerStart(ALLNEO, WHITE);            // 전체 흰색 점멸 시작
  RfidLoop();
}

void Game_Login(){
  Game_ptrPrint("Game_Login");
  AllNeoColor(RED);
}

void Game_Used(){
  Game_ptrPrint("Game_Used");
  AllNeoColor(BLUE);
}

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