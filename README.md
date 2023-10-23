# avr_processing
Control 2 DOF arm with Processing program. The avr_robot.c code is for CodevisionAVR, atmega128. I used packet to transmit mouse cordinate, the form of packet is 0, mouseX, mouseY. If the packet doesn't start with 0, ignore that. You may upgrade this code with interrupt. HC-SR04 has recommend cycle period as 50ms, so you can read data every 50ms with interrupts of timers.
