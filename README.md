# blynkTimerTesting
My attempt at making full use of the timer library with mutiple timers
The trouble with multiple timers being created and delted is the break in the link between the main code and the library when timer.delete is called.

If you create a timer:
    if (!timer.isEnabled(loopT5)) {
        loopT5 = timer.setInterval(4000L, LEDToggle);
    }
loopT5 is now equal to 0 -- which means timer ID slot 0
Then toggle it off:
    timer.toggle(loopT5);
now running:
    if (!timer.isEnabled(loopT5)) {
        loopT5 = timer.setInterval(4000L, LEDToggle);
    }
allows another timer to be created in timer slot 1 and the timer in slot 0 is still there, but disabled. I haven't found a way to free up slot 0 again.
not sure how to safely use timer.toggle();
