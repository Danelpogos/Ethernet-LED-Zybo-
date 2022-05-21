# Final Project 
 # Lab7 

## part1 
------
In this project I managed to have the Leds of the ZYBo Z10 baord to turn on depending on the input value though ethernet communication protocol. I am sending the data through TCP/IP clinet and server connection. 

TCP/IP stands for transmission control protocol and internet protocol.This communication protocls identify how the data should be broken down, addressed, trnsmitted, routed and recieved. The TCP protocol defines how application create communication channles, Also, manages how message is broken down to be transmitted. IP defines how to address and route packets for delivery. 
The functionlty divides into four layers Application,transport layer, network layer and physcal layer. THe application layer protocols inlcude hhtp, ftp. 

![Firmware](Firmware.png)

THis is our enire design for the firmware. We ran aut conaction which creatd the DDR and FIXED input output ports. We also enbaled UART1 and ETHN

Echo server connects to ethernet and talks with the router to see if the DHCP canbe create a dynamic IP. DHCP stands for Dynamic Host configuration Protcol. THe router job is to give IPs to the devices that try to connect to the intenret. The router has a table of IPs for diffent devices that were connected to the router at one point. So, when a new device tries to connect to the intnet the router will look at its tabel to see if it has an assigned IP for that device. If not the router will assign a new IP to that device. This method is called DHCP.
In Static IP assingin the device will tell the router that it wants to connect to the internet with the specific IP address. And the router will update its routing table and accept the IP the device wants to use. 


First thing we see on the vitis terminal is that it starts the PHY autonegotioation.After the autonegotion we the spped of the ethernet data Then the router assignas the baord with an IP. Assings the netmask, gateaway address and assigns the server to listen to clinet with port 7. 
Once the connection takes place on the terminal we type telnet the Ip address assigned to the board and the servers port number.AND we enter the telnet interactive shell if we type anything on the terminal it will echo back to us on the terminal. 


In echo.c the int start_application() function starts our echoing application. In this function we instinatiate the TCP/IP. TCP is ...Ans srarts to listen to the network. 


Uart unlike ethenrt provides only one serial port connected to the system. In the ethernt system each device or system connecting will have their own session. In TCP/IP connection the server always listens to the client to send request. And based on the users application code the echoing or sending an HTML file to the clinet the server will drop end it connection with the server. 


## Part2 
## Vitis
---------
We used the same firmware built in part1. However, we built a difreent application in vitis. We built lwiP TCP perf server. This appilication is used ofr creating TCP server and measure the downlink performance weight IP stack. This application like the echo server will assign a ip address to the device based on its MAC adddress. THe application creates  TCP server on board and listens to TCP client connection. It is responsible for displayin connection information on the terminal. 


I is the inteval how often we send the data from server to clinet. T is the time in second. And the w is the TCP window, how big the data will be sending. 



## Part3 
## Vitis
--------
We createda petalinux application and ran a HTML website on it. 

1) petalinux create -t project -n webserver --template zynq

-t = could be porject, application, or modules. 
-n = name

2) we need to copy the firmeare .xsa file built in part 1 into the petalinux hardware description folder. 

petalinux-config --get-hw-description ../../hardware/exports/webserver_fw_wrapper.xsa

3) petalinux-config -c rootfs
Filesystem packages->base->busybox
We enabled busybox-httpd, busybox-udhcpd, busybox-inetd. 

4)petalinx-build 
This command will build our application with HTML enabled, so that we can run the HTML website on it. 

5) petalinux-boot --jtag --kernel
this commmand builds the processor. 

We programmed the board on vivado first. Then on the commnad line we run petalinux-boot --jtag --kernel to boot the kernel. On another terminal we run sudo screen /dev/ttyUSB1 115200. 
Laslty, when the peralinux booted we need to go to /srv/www of the petalinux. And create the index.html page code there. 

 