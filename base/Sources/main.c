#include "system.h"
#include "base.h"

static pAppCallback_t pApp;

extern void baseInitApp();

void baseInstallApp(pAppCallback_t const pCallback)
{
    if (0 != pCallback) 
    {
        pApp = pCallback;
    }
}

void main(void)
{
	systemEnableInterrupts();
	baseInitApp();
	
	for (;;)
	{		
        if (0 != pApp) 
        {
            (*pApp)();
        }
		systemResetWatchdog();
	} /* loop forever */
	/* please make sure that you never leave main */
}
