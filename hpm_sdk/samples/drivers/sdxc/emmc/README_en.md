# eMMC low-level API demo

## Overview

The demo provides the following features:
- Write the last sector and verify the data
- Write the last 1024 sectors and verify the data
- eMMC sleep & awake

## Board Setting

For the board which doesn't have On-board eMMC devices, An eMMC to TF device is required
For the board that has on-board eMMC devices, this test is based on on-board eMMC device

## Running the example

When the example runs successfully, the board shows the following menu, please input 1 or 2 for testing.

```console

-----------------------------------------------------------------------------------
*                                                                                 *
*                   eMMC Low-level test demo                                      *
*                                                                                 *
*        1. Write & Read the last block                                           *
*        2. Write & Read the last 1024 blocks                                     *
*        3. Demonstrate eMMC sleep & awake                                        *
*                                                                                 *
*---------------------------------------------------------------------------------*

```
