# 09 Sequencer
## Table of Contents
- [09 Sequencer](#09-sequencer)
  - [Table of Contents](#table-of-contents)
  - [Disclaimer](#disclaimer)
  - [What I've learned](#what-ive-learned)
    - [adding sounds together is not just an addition](#adding-sounds-together-is-not-just-an-addition)
   
## Disclaimer
I'm not a c++ developer nor a arduino expert and even less a sound expert. I try -with my developer experience- to mimic some classic development technics in order to build simple things. If I say some bs, feel free to make a PR to correct me ! 
## What I've learned

### adding sounds together is not just an addition
With the sequencer running, we can now have multiple notes playing at the same time : in my case, a noise and a triangle wave. In lot of Mozzi exemples, forums, etc... you can see people adding two oscillators signals and divide by 2 the result. It's actually *wrong*. I found a note of a person (Viktor T. Toth) who had the same problem and find a easy way to correct it : http://www.vttoth.com/CMS/index.php/technical-notes/68 

As he explains in his note, if you have two signals from 0 to 255 but with the "no sound" in the middle, you have to test if both sounds are below the middle or not.

**pseudo code** 
```cpp
output =(A*B) >> 7;

output =2*(A+B)−(A*B >>7)−256;
```
It works fine even with 4 signals by adding all of them : A+B=>E, C+D=>F, F+E=> output