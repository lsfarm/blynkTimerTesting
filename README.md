# blynkTimerTesting
My attempt at making full use of the timer library with mutiple timers. The trouble with multiple timers being created and deleted is the break in the link between the main code and the library when timer.delete is called.  
Files in question:  
https://github.com/blynkkk/blynk-library/blob/master/src/Blynk/BlynkTimer.h  
https://github.com/blynkkk/blynk-library/blob/master/src/utility/BlynkTimer.cpp  

1/18/21: I think the only time you need to worry about keep track of timers is if a timer will ever "come back on top" of itself before it has ran out OR your ever call the following functions in your code:  
```javascript
bool SimpleTimer::changeInterval(unsigned numTimer, unsigned long d) {  
void SimpleTimer::deleteTimer(unsigned timerId) {  
void SimpleTimer::restartTimer(unsigned numTimer) {  
void SimpleTimer::enable(unsigned numTimer) {  
void SimpleTimer::disable(unsigned numTimer) {  
void SimpleTimer::enableAll() {  
void SimpleTimer::disableAll() {  
void SimpleTimer::toggle(unsigned numTimer) {  
```
If you ever call any of the functions above be sure to track that timer from the very start with this above setup():  
```javascript
int timerNA = 99; // I'm not sure if 99 is the best number for here yet.  
int myTimer1 = timerNA;  
```
by doing this you avoid setting the timer pointer to it's default state which points to timerID slot 0. I used to setup timers like this:  
int myTimer1; //this will forever cause headaches because this points to timer 0. and the first time in your loop that you call timer.deleteTimer(myTimer8); you won't be deleting timer 8, you'll be deleting the timer in slot 0, which is most likely the first timer.setInterval in setup() that probably does something Blynky!  

Than if you ever create a new timer in the loop make sure it's not already created with this:  
```javascrpit
if (!timer.isEnabled(loopT4)) {// to more I study this the more I think an if like this is all thats needed               
    //start your timer here
}  
```
and finally when you create timers be sure to set their pointers back to timerNA like this:  
```javascript
loopT3 = timer.setTimeout(30000L, [] () { dosomethinglater(); loopT3 = timerNA;  } );
```

#Another Example
If you create a timer:  
```javascript
if (!timer.isEnabled(loopT5)) {  
  loopT5 = timer.setInterval(4000L, LEDToggle);  
}
```
loopT5 is now equal to 0 -- which means timer ID slot 0  
Then toggle it off:  
```javascript
timer.toggle(loopT5);  
```
now running:  
```javascript
if (!timer.isEnabled(loopT5)) {  
    loopT5 = timer.setInterval(4000L, LEDToggle);  
}  
```
allows another timer to be created in timer slot 1 and the timer in slot 0 is still there, but disabled. I haven't found a way to free up slot 0 again.
not sure how to safely use timer.toggle();
