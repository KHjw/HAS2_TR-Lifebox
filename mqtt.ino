void callback(char* topic, byte* payload, unsigned int length){
  String input_data = "";

  for(int i=0; i<length; i++)
    input_data += (char)payload[i];
  has2_mqtt.SaveByTopic(topic, input_data);

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  Serial.println(input_data);

  Mqtt_myDN(input_data);  // myDN 등록
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
    if(myDS == "OTA")               has2_mqtt.FirmwareUpdate("lifebox");
    else if(myDS == "setting")      device_ptr = Device_Setting;
    else if(myDS == "ready")        device_ptr = Device_Ready;
    else if(myDS == "used")         device_ptr = Device_Used;
    else if(myDS == "manual")       device_ptr = Device_Manual;
    else if(myDS == "minigame")     device_ptr = Device_MiniGame;
  }
  Device_ptrPrint(myDS);
}

void Mqtt_updateSCN(String mySCN){
  if(mySCN != current_scenario){
    if(mySCN == "t42")       device_ptr = Device_GhostLogin;
    else if(mySCN == "t")    device_ptr = Device_PlayerLogin;
    else if(mySCN == "tt")   device_ptr = Device_Used;
    else                    device_ptr = VoidFunc;
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
  BlinkTimerStart(MID, WHITE);            // 태그부 흰색 점멸 시작
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