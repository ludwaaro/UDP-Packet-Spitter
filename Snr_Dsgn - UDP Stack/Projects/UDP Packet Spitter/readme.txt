Project:     	UDP Packet Spitter
Author:      	Aaron Ludwig
Date:        	November 14, 2019

Description: 	Project contains an example project utilizing FreeRTOS's support for UDP communication 
	     	functionality in the FreeRTOS + TCP library running on a chipKIT_Pro_MX7 running the 
	     	PIC32MX795F512L processor. The operating system creates and manages a task that sends UDP 
	     	packets with predefined contents to a predefined destination address at regular intervals.

Configuration:	There are three define statements at the top of main.c that should be changed for use on
		your board.
		sourceAddress:     	An array of integers representing the source IP Address you want to use.
		destinationAddress:     A string representing the destination IP Address you want to use.
		UDPContents:            The contents of the message you wish to send.

Notes:       	This project expands on the base FreeRTOS project RD1.X from the FreeRTOS.zip file found 
	     	on Dr. Frenzel's website to utilize the aspects of the FreeRTOS + TCP library supporting
	     	UDP communication. 

		This project draws from the following sources other than the RD1.X base project and the 
		FreeRTOS and FreeRTOS + TCP libraries to implement these changes:

		The FreeRTOS + TCP Tutorial from FreeRTOS.org:
	     	
			This website was the main source for the function used for vUDPSendUsingStandardInterface:
			the task used for sending UDP packets in the application.
		
	     		url: https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/Networking_Tutorial_Sending_UDP_Data.html

	     	The GitHub project "FreeRTOS-TCP-for-PIC32" from user jjr-simiatec:
		
			This project was the source of many files related to the network driver used. More 
			specifically, modified or unmodified versions of the following files of this project 
			originate from this source:
				Ethernet.h
				EthernetPrivate.h
				PHYGeneric.h
				PIC32Arch.h
				PHYHardwareProfile.h (M)
				NetworkInterface.c (M)
				PHY_isr.S
				Ethernet_ISR.S
				PHYGeneric.c
				LAN8740.c (M) (now LAN8720.c)
			*(M means the file was modified for my project)
		
			Files from this project were because the ethernet driver for the project was said 
			to work on the PIC32MX795F512L. Thus, only functionality relating to the PHY had 
			to be changed for it to be operational for the Cerebot MX7cK.
		
	     		url: https://github.com/jjr-simiatec/FreeRTOS-TCP-for-PIC32

	     	The Web-Vending Machine Project from Dr. Frenzel's website:
		
			This project was the source of all other files relating to the PHY chip for the board.
			Modified or unmodified versions of the following files of this project originate from 
			this source:
				ETHPIC32ExtPhy.h (M)
				ETHPIC32ExtPhyRegs.h
				ETHPIC32ExtPhySMSC8720.h
				ETHPIC32ExtPhy.c
				ETHPIC32ExtPhySMSC8720.c

			Files from this project were to build out the PHY driver used with the Ethernet 
			driver from the "FreeRTOS-TCP-for-PIC32" project to complete the network interface
			functionality.
		
			url: http://www.mrc.uidaho.edu/mrc/people/jff/443/Handouts/Ethernet/PIC32/TCPIP%20Tutorials/

TESTING:     	This project has been tested only using a Digilent Cerebot MX7cK using:
	     	MPLAB X IDE 		v3.65
	     	XC32 			v1.31
	     	FreeRTOS 		v10.2.0
	     	     
WARNING:     	THIS PROJECT WAS CREATED FOR THE PURPOSES OF EDUCATIONAL USE ONLY AND IS NOT INTENDED FOR 
	     	RELAIABLE USE IN PRODUCTION SYSTEMS. 