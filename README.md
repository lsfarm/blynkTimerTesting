# blynkTimerTesting
An attempt at making full use of the timer library with mutiple timers. The trouble with multiple timers being created and deleted is the break in the link between the main code and the library when timer.delete(); is called.
You can get an int from the library when creating a timer, but than it's up to the coder to set that int back to some default value(needs to be an int outside the timer array range (is that ?0-15? ?1-16?) when the timer expires and deleteTimer is called.  
Note: If you ever get a compile error of some variable not captured in your timer, declaring that variable globaly seems to fix this problem.(#define your var above setup() )  
Library Files:
[BlynkTimer.h](https://github.com/blynkkk/blynk-library/blob/master/src/Blynk/BlynkTimer.h)
[BlynkTimer.cpp](https://github.com/blynkkk/blynk-library/blob/master/src/utility/BlynkTimer.cpp)  

## 1st Testing
1/18/21: [File used to test this](https://github.com/lsfarm/blynkTimerTesting/blob/main/timertesting.ino) I think the only time you need to worry about keeping track of timers is if a timer will ever "come back on top" of itself before the time has ran out OR if you ever calls the following functions in your code:  
```cpp
//The 8 functions that can edit a running timer:
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
```c
int timerNA = 99; // I'm not sure if 99 is the best number for here yet.  
int myTimer1 = timerNA;  
```
by doing this you avoid setting the timer pointer to it's default state which points to timerID slot 0. I used to setup timers like this:  
int myTimer8; //this will forever cause headaches because this points to timerID 0. and if you call timer.deleteTimer(myTimer8); in loop before any timer.setTimer(); you won't be deleting timer 8, you'll be deleting the timer in slot 0, which is most likely the first timer.setInterval in setup() that probably does something Blynky!  

Than if you ever create a new timer in the loop make sure it's not already created with this:  
```cpp
if (!timer.isEnabled(loopT4)) {// to more I study this the more I think an if like this is all thats needed               
    //start your timer here
}  
```
and finally when you create timers be sure to set their pointers back to timerNA like this:  
```cpp
loopT3 = timer.setTimeout(30000L, [] () { dosomethinglater(); loopT3 = timerNA;  } );
```  
## 2nd Testing  
1/20/21: If you never mess with a timer with any of the 8 functions above and you make sure to allow time for the timer to expire, is it necessary to keep track of it and reset it to timerNA when its deleted? [See this timertesting2 file for this](https://github.com/lsfarm/blynkTimerTesting/blob/main/timertesting2.ino)

## Another Example  
If you create a timer:  
```cpp
if (!timer.isEnabled(loopT5)) {  
  loopT5 = timer.setInterval(4000L, LEDToggle);  
}
```
loopT5 is now equal to 0 -- which means timer ID slot 0  
Then toggle it off:  
```cpp
timer.toggle(loopT5);  
```
now running:  
```cpp
if (!timer.isEnabled(loopT5)) {  
    loopT5 = timer.setInterval(4000L, LEDToggle);  
}  
```
allows another timer to be created in timer slot 1 and the timer in slot 0 is still there, but disabled. I haven't found a way to free up slot 0 again.
not sure how to safely use timer.toggle();
