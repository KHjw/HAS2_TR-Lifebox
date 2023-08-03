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
    if(Neo == ALLNEO)   BlinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkAllTimerFunc);
    else                BlinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);
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