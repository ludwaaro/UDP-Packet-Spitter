/*  FreeRTOS + TCP Reference Design UDP Packet Spitter
 * Project:     UDP Packet Spitter
 * File name:   main.c
 * Author:      Aaron Ludwig
 * Date:        November 14, 2019
 *
 * Description: An example of a project utilizing FreeRTOS's support for
 *              UDP communication functionality in the FreeRTOS + TCP library
 *              running on a chipKIT_Pro_MX7 running the PIC32MX795F512L processor.
 *              The operating system creates and manages a task that sends UDP 
 *              packets with predefined contents to a predefined destination 
 *              address at regular intervals.
 *
 * Observable:  Using the Wireshark program while this program runs should
 *              show UDP packets containing the message "Jimbo" sent from the
 *              defined sourceAddress to the defined destinationAddress.
 * 
 * NOTES:       This file is an edited version of the RD1.c file originally created
 *              by Richard Wall in the FreeRTOS RD1 project.
*******************************************************************************/

#define sourceAddress           { 129, 101, 222, 34 }
#define destinationAddress      "129.101.222.131" 
#define UDPContents             "Jimbo"

/* Standard includes. */
#include <plib.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Hardware dependent setting */
#include "chipKIT_Pro_MX7.h"
#include "FreeRTOS.h"
#include "FreeRTOS_IP_Private.h"

/* Set up the processor for the example program. */
static void prvSetupHardware( void );

/* Simple Task that constantly sends UDP Packets to a predefined address with a delay between each sent packet*/
static void vUDPSendUsingStandardInterface( void *pvParameters );

/* The MAC address array is not declared const as the MAC address will
normally be read from an EEPROM and not hard coded (in real deployed
applications). In this case the MAC Address is hard coded to the value we
have for the Cerebot board we're using. */
static uint8_t ucMACAddress[ 6 ] = { 0x00, 0x04, 0xA3, 0x17, 0xCB, 0xF8 };

/* Set this value to be the IP address you want to use (you are recommended 
 * to use the value given with your Cerebot station but the value doesn't seem
 * to be strictly limited to that).*/
static const uint8_t ucIPAddress[ 4 ] = sourceAddress;

/* You shouldn't need to worry about these values as we're using DHCP. */
static const uint8_t ucNetMask[ 4 ] = { 255, 255, 252, 0 };
static const uint8_t ucGatewayAddress[ 4 ] = { 129, 101, 220, 1 };
/* The following is the address of an OpenDNS server. */
static const uint8_t ucDNSServerAddress[ 4 ] = { 0, 0, 0, 0 };

/* main Function Description ***************************************************
 * SYNTAX:		    int main( void );
 * KEYWORDS:		Initialize, create, tasks, scheduler
 * DESCRIPTION:     This is a typical RTOS set up function. Hardware is
 *                  initialized, tasks are created, and the scheduler is
 *                  started.
 * PARAMETERS:		None
 * RETURN VALUE:	Exit code - used for error handling
 * END DESCRIPTION ************************************************************/
int main( void )
{
    prvSetupHardware();
    /* Initialise the RTOS's TCP/IP stack.  The tasks that use the network
    are created in the vApplicationIPNetworkEventHook() hook function
    below.  The hook function is called when the network connects. */
    FreeRTOS_IPInit( ucIPAddress,
                     ucNetMask,
                     ucGatewayAddress,
                     ucDNSServerAddress,
                     ucMACAddress );

    /*
     * Our RTOS tasks can be created here.
     */
    xTaskCreate( vUDPSendUsingStandardInterface, "UDP1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
    
    /* Start the RTOS scheduler. */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running. */
    for( ;; );
    
    /* Will only reach here if there is insufficient heap available to start
     *  the scheduler. */
    return 0;
}  /* End of main */

/* prvSetupHardware Function Description ***************************************
 * SYNTAX:	static void prvSetupHardware( void );
 * KEYWORDS:	Hardware, initialize, configure, setup
 * DESCRIPTION: Initializes hardware specific resources.
 * PARAMETERS:	None
 * RETURN VALUE: None
 * NOTES:	Static function - can be called exclusively from 
 * 		within this program.
 * END DESCRIPTION ************************************************************/
static void prvSetupHardware( void )
{
    chipKIT_PRO_MX7_Setup();

/* Set up PmodSTEM LEDs */
    PORTSetPinsDigitalOut(IOPORT_B, SM_LEDS);
    LATBCLR = SM_LEDS;                      /* Clear all SM LED bits */
    
/* Enable chipKIT Pro MX7 and Cerebot 32MX7ck PHY 
 * (this is essential for using the PHY chip)*/
    TRISACLR = (unsigned int) BIT_6; // Make bit output
    LATASET = (unsigned int) BIT_6;	 // Set output high
    
/* Enable multi-vector interrupts */
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);  /* Do only once */
    INTEnableInterrupts();   /*Do as needed for global interrupt control */
    portDISABLE_INTERRUPTS();
} /* End of prvSetupHardware */

