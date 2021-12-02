#include <errno.h>
#include <stddef.h>
#include <stdio.h>

#include <IOKit/IOKitLib.h>
#include <mach/mach.h>
#include <mach/message.h>
#include <servers/bootstrap.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <unistd.h>

mach_port_t syz_open_service(const char* name, uint32_t type)
{
	io_service_t gpu = IO_OBJECT_NULL;
	io_connect_t connect = IO_OBJECT_NULL;
	IOReturn ioret;

	gpu = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("IOGPU"));
	if (gpu == IO_OBJECT_NULL) {
		fprintf(stderr, "client: unable to find IOGPU service\n");
		exit(1);
	}

	uint32_t open_type = 5;
	ioret = IOServiceOpen(gpu, mach_task_self(), open_type, &connect);
	if (ioret != kIOReturnSuccess) {
		fprintf(stderr, "client: unable to open user client: 0x%x\n", ioret);
		exit(1);
	}
}

kern_return_t syz_io_connect_call_method(mach_port_t connection, uint32_t selector, const uint64_t* input, uint32_t inputCnt, const void* inputStruct, size_t inputStructCnt, uint64_t* output, uint32_t* outputCnt, void* outputStruct, size_t* outputStructCnt)
{
	kern_return_t ret = 0;
	ret = IOConnectCallMethod(connection, selector, input, inputCnt, inputStruct, inputStructCnt, output, outputCnt, outputStruct, outputStructCnt);
	return ret;
}