# I2S Interrupt

## Overview

This example shows I2S transfer audio data with Interrupt way to audio codec on board.

## Project Configuration

- In the file `CMakeLists.txt`, set a matched audio codec type according to the development board schematic，e.g. "set(CONFIG_CODEC "sgtl5000")"

## Board Setting

- Connect headphone to the [headphone](lab_board_app_headphone) interface on the audio codec chip.

## Known Issue

- When using the headphone interface of Codec to play audio on some development boards, there may be crosstalk. For example, when playing mono audio through the left channel of the headphone, weak sound can be heard on the right channel headphone.

## Running the example

When the example runs successfully, the headset will play the sound, and the following message is displayed in the terminal:
```console
I2S Interrupt example
I2C bus is ready
I2S interrupt play finished
```