/* vApplicationStackOver Function Description **********************************
 * SYNTAX:          void vApplicationStackOverflowHook( void );
 * KEYWORDS:        Stack, overflow
 * DESCRIPTION:     Look at pxCurrentTCB to see which task overflowed
 *                  its stack.
 * PARAMETERS:      None
 * RETURN VALUE:    None
 * NOTES:           See FreeRTOS documentation
 * END DESCRIPTION ************************************************************/
void vApplicationStackOverflowHook( void )
{
	for( ;; );
} /* End of vApplicationStackOver */

/* _general_exception_handler Function Description *****************************
 * SYNTAX:          void _general_exception_handler( unsigned long ulCause,
 *                                              unsigned long ulStatus );
 * KEYWORDS:        Exception, handler
 * DESCRIPTION:     This overrides the definition provided by the kernel.
 *                  Other exceptions should be handled here. Set a breakpoint
 *                  on the "for( ;; )" to catch problems.
 * PARAMETER 1:     unsigned long - Cause of exception code
 * PARAMETER 2:     unsigned long - status of process
 * RETURN VALUE:    None
 * NOTES:           Program will be vectored to here if the any CPU error is
 *                  generated. See FreeRTOS documentation for error codes.
END DESCRIPTION ***************************************************************/
void _general_exception_handler( unsigned long ulCause, unsigned long ulStatus )
{
    for( ;; );
} /* End of _general_exception_handler */

/* vUDPSendUsingStandardInterface Function Description *************************
 * SYNTAX:          static void vUDPSendUsingStandardInterface( void *pvParameters );
 * KEYWORDS:        RTOS, Task
 * DESCRIPTION:     Sends UDP packets with predefined contents to a 
 *                  predefined destination address at regular intervals.
 * PARAMETER 1:     void pointer - data of unspecified data type sent from
 *                  RTOS scheduler
 * RETURN VALUE:    None (There is no returning from this function)
 * NOTES:           This function is a slightly altered form of the one used in
 *                  the FreeRTOS + TCP tutorial on sending UDP Data which can 
 *                  be found at the following url:
 *                  https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/Networking_Tutorial_Sending_UDP_Data.html    
 * END DESCRIPTION ************************************************************/
static void vUDPSendUsingStandardInterface( void *pvParameters )
{
    Socket_t xSocket;
    struct freertos_sockaddr xDestinationAddress;
    uint8_t cString[ 50 ];
    uint32_t ulCount = 0UL;
    const TickType_t x1000ms = 1000UL / portTICK_PERIOD_MS;
    const TickType_t xSendTimeOut = 200 / portTICK_PERIOD_MS;
    
    /* Send strings to port 1900 on IP address 129.101.222.131. */
    xDestinationAddress.sin_addr = FreeRTOS_inet_addr( destinationAddress );
    xDestinationAddress.sin_port = FreeRTOS_htons( 1900 );

    /* Create the socket. */
    xSocket = FreeRTOS_socket( FREERTOS_AF_INET,
                              FREERTOS_SOCK_DGRAM,/*FREERTOS_SOCK_DGRAM for UDP.*/
                              FREERTOS_IPPROTO_UDP );
    
    /* Check the socket was created. */
    configASSERT( xSocket != FREERTOS_INVALID_SOCKET );

    FreeRTOS_setsockopt( xSocket,
                        0,
                        FREERTOS_SO_SNDTIMEO,
                        &xSendTimeOut,
                        sizeof( xSendTimeOut ) );
    
    /* NOTE: FreeRTOS_bind() is not called.  This will only work if
    ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND is set to 1 in FreeRTOSIPConfig.h. */
    
    for( ;; )
    {
        /* Create the string that is sent. */
        sprintf( cString, UDPContents, ulCount );
        
        /* Send the string to the UDP socket.  ulFlags is set to 0, so the standard
        semantics are used.  That means the data from cString[] is copied
        into a network buffer inside FreeRTOS_sendto(), and cString[] can be
        reused as soon as FreeRTOS_sendto() has returned. */
        int32_t answer;
        answer = FreeRTOS_sendto( xSocket,
                        cString,
                        strlen( cString ),
                        0,
                        &xDestinationAddress,
                        sizeof( xDestinationAddress ) );
        
        ulCount++;

        /* Wait until it is time to send again. */
        vTaskDelay( x1000ms );
    }
}

