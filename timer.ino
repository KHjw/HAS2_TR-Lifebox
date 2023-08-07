void TimerInit(){
    Serial.println("TimerInit");
    blinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);
    BlinkTimer.deleteTimer(blinkTimerId); 
}

//****************************************Blink Timer****************************************
void BlinkTimerStart(int Neo, int NeoColor){
    blinkNeo = Neo;
    blinkColor = NeoColor;
    if(Neo == ALLNEO)   blinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkAllTimerFunc);
    else                blinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);
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
