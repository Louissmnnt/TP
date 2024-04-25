/*
 * FreeRTOS V202107.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/******************************************************************************
 * NOTE 1: The FreeRTOS demo threads will not be running continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Linux port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Linux
 * port for further information:
 * https://freertos.org/FreeRTOS-simulator-for-Linux.html
 *
 * NOTE 2:  This project provides two demo applications.  A simple blinky style
 * project, and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting in main.c is used to select
 * between the two.  See the notes on using mainCREATE_SIMPLE_BLINKY_DEMO_ONLY
 * in main.c.  This file implements the simply blinky version.  Console output
 * is used in place of the normal LED toggling.
 *
 * NOTE 3:  This file only contains the source code that is specific to the
 * basic demo.  Generic functions, such FreeRTOS hook functions, are defined
 * in main.c.
 ******************************************************************************
 *
 * main_blinky() creates one queue, one software timer, and two tasks.  It then
 * starts the scheduler.
 *
 * The Queue Send Task:
 * The queue send task is implemented by the prvQueueSendTask() function in
 * this file.  It uses vTaskDelayUntil() to create a periodic task that sends
 * the value 100 to the queue every 200 milliseconds (please read the notes
 * above regarding the accuracy of timing under Linux).
 *
 * The Queue Send Software Timer:
 * The timer is an auto-reload timer with a period of two seconds.  The timer's
 * callback function writes the value 200 to the queue.  The callback function
 * is implemented by prvQueueSendTimerCallback() within this file.
 *
 * The Queue Receive Task:
 * The queue receive task is implemented by the prvQueueReceiveTask() function
 * in this file.  prvQueueReceiveTask() waits for data to arrive on the queue.
 * When data is received, the task checks the value of the data, then outputs a
 * message to indicate if the data came from the queue send task or the queue
 * send software timer.
 *
 * Expected Behaviour:
 * - The queue send task writes to the queue every 200ms, so every 200ms the
 *   queue receive task will output a message indicating that data was received
 *   on the queue from the queue send task.
 * - The queue send software timer has a period of two seconds, and is reset
 *   each time a key is pressed.  So if two seconds expire without a key being
 *   pressed then the queue receive task will output a message indicating that
 *   data was received on the queue from the queue send software timer.
 *
 * NOTE:  Console input and output relies on Linux system calls, which can
 * interfere with the execution of the FreeRTOS Linux port. This demo only
 * uses Linux system call occasionally. Heavier use of Linux system calls
 * may crash the port.
 */
#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"
/*Size of the list for a task*/
#define SIZE 50

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 5 )

#define mainQUEUE_SEND_TASK1_PRIORITY       ( tskIDLE_PRIORITY + 1 )
#define mainQUEUE_SEND_TASK2_PRIORITY       ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK3_PRIORITY       ( tskIDLE_PRIORITY + 3 )
#define mainQUEUE_SEND_TASK4_PRIORITY       ( tskIDLE_PRIORITY + 4 )


/* The rate at which data is sent to the queue.  The times are converted from
 * milliseconds to ticks using the pdMS_TO_TICKS() macro. */
#define mainTASK1_SEND_FREQUENCY_MS         pdMS_TO_TICKS( 18UL )
#define mainTASK2_SEND_FREQUENCY_MS         pdMS_TO_TICKS( 60UL )
#define mainTASK3_SEND_FREQUENCY_MS         pdMS_TO_TICKS( 36UL )
#define mainTASK4_SEND_FREQUENCY_MS         pdMS_TO_TICKS( 12UL )
#define mainTIMER_SEND_FREQUENCY_MS         pdMS_TO_TICKS( 18000UL )

/* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH                   ( 5 )

/* The values sent to the queue receive task from the queue send task and the
 * queue send software timer respectively. */
#define mainVALUE_SENT_FROM_TASK1           ( 100UL )
#define mainVALUE_SENT_FROM_TASK2           ( 200UL )
#define mainVALUE_SENT_FROM_TASK3           ( 300UL )
#define mainVALUE_SENT_FROM_TASK4           ( 400UL )
#define mainVALUE_SENT_FROM_TIMER           ( 10UL )

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void * pvParameters );

static void prvQueueSendTask1( void * pvParameters );
static void prvQueueSendTask2( void * pvParameters );
static void prvQueueSendTask3( void * pvParameters );
static void prvQueueSendTask4( void * pvParameters );

/*
 * The callback function executed when the software timer expires.
 */
static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle );
/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/* A software timer that is started from the tick hook. */
static TimerHandle_t xTimer = NULL;

