# Netx Duo Http Server

## Overview

The netx duo http server example shows netx http server functions. This example enables a http server.

## Board Setting

- Connect an Ethernet port on PC to a RGMII port or a RMII port on the development board with an Ethernet cable

## Project Configuration
- Ethernet Port Settings: Refer to [Ethernet common project settings](../../../../lwip/doc/Ethernet_Common_Project_Settings_en.md)
- By Setting `CONFIG_NETX_ENABLE_DHCP` to zero or comment the definition code in CMakeLists.txt, the DHCP will be disabled
- When DHCP is disabled, comment or delete `set(CONFIG_ECLIPSE_THREADX_NETXDUO_ADDONS_DHCP 1)` can decrease project source files number

## Running the example

When the example runs successfully, it will print phy link status periodically until its status becomes Up. Following message is displayed in the terminal:
When accessing the HTTP server, a window will pop up asking for a username and password. The default username is "name" and the default password is "password". After confirmation, the page will be displayed.

```
NetX HTTP Server unable to find file: /
```

```console
NetXDuo is running
IP address: 192.168.1.223
Mask: 255.255.255.0
Enet phy init passed !
Link Status: Down
Link Status: Down
Link Status: Up
Link Speed:  1000Mbps
Link Duplex: Full duplex
```
