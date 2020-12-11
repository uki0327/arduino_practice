char ledState = 0;

void setup() {
    pinMode(2, INPUT); // PULL-DOWN
    pinMode(13, OUTPUT); // GOES TO TRANSISTER TO CONTROL RELAY
}

void loop() {
    if(digitalRead(2) == HIGH) {
        delay(50);
        if(digitalRead(2) == HIGH)
            ledState = !ledState;
    }        
    digitalWrite(13, ledState);    
}
