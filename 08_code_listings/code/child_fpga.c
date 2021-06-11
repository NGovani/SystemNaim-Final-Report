#include <stdlib.h>
#include <sys/alt_stdio.h>
#include <sys/alt_alarm.h>
#include <sys/times.h>
#include <system.h>
#include <unistd.h>
#include <math.h>

#define ALT_CI_HLS_REMOTE_FUNC_1_0_N 0x0
#define ALT_CI_HLS_REMOTE_FUNC_1_0() __builtin_custom_in(ALT_CI_HLS_REMOTE_FUNC_1_0_N)

int main()
{ 
	int out;
	alt_printf("Starting Execution\n");
	out =  ALT_CI_HLS_REMOTE_FUNC_1_0();
	alt_printf("Output from instr: %x", out);
	return 0;
}
