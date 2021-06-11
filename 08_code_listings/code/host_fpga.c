#include <stdlib.h>
#include <sys/alt_stdio.h>
#include <sys/alt_alarm.h>
#include <sys/times.h>
#include <system.h>
#include <unistd.h>
#include <math.h>

#define ALT_CI_SYSNAIM_HOST_0_N 0x0
#define ALT_CI_SYSNAIM_HOST_0(A,B) __builtin_custom_inii(ALT_CI_SYSNAIM_HOST_0_N,(A),(B))

int main()
{ 
	int out;
	alt_printf("Starting Execution\n");
	out = ALT_CI_SYSNAIM_HOST_0(2000,6050);
	alt_printf("Output from instr: ");
	alt_printf("%x\n", out);
	return 0;
}
