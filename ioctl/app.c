#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>


int main()
{
	
	int fd = open("/dev/dummy",O_RDWR);
	if(fd < 0)
	{
		printf("open: %s\n",strerror(errno));
		return -1;
	}
	if(ioctl(fd, 1, NULL) < 0){
		printf("ioctl : %s\n",strerror(errno));
		return -1;
	}

	printf("ioctl success\n");
	return 0;
}