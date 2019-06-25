# RSU Server

### Description:
This solution provides an independent verification framework inspecting connected vehicle's broadcasted Basic Safety Messages. It inspects and verifies the speed field within the BSM structure and check its validity. Below proposes a J2735 message content inspector and speed verification framework. 

#### Demonstrator

RSU Server was tested and developed using the following materials:

**Physical RSU**
* Raspberry Pi 3 Model B+
* 1 x 32GB MicroSD etched with Raspian Stretch [Kernel version: 4.14]
* 4 x SPDT 3A micro switch with lever wired to RPi GPIO pins

**RSU Server Testbed**

Two 2 medium-density fibreboards (MDF) was installed with four SPDT microswitches and connected to a RaspberryPi, the testbed has the following dimensions

W: 45cm
L: 150cm



- Receive and transmit BSM using J2735 protocol and 802.11ac

*RSU server starts by opening a port and listens for incoming Raspberry Pi's wireless request [802.11ac/b/g/n] (depending on its model) and serves as a communication hub for connected vehicles with this server. It also 

*It has 2 independent sensor connected to verify its speedrepresented  
It inspects J2735 - BSM safety messages and verifies its speed field against broadcasted


