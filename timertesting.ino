/////////************* **********/////////
//          Blynk Timer Rules           //
/////////************* **********/////////
/*
#1 Never start another timer in the loop if it is already running if you do, you'll create 2 timers and the first timers timerID is easily lost. (Both timers will run and you'll never be able to dig up the first timer and kill it)
    I was hoping  >>if (!timer.isEnabled(loopT5)) {loopT5 = timer.setInterval(1000L, LEDToggle);}<< would fix this but for some reason all timers (??1-16??)are enabled at setup()
#2 When using setTimer and setTimeout (the ones that will expire) in the loop be sure to set their ID back to ??-1?? ??99?? once finished with them if you don't that expired timerID will come back to haunt you
  timer.toggle(0) seems to toggle a lot of timers.. all the unused ones
  it's quit easy to end end with a lot of timers with the same ID's
#3 If you try to delete a timer at the top of the loop using its ID you'll delete the first timer created in setup because calling timer.delete(ID4) before timer.Setup(ID4)
     makes ID4 in timer.delete = ID0 -- 
*/

#include <blynk.h>
char auth[] = "oxqsxd9OWMutgQ0QlUBn_pt1H5n-cUw4"; //
WidgetTerminal terminal(V10);
BlynkTimer timer;
bool ledon;
int LED = D7;
int button1;
int button2;
int button3;
int button4;

/*
if you create timer pointers like this all three of these will point to timerID 0 until the first timer.Setup is called using one of these int's. So.. it would seem like 
calling timer.delete(t2); would delete timerID 3, but it doesn't. IT WILL DELETE TIMER ID 0. which will most likely be the first timer.setInterval that was created in setup()
int t0;
int t1;
int t2;
Create timer pointer like this instead: */
//if timers are created as setInterval in setup() they will hang onto there timerID forever
int t0 = 99;
int t1 = 99;
int t2 = 99;

//creating timers in the loop() is a whole differant story
int loopT3 = 99;
int loopT4 = 99;
int loopT5 = 99;

int t15;


/////////************* **********///////// button's in Blynk app are set to push. tap a button in blynk and the functions below run once
BLYNK_WRITE(V1) {//this method allows loop4() to be run mutiple times (If you press V1 5 times loop4() will be ran 5 times) each in another timer slot
    button1 = param.asInt();
    if (button1) {
        loopT3 = timer.setTimeout(8000L, [] () { loop4(); loopT3 = 99;  } );
    }
}
BLYNK_WRITE(V2) {//if timer is already running, don't turn start again --This helps, but doesn't entirely fix the problem.  actually mabye it does now that I'm doing loopT4 = 99; above setup()
    button2 = param.asInt();
    if (button2) {
            if (!timer.isEnabled(loopT4)) {
            loopT4 = timer.setInterval(4000L, LEDToggle);
            }
    }
}
BLYNK_WRITE(V3) {
    button3 = param.asInt(); 
    if (button3) {
        timer.deleteTimer(loopT4);
        loopT4 = 99; //<<< Very Important!!  this is the disconnect I talk about  timer.delete should return something to the main code.
        // dont do this: timer.toggle(loopT4);//cause problems if you toggle off than create another timer using loopT4 var. you loose a timer slot
    }
}
BLYNK_WRITE(V4) {//this seems to be the fix I've been looking for--hoping to take this idea back into my other code files. 1/17/21-Testing this
    button4 = param.asInt(); 
    if (button4) {
        if (loopT5 == 99 && !timer.isEnabled(loopT5)) { //creating timers like this allow you to "come back ontop" of this again and again before the timer runs out
            //loopT5 = timer.setInterval(4000L, LEDToggle); //you can setup as many of these timer types as you like(16 actully) Just be sure to set loopT5 back to 99 if you ever call timer.delete(loopT5);
            loopT5 = timer.setTimeout(8000L, [] () { LEDToggle(); loopT5 = 99; });
        }
    }
}

void setup() {
    Time.zone(-6);
    Blynk.begin(auth);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
    ledon = 1;
    
    t0 = timer.setInterval(10000L, WiFifunction); //setInterval timers never call timer.delete >> but if you ever do delete be sure to set its var back to the unsed state
    t1 = timer.setInterval(5000L, sendinfo);
    //t2 = timer.setTimeout(2000L, loop4); // setting timers like this allows the var t2 to come back to haunt you time an again do this instead:
    t2 = timer.setTimeout(4000L, [] () { loop4(); t2 = 99; }); //asaign the unused timer flag to t2

}

void loop() {
    Blynk.run();
    timer.run();
}

void sendinfo() {
    Blynk.virtualWrite(V0, Time.format("%r - %a %D"));
    int numTi = timer.getNumTimers();
    int t0ON = timer.isEnabled(t0);  //if varaibles used here (t0-t2 and loopT3-loopT5) are set like this: int t0; above setup()
    int t1ON = timer.isEnabled(t1);  //  ALL 6 of these timer.isEnabled are checking to see if timerID 0 is enabled.
    int t2ON = timer.isEnabled(t2);
    int t3ON = timer.isEnabled(loopT3);
    int t4ON = timer.isEnabled(loopT4);
    int t5ON = timer.isEnabled(loopT5);
    
    /*int t15ON = timer.isEnabled(15);
    terminal.print("t15 - EN?: ");
    terminal.print(t15ON);
    terminal.print(" t15timerID: ");
    terminal.println(t15);*/
    
    terminal.print(Time.format("%r - # Timers: "));
    terminal.println(numTi);
    terminal.print("t0 - EN?: ");
    terminal.print(t0ON);
    terminal.print(" timerID: ");
    terminal.println(t0);
    terminal.print("t1 - EN?: ");
    terminal.print(t1ON);
    terminal.print(" timerID: ");
    terminal.println(t1);
    terminal.print("t2 - EN?: ");
    terminal.print(t2ON);
    terminal.print(" timerID: ");
    terminal.println(t2);
    terminal.print("T3 - EN?: ");
    terminal.print(t3ON);
    terminal.print(" timerID: ");
    terminal.println(loopT3);
    terminal.print("T4 - EN?: ");
    terminal.print(t4ON);
    terminal.print(" timerID: ");
    terminal.println(loopT4);
    terminal.print("T5 - EN?: ");
    terminal.print(t5ON);
    terminal.print(" timerID: ");
    terminal.println(loopT5);
    terminal.print("isENbyID:"); //is the timer in the specific splot enabled?
    terminal.print(" ID0:"); terminal.print(timer.isEnabled(0));
    terminal.print(" ID1:"); terminal.print(timer.isEnabled(1));
    terminal.print(" ID2:"); terminal.print(timer.isEnabled(2));
    terminal.print(" ID3:"); terminal.print(timer.isEnabled(3));
    terminal.print(" ID4:"); terminal.print(timer.isEnabled(4));
    terminal.print(" ID5:"); terminal.println(timer.isEnabled(5));
    terminal.flush();
}

void WiFifunction() {
    int wifi = WiFi.RSSI().getStrength();
    //Blynk.virtualWrite(V105, wifi);
    terminal.print(Time.format("%r - "));
    terminal.print(wifi);
    terminal.println("% WiFi Strength");
    terminal.flush();
}
void LEDToggle() {
    if(!ledon){
        digitalWrite(LED, HIGH);
        ledon = 1;
    }
    else if (ledon) {
        digitalWrite(LED, LOW);
        ledon = 0;
    }
    terminal.print(Time.format("%r - InToggle State: "));
    terminal.println(ledon);
    terminal.flush();
}

void loop4() {
    terminal.println(Time.format("%r - inLoop4: "));
}
