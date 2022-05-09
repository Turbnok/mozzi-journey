# 07_potentiometers
## Table of Contents
- [07_potentiometers](#07_potentiometers)
  - [Table of Contents](#table-of-contents)
  - [Disclaimer](#disclaimer)
  - [What I've learned](#what-ive-learned)
    - [Potentiometers are not perfect](#potentiometers-are-not-perfect)
    - [The circuit may disturb your code](#the-circuit-may-disturb-your-code)
   
## Disclaimer
I'm not a c++ developer nor a arduino expert. I try -with my developer experience- to mimic some classic development technics in order to build simple things. If I say some bs, feel free to make a PR to correct me ! 
## What I've learned
### Potentiometers are not perfect
Reading analogic inputs on an arduino with a patentiometer gives you values between 0 and 1023. But those values are ANALOGICS so the signal is not perfect and reading values is approximative :

**Output of :
int pot = analogRead(A0);
Serial.println(pot);**

```324, 324, 324, 326, 326, 323, 324, 326, 322, etc...``` 

I can't find any hardware solution to this problem and it seems quite normal, so you can eather "smooth" those values or ignore minor changes : I choose this solution.

**pot.cpp**
```cpp
int currentValue = mozziAnalogRead(_pin); 
    if (currentValue > (_value + 2) || currentValue < (_value - 2))
    {
        _value = currentValue;
        _hasChanged = true;
    }else{
        _hasChanged = false;
    }   
```

### The circuit may disturb your code

When I was testing potentiometers everything went fine. I had correct values (plus or minus 2) and playing with potentiometers was pretty accurate so I tried to plug an oscillator and change frequency of the sine wave with a pot (Mozzi HelloWorld). 

The sketch was working but i had some strange frequencies pikes (even without turning knobs). Printing potentiometers values reveals that there were bigger pikes than 2 or -2. I checked my code but it was a power issue : with the speaker plugged values were incorrect. 

Searching on the internet leads me to this page https://diyelectromusic.wordpress.com/2020/08/26/mozzi-output-circuit/
where you can see a resistor and capacitor wired to the output.
This pages links : https://sensorium.github.io/Mozzi/learn/output/ 

I found two resistors and a 100nF capacitor, and voila, the problem is gone. 




