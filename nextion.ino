void NextionInit(){
  nexInit();
  nexHwSerial.begin(9600, SERIAL_8N1, SERIAL2_RX_PIN, SERIAL2_TX_PIN);
  sendCommand("chip.vLang.txt=\"Kor\"");          // Nextion 언어변수를 Kor로
  sendCommand("page chip");
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