# CS441/541
## Project 4 Template

This repository contains the template materials for project 4.

You may use this file for your project documentation.

- Part 1 : part1/

Questions:
1. If 1 or more philosophers are deadlocked, the program does not print any output and does not exit until CTRL-C is hit
2. If 1 or more philosophers were starving, their number of times ate and thought are significantly lower than the rest of the philosophers
3.  Deadlock did not happen every time, but when the number of philosophers was 2 it deadlocked every time.
4.  Increasing the number of philosophers for version 1 resulted in deadlock happening less often. For both versions, as the
     number of philosophers increased, there was less of a spread or variation in the number of times ate and thought for the
     different philosophers. The numbers correlated to which processes were starving versus those that were not.
5. Out of the 10 simulations that I ran of the second version, philosopher #2 starved the most and the second most
    starved was philosopher #0. I figured this out by calculating the number of times each philosopher had the least and
    second least amount of times eaten and thought. #2 had been the lowest 3 time and the second lowest 2 times.
    #0 had been the second lowest consumer 5 times. After that #4 had 3 times being the lowest consumer and 1 time
    being the second lowest. #3 was the lowest consumer 2 times and the second lowest consumer 2 times. Finally,
    philosopher #1 was the lowest consumer 3 times and the second lowest 0 times. Therefore, it's kind of hard to say that
    any specific philosopher was periodically the lowest consumer.