/*-----------------------------------------------------------*/
void ipsa_sched( void )
{
    const TickType_t xTimerPeriod = mainTIMER_SEND_FREQUENCY_MS;

    /* Create the queue. */
    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

    if( xQueue != NULL )
    {
        /* Start the two tasks as described in the comments at the top of this file. */
        xTaskCreate( prvQueueReceiveTask,"Rx", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_RECEIVE_TASK_PRIORITY, NULL );
        xTaskCreate( prvQueueSendTask1, "T1X", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK1_PRIORITY, NULL );
        xTaskCreate( prvQueueSendTask2, "T2X", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK2_PRIORITY, NULL );
        xTaskCreate( prvQueueSendTask3, "T3X", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK3_PRIORITY, NULL );
        xTaskCreate( prvQueueSendTask4, "T4X", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK4_PRIORITY, NULL );
        /* Create the software timer, but don't start it yet. */
        xTimer = xTimerCreate( "Timer", xTimerPeriod, pdTRUE, NULL, prvQueueSendTimerCallback ); 

        if( xTimer != NULL )
        {
            xTimerStart( xTimer, 0 );
        }
        vTaskStartScheduler();
    }
    for( ; ; )
    {
    }
}

/*------Timer clock-------------------------------------*/

static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle )
{
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TIMER;
    ( void ) xTimerHandle;
    xQueueSend( xQueue, &ulValueToSend, 0U );
}
/*-----Fonction pour chaque tache------------------------------------------*/

/*TACHE1*/
static void prvQueueSendTask1( void * pvParameters )
{
    TickType_t xNextWakeTime;
    const TickType_t xBlockTime = mainTASK1_SEND_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK1;
    ( void ) pvParameters;
    xNextWakeTime = xTaskGetTickCount();
    for( ; ; )
    {
        vTaskDelayUntil( &xNextWakeTime, xBlockTime );
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}
/*TACHE2*/
static void prvQueueSendTask2( void * pvParameters )
{
    TickType_t xNextWakeTime;
    const TickType_t xBlockTime = mainTASK2_SEND_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK2;
    ( void ) pvParameters;
    xNextWakeTime = xTaskGetTickCount();
    for( ; ; )
    {
        vTaskDelayUntil( &xNextWakeTime, xBlockTime );
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}

/*TACHE3*/
static void prvQueueSendTask3( void * pvParameters )
{
    TickType_t xNextWakeTime;
    const TickType_t xBlockTime = mainTASK3_SEND_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK3;
    ( void ) pvParameters;
    xNextWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
        vTaskDelayUntil( &xNextWakeTime, xBlockTime );
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}

/*TACHE4*/
static void prvQueueSendTask4( void * pvParameters )
{
    TickType_t xNextWakeTime;
    const TickType_t xBlockTime = mainTASK4_SEND_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK4;
    ( void ) pvParameters;
    xNextWakeTime = xTaskGetTickCount();
    for( ; ; )
    {
        vTaskDelayUntil( &xNextWakeTime, xBlockTime );
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}

/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void * pvParameters )
{
    uint32_t ulReceivedValue;
    ( void ) pvParameters;
    for( ; ; )
    {
        /* Blocked state. */
        xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );
        if( ulReceivedValue == mainVALUE_SENT_FROM_TASK1)  //Tache 1 executée si val1 recu dans la file d'attente
        {
            console_print( "Working\n" );
        }
        else if( ulReceivedValue == mainVALUE_SENT_FROM_TASK2)//Tache 2 executée si val2 recu dans la file d'attente
        {
            double celsius = 15.0;
            double fahrenheit;
            fahrenheit = (celsius * 9.0 / 5.0) + 32.0;
            console_print("%.2f Fahrenheit\n", fahrenheit);
    
        }else if( ulReceivedValue == mainVALUE_SENT_FROM_TASK3)//Tache 3 executée si val3 recu dans la file d'attente
        {   
            long int num1 = 1234567890;
            long int num2 = 9876543210;
            long int result;
            result = num1 * num2;
            console_print("Result of the multiplication of is : %ld \n",result);
    
        }else if( ulReceivedValue == mainVALUE_SENT_FROM_TASK4)//Tache 4 executée si val4 recu dans la file d'attente
        {
            
            // Crée une liste de 50 éléments (nombres de 1 à 50)
            int arr[SIZE];
            for (int i = 0; i < SIZE; i++) {
                arr[i] = i + 1;
            }

            // Élément à rechercher
            int key = 25;

            // Fonction de recherche binaire
            int low = 0, high = SIZE - 1, mid;
            int found = 0;
            while (low <= high) {
                mid = low + (high - low) / 2;
                if (arr[mid] == key) {
                    found = 1;
                    break;
                } else if (arr[mid] < key) {
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            console_print("L'élément %d a été trouvé à l'index %d\n", key, mid);

        }else if( ulReceivedValue == mainVALUE_SENT_FROM_TIMER)
        {
            console_print( "HyperPeriod reached \n" );
        }
        else 
        {
            console_print( "Unexpected message\n" );
        }
    }
}
/*-----------------------------------------------------------*/
