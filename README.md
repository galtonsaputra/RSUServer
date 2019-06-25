# RSU Server

### Description:
This solution provides an independent verification framework inspecting connected vehicle's broadcasted Basic Safety Messages within a J2735 MessageFrame. It inspects and verifies the speed field within the BSM structure and cross-check speed's field validity against its independent sensor readings. Below proposes a J2735 message content inspector and speed verification framework. 

#### Table of Contents
RSUServer Demonstrator
RSUServer Initial Config
Running the code
Wiring Diagram

## Demonstrator 
### RSU Server</br>
RSU Server was tested and developed using the following materials:
**Physical PI** 
* Raspberry Pi 3 Model B+
* 1 x 32GB MicroSD etched with Raspian Stretch [Kernel version: 4.14]
* 4 x SPDT 3A micro switch with lever wired to RPi GPIO pins
* gcc [v6.3.0]

**Software Components**
- Visual Studio 2017
- WiringPi
- Etcher

### RSU Server TestBed
Two (2) medium-density fibreboards (MDF) installed with four (4) SPDT microswitches and wired to a RaspberryPi. This testbed has the following dimensions: W: 45cm x L: 150cm

The demonstrator with the above requirements delivers the following capabilities:
- RSU able to receive and transmit J2735 message using RPi wireless capabilities 802.11ac [802.11ac/b/g/n] (depending on its model)
- Able to independently verify broadcasted speed field of a BSM within a messageframe using independent speed sensor in the form of lever click.
- SPDT microswitch levers represents independent speed sensors to calculate time in seconds.
- 2 SPDT switches (1-trigger startTime and 1-trigger endTime) gives 1 average speed reading (cm/s).
- Distance is a fixed length of 100cm and using DTS (Distance / Time * Speed) to obtain average speed reading.
- Independent speed sensor readings have a standard deviation of +- 5% for each cross check
- If broadcasted speed readings does not match independent speed sensor readings, the vehicle is tagged as a bad actor. 

### Demonstrator's limitation
- Does not utilise 802.11p and does not fully conform to 1609
- Bad speed reading does not translate to being a bad vehicle being tagged. 
- Inspecting only J2735 - BSM safety messages and verifying its speed field against independent sensor readings



## Initial Config
Once the above have been wired and the code have been compiled to the RaspberryPi, please check the following server configuration:</br>
- RSU server starts by opening and listening on PORT: 8080
- To obtain GPIO pin mapping and its state, please run
```Linux cmd
gpio readall
```
- Please note that the pin number specified at code, mapps to WiringPi numbering (WPi). Additionally, running the above code will also gives you a Broadcom representation of the pins. 


## Running the code
1. Open Terminal
2. Paste the following code to navigate:
```Linux cmd
cd /home/pi/projects/RSUServer/bin/ARM/Debug/RSUServer.out
//The above navigation path is using VS Cross Platform Manager and where it builds and outputs to. 
```
3. To run the server, paste the following:
```Linux cmd
sudo RSUServer.out
```
