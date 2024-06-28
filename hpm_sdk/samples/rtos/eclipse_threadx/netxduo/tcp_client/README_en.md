# Netx Duo Tcp Client

## Overview

The netx duo tcp client example shows netx tcp client functions.

## Board Setting

- Connect an Ethernet port on PC to a RGMII port or a RMII port on the development board with an Ethernet cable and ensure that 'SERVER-IPV4-ADDRESS' is accessible.

## Project Configuration
- Ethernet Port Settings: Refer to [Ethernet common project settings](../../../../lwip/doc/Ethernet_Common_Project_Settings_en.md)
- By Setting `CONFIG_NETX_ENABLE_DHCP` to zero or comment the definition code in CMakeLists.txt, the DHCP will be disabled
- When DHCP is disabled, comment or delete `set(CONFIG_ECLIPSE_THREADX_NETXDUO_ADDONS_DHCP 1)` can decrease project source files number
- When DHCP is enabled, make sure the tcp server `SERVER_IPV4_ADDRESS` is reachable.

## Running the example

When the example runs successfully, it will try to connect **SERVER_IPV4_ADDRESS : ECHO_SERVER_PORT**(192.168.1.2:7777). Once connected, it will send message **"ABCDEFGHIJKLMNOPQRSTUVWXYZ "** to the host. Then waiting for receive message from the host forever until it really receive one or server close the connection. It will print phy link status periodically until its status becomes **Up**. If received message, it will also print it to the console. If server close the connection, the receive function will return immediately. Following message is displayed in the terminal:
```console
NetXDuo is running
IPv4 address: 192.168.1.223
Mask: 255.255.255.0
Enet phy init passed !
Link Status: Down
Link Status: Up
Link Speed:  1000Mbps
Link Duplex: Full duplex
Connecting to server: Connected
Receive response: abcdefg
Receive response: abcdefg
Receive response: abcdefg
Receive response: abcdefg
No response from server
```
