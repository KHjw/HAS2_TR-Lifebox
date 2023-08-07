void callback(char* topic, byte* payload, unsigned int length){
  String input_data = "";
  for (int i = 0; i < length; i++)
    input_data += (char)payload[i];

  Serial.print("Message arrived[");
  Serial.print(topic);
  Serial.print("] : ");
  Serial.println(input_data);

  if(input_data == "OTA")               has2_mqtt.FirmwareUpdate("ex_lifebox");
  else if(input_data == "Manual")       game_ptr = Game_Manual;
  else if(input_data == "Setting")      game_ptr = Game_Setting;
  else if(input_data == "Ready")        game_ptr = Game_Ready;
  else if(input_data == "Selected")     game_ptr = Game_Selected;
  else if(input_data == "Login")        game_ptr = Game_Login;
  else if(input_data == "Used")         game_ptr = Game_Used;
}