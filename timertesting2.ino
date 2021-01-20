/////////************* **********/////////
//          Blynk Timer Rules           //
/////////************* **********/////////
/*
#define run&doneTmers = NEVER use any of the 8 functions listed in the readMe on these timers -- this is a timer that is created and allowed to expire before it is created again
   (don't EVER allow loop to come back around on top of this timer and start it again..well you can I guess but you'll suddenly have a lot going on until all 16 timer slots get filled)
#define editedTimers = any timer that you use any of the 8 functions listed in the readMe on. refer to timertesting file for the rules 1-3 that apply to these timers<<very important

#4 It appears like as long as you properly handle timer pointers on ALL editedTimers, you can set run&doneTimers without needing to record their ID to an int. Warning: if you EVER
   forget properly set a pointer for ANY editedTimer, run&doneTimers will not work correctly. they might for awhile but eventually a run&doneTimer will wind up with the same pointer 
   as a editedTimer and suddenly you'll be deleting both a run&doneTimer and a editedTimer
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
        if (!timer.isEnabled(loopT5)) { //creating timers like this allow you to "come back ontop" of this again and again before the timer runs out
            //loopT5 = timer.setInterval(4000L, LEDToggle); //you can setup as many of these timer types as you like(16 actully) Just be sure to set loopT5 back to 99 if you ever call timer.delete(loopT5);
            loopT5 = timer.setTimeout(20000L, [] () { LEDToggle(); loopT5 = 99; });
        }
    }
}

void setup() {
    Time.zone(-6);
    Blynk.begin(auth);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
    ledon = 1;
    
    t0 = timer.setInterval(5000L, createNewTimerFunction); //setInterval timers never call timer.delete >> but if you ever do delete be sure to set its var back to the unsed state
    t1 = timer.setInterval(5000L, sendinfo);
    //t2 = timer.setTimeout(2000L, loop4); // setting timers like this allows the var t2 to come back to haunt you time an again do this instead:
    //t2 = timer.setTimeout(4000L, [] () { loop4(); t2 = 99; }); //asaign the unused timer flag to t2

}

void loop() {
    Blynk.run();
    timer.run();
}
void createNewTimerFunction() {
    timer.setTimeout(2000L, loop4); //this is the timer in question>can we ever mess up this timer since we're not getting an int back for it
}//with the code setup this way the timer in the line above is running in timer slot ID 2 so... if we ever call a timer edit function like-timer.toggle(2); this will mess with this timer

void sendinfo() {
    Blynk.virtualWrite(V0, Time.format("%r - %a %D"));
    int numTi = timer.getNumTimers();
    int t0ON = timer.isEnabled(t0);  //if varaibles used here (t0-t2 and loopT3-loopT5) are set like this: int t0; above setup()
    int t1ON = timer.isEnabled(t1);  //  ALL 6 of these timer.isEnabled are checking to see if timerID 0 is enabled.
    int t2ON = timer.isEnabled(t2);
    int t3ON = timer.isEnabled(loopT3);
    int t4ON = timer.isEnabled(loopT4);
    int t5ON = timer.isEnabled(loopT5);
    
    
    terminal.print(Time.format("%r - # Timers: ")); terminal.println(numTi);
    terminal.print("t0 - EN?: "); terminal.print(t0ON); terminal.print(" timerID: "); terminal.println(t0);
    terminal.print("t1 - EN?: "); terminal.print(t1ON); terminal.print(" timerID: "); terminal.println(t1);
    terminal.print("t2 - EN?: "); terminal.print(t2ON); terminal.print(" timerID: "); terminal.println(t2);
    terminal.print("T3 - EN?: "); terminal.print(t3ON); terminal.print(" timerID: "); terminal.println(loopT3);
    terminal.print("T4 - EN?: "); terminal.print(t4ON); terminal.print(" timerID: "); terminal.println(loopT4);
    terminal.print("T5 - EN?: "); terminal.print(t5ON); terminal.print(" timerID: "); terminal.println(loopT5);
    
    terminal.print("isENbyID-"); //is the timer in the specific slot enabled?
    terminal.print("-ID0:"); terminal.print(timer.isEnabled(0));
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