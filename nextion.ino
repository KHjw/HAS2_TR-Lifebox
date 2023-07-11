void NextionInit(){
  nexInit();
  nexHwSerial.begin(9600, SERIAL_8N1, SERIAL2_RX_PIN, SERIAL2_TX_PIN);
}

void SendCmd(String command){           // 영문용 디스플레이 send
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
  SendCmd("pgItemOpen.vExp.val=50");    // 경험치 +50 차있는걸로 기본세팅
}

void BatteryPackSend(){
  SendCmd("picBatteryPack.pic=5");
  SendCmd("pgItemOpen.vBatteryPack.val=3");    // 배터리팩 3개 차있는걸로 기본세팅
}
