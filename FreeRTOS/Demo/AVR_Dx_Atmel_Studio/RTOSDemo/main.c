/*
(C) 2020 Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software and
any derivatives exclusively with Microchip products. It is your responsibility
to comply with third party license terms applicable to your use of third party
software (including open source software) that may accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS,
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES
OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER
RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF
THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED
BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY
TO MICROCHIP FOR THIS SOFTWARE.
*/

#include <avr/io.h>
#include "FreeRTOS.h"
#include "TaskNotify.h"
#include "task.h"
#include "clk_config.h"

#define mainSELECTED_APPLICATION	0

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

#if ( mainSELECTED_APPLICATION == 0 )
extern void main_blinky( void );
extern void init_blinky( void );
#elif ( mainSELECTED_APPLICATION == 1 )
extern void main_minimal( void );
extern void init_minimal( void );
#elif ( mainSELECTED_APPLICATION == 2 )
extern void main_full( void );
extern void init_full( void );
#else
#error Invalid mainSELECTED_APPLICATION setting. See the comments at the top of this file and above the mainSELECTED_APPLICATION definition.
#endif

int main(void)
{
	prvSetupHardware();

#if ( mainSELECTED_APPLICATION == 0 )
	main_blinky();
#elif ( mainSELECTED_APPLICATION == 1 )
	main_minimal();
#elif ( mainSELECTED_APPLICATION == 2 )
	main_full();
#endif
	
	return 0;
}

static void prvSetupHardware( void )
{
	/* Ensure no interrupts execute while the scheduler is in an inconsistent
	state.  Interrupts are automatically enabled when the scheduler is
	started. */
	portDISABLE_INTERRUPTS();

	CLK_init();

#if ( mainSELECTED_APPLICATION == 0 )
	init_blinky();
#elif ( mainSELECTED_APPLICATION == 1 )
	init_minimal();
#elif ( mainSELECTED_APPLICATION == 2 )
	init_full();
#endif
}

void vApplicationTickHook( void )
{
#if( mainSELECTED_APPLICATION == 2 )
	xNotifyTaskFromISR();
#endif
}

void vApplicationStackOverflowHook(TaskHandle_t *pxTask, signed char *pcTaskName )
{
	asm volatile ("nop");
	for( ;; );
}

void vApplicationMallocFailedHook( void )
{
	asm volatile ("nop");
	for( ;; );
}