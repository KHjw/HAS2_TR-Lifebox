void callback(char* topic, byte* payload, unsigned int length){
  String input_data = "";

  for(int i=0; i<length; i++)
    input_data += (char)payload[i];
  has2_mqtt.SaveByTopic(topic, input_data);

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  Serial.println(input_data);

  Mqtt_updateDS(has2_mqtt.GetData(myDN, "DS"));
}

void Mqtt_updateDS(String myDS){
  BlinkTimer.deleteTimer(BlinkTimerId);
  takechipCNT = 0;
  if(myDS == "scenario"){
    IsScenarioMode = true;
    Mqtt_updateSCN(has2_mqtt.GetData(myDN, "SCN"));
  }
  if(myDS != device_ptr_state){
    sendCommand("sleep=0");
    if(myDS == "OTA"){
      AllNeoColor(GREEN);
      sendCommand("page black");
      has2_mqtt.FirmwareUpdate("ex_lifebox");
    }
    else if(myDS == "setting")      device_ptr = Device_Setting;
    else if(myDS == "ready")        device_ptr = Device_Ready;
    else if(myDS == "activate"){
      sendCommand("chip.vLang.txt=\"Kor\"");      // Nextion 언어변수를 Kor로
      sendCommand("page chip");
      IsScenarioMode = false;
      IsMachineUsed = false;
      AllNeoColor(YELLOW);
      Serial.println("DeviceMode Set :: Manual");
      device_ptr = Device_Manual;
    }
    else if(myDS == "used")         device_ptr = Device_Used;
    else if(myDS == "manual")       device_ptr = Device_Manual;
    else if(myDS == "minigame")     device_ptr = Device_MiniGame;
  }
  Device_ptrPrint(myDS);
}

void Mqtt_updateSCN(String mySCN){
  IsDsSkip = true;
  Serial.println("Scenario RCV :: " + mySCN);
  if(mySCN != "-1" && mySCN != current_scenario){
    Serial.println("Scenario at :: " + mySCN);

    String temp_mySCN = mySCN;
    if(temp_mySCN.startsWith("p")){
      temp_mySCN.remove(0,1);
      if(temp_mySCN.toInt()==1 || (temp_mySCN.toInt()>3 && temp_mySCN.toInt()<65) || temp_mySCN.toInt()>=77){
        sendCommand("page black");
        sendCommand("sleep=1");
        Serial.println("sleep!!");
        device_ptr = Device_Stelth;
      }
    }
    else if(mySCN.startsWith("t")){
      temp_mySCN.remove(0,1);
      if(temp_mySCN.toInt()==1 || (temp_mySCN.toInt()>2 && temp_mySCN.toInt()<39) || temp_mySCN.toInt()>=45){   
        sendCommand("page black");
        sendCommand("sleep=1");
        Serial.println("sleep!!");
        device_ptr = Device_Stelth;
      }
    }
    if(mySCN == "p3"){
      sendCommand("sleep=1");
      device_ptr = Device_PlayerRandBlink;
    }
    else if(mySCN == "t2"){
      sendCommand("sleep=1");
      device_ptr = Device_TaggerRandBlink;
    }
    else if(mySCN == "p65"){
      sendCommand("sleep=0");
      AllNeoColor(YELLOW);
      Scenario_WaitBlink(MID);
      sendCommand("page chip");
      device_ptr = Device_PlayerLogin;
    }
    else if(mySCN == "p69"){
      sendCommand("sleep=0");
      AllNeoColor(YELLOW);
      takechipCNT = 5;
      TakechipTimerFunc();
    }
    else if(mySCN == "t39"){
      sendCommand("sleep=0");
      sendCommand("page chip");
      device_ptr = Device_Selected;
    }
    else if(mySCN == "t40"){
      AllNeoColor(YELLOW);
      Scenario_WaitBlink(MID);
      sendCommand("sleep=0");
      sendCommand("page chip");
      device_ptr = Device_GhostLogin;
    }
    else if(mySCN == "t45"){
      sendCommand("sleep=0");
      sendCommand("page chip");
      device_ptr = Device_Selected;
    }
    else
      Serial.println("Receved Unsigned SCN");
    current_scenario = mySCN;
  }
}

void SituationSend(){
  Serial.println("Situation Send???");
  if(IsScenarioMode)  Serial.println("IsScenarioMode :: true");
  if(!IsDsSkip)  Serial.println("IsDsSkip :: false");
  if(IsScenarioMode && !IsDsSkip){
    Serial.println("Stituation \"tag\" :: Publish");
    has2_mqtt.Situation("tag", tagUser);
  }
}

//****************************************device_ptr Function****************************************
void Device_Manual(){
  RfidLoop("manual");
}

void Device_MiniGame(){
  // Serial.println("DeviceMode Set :: MiniGame");
  IsScenarioMode = false;
  IsMachineUsed = false;
  AllNeoColor(YELLOW);
  RfidLoop("player");
}

void Device_Setting(){
  AllNeoColor(WHITE);
  RfidLoop("MMMM");
}

void Device_Ready(){
  AllNeoColor(RED);
  RfidLoop("MMMM");
}

void Device_Selected(){
  AllNeoColor(YELLOW);
  RfidLoop("MMMM");
}

void Device_PlayerRandBlink(){
  static bool bNeoBlink = false;
  long lnRandomDelay = random(2,8) *100;
  if(bNeoBlink)
    AllNeoColor(GREEN);
  else
    AllNeoColor(BLACK);
  bNeoBlink = !bNeoBlink;
  delay(lnRandomDelay);
}

void Device_TaggerRandBlink(){
  static bool bNeoBlink = false;
  long lnRandomDelay = random(2,8) *100;
  if(bNeoBlink)
    AllNeoColor(PURPLE);
  else
    AllNeoColor(BLACK);
  bNeoBlink = !bNeoBlink;
  delay(lnRandomDelay);
}

void Device_Stelth(){
  // SendCmd("sleep=1");
  AllNeoColor(BLACK);
  RfidLoop("MMMM");
}

void Device_GhostLogin(){
  RfidLoop("ghost");
}

void Device_PlayerLogin(){
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
