# mBot-maze-solver
CG1111 Project for NUS Sem 1 18/19

Done by Team Technical Difficulties :>

Task: Assemble an mBot and required sensors to solve a maze based on challenges that have to be solved using sensors.

We designed our own IR sensors utilizing components in the lab. Utilizing the PID (Proportional, Integral, Derivative) library from Arduino, we calibrated the sensors such that they will change the turning speed of the wheels in order to avoid collision with the walls of the maze.

We also designed our own microphone using components from the lab such as variable resistors, diodes and capacitors to design a bandpass filter for 300Hz, a high pass filter for 3000Hz frequencies and an envelope detector to check its amplitude. This is for solving the sound challenge whereby we have to decipher which frequency is louder based on their amplitude.

We also calibrated the onboard Light Sensor and RGB lights in order to solve the colour challenge, whereby our mBot will read the RGB values of the coloured paper above it and decide where to go base on the RGB values. By measuing the reflected light intensity after shining Red, Green and Blue light, we are able to roughly measure the RGB values of each colour.

Upon reaching the end of the maze, we utilized the onboard buzzer to play a small victory tune.