/* ulApplicationGetNextSequenceNumber Function Description *********************
 * SYNTAX:          uint32_t ulApplicationGetNextSequenceNumber
 *                                              ( uint32_t ulSourceAddress,
 *												uint16_t usSourcePort,
 *												uint32_t ulDestinationAddress,
 *												uint16_t usDestinationPort );
 * DESCRIPTION:     Callback that provides the inputs necessary to generate a 
 *                  randomized TCP Initial Sequence Number per RFC 6528.  THIS 
 *                  IS ONLY A DUMMY IMPLEMENTATION THAT RETURNS A PSEUDO RANDOM 
 *                  NUMBER SO IS NOT INTENDED FOR USE IN PRODUCTION SYSTEMS.
 * PARAMETER 1:     uint32_t - IP source address
 * PARAMETER 2:     uint32_t - IP source address port
 * PARAMETER 3:     uint32_t - IP destination address
 * PARAMETER 4:     uint32_t - IP destination address port
 * RETURN VALUE:    A randomized TCP Initial Sequence Number.
 * NOTES:           This function came from a project on GitHub from user
 *                  jjr-simiatec called "FreeRTOS-TCP-for-PIC32" using commit
 *                  11bee9b5f2a5a21b6311feca873286e4c7be3534 from the master
 *                  branch. This project can be found at the following url:
 *                  https://github.com/jjr-simiatec/FreeRTOS-TCP-for-PIC32/commit/11bee9b5f2a5a21b6311feca873286e4c7be3534
END DESCRIPTION ************************************************************/
extern uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
													uint16_t usSourcePort,
													uint32_t ulDestinationAddress,
													uint16_t usDestinationPort )
{
	( void ) ulSourceAddress;
	( void ) usSourcePort;
	( void ) ulDestinationAddress;
	( void ) usDestinationPort;

	return uxRand();
}

/* uxRand Function Description *********************************************
 * SYNTAX:          UBaseType_t uxRand( void );
 * DESCRIPTION:     Function called by the IP stack to generate random numbers for
 *                  things such as a DHCP transaction number or initial sequence number.
 * RETURN VALUE:    A pseudo random number.
 * NOTES:           This function came from a project on GitHub from user
 *                  jjr-simiatec called "FreeRTOS-TCP-for-PIC32" using commit
 *                  11bee9b5f2a5a21b6311feca873286e4c7be3534 from the master
 *                  branch. This project can be found at the following url:
 *                  https://github.com/jjr-simiatec/FreeRTOS-TCP-for-PIC32/commit/11bee9b5f2a5a21b6311feca873286e4c7be3534
END DESCRIPTION ************************************************************/
UBaseType_t uxRand( void )
{
    static UBaseType_t ulNextRand;
    const uint32_t ulMultiplier = 0x015a4e35UL, ulIncrement = 1UL;

	/* Utility function to generate a pseudo random number. */

	ulNextRand = ( ulMultiplier * ulNextRand ) + ulIncrement;
	return( ( int ) ( ulNextRand >> 16UL ) & 0x7fffUL );
}
/*--------------------------End of main.c  -----------------------------------*/

