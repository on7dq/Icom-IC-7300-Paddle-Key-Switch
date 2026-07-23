# Icom-IC-7300-Paddle-Key-Switch

![IC-7300 Mini Controller](https://user-images.githubusercontent.com/17215772/217466927-17b35049-a6ed-4161-aca5-fb4c072b88ef.jpg)

Avoid having to push 8 keystrokes on your IC-7300, to make the switch from paddle to straight key !
Easy with just one button press with this project.

All info here (first version)

https://on7dq.blogspot.com/2017/02/ic-7300-paddle-or-key-easy-solution.html

NEW VERSION - FEB 2023

I brushed up the code, and added some code for the remaining buttons.
Of course you don't need a Nintendo Controller, you can build it any way you want, all you need is any Arduino board and 8 buttons, a 1N4148 diode, a 10k resistor and some wire.

See description of the new version here
https://on7dq.blogspot.com/2023/02/ic-7300-paddle-key-switch-revisited.html

NEW VERSION - JULY 2026

The new model **IC-7300 Mk2** has a revised CI-V command table, as a consequence some commands have changed (for this project : the Key/Paddle and the USB AF/IF codes, other commands were unchanged). The hardware remains the same.
Alas, still no command to send the 8 CW messages, only the voice messages are available.
See the new Arduino sketch above, which works with the **IC-7300 Mk2** :
_MiniPro_IC7300Mk2_PaddleKeyKSwitch_2026.ino_

Enjoy ! 
Luc ON7DQ (KF0CR)
