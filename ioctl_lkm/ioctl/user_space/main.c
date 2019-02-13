#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>

char msg[256];

/*
 * The combination is made keeping in mind the unique IOC CMD otherwise other
 * device may do something
 */
#define AV_VID_DRIVER_TYPE 'V'
#define AV_VID_IOC_CMD _IOW(AV_VID_DRIVER_TYPE, 1, msg)
#define DRVNAME "VidIoc"
#define DRVPATH "/dev/" DRVNAME

int main() {
  int ret = 0;
  int fd = -1;
  strcpy(msg, "Hi");
  if ((fd = open(DRVPATH, O_RDONLY)) <= 0) {
    printf("Device Open failed!!\r\n");
    return -1;
  }

  printf("Sending hi to device\r\n");
  ret = ioctl(fd, AV_VID_IOC_CMD, &msg);
  if (ret != 0) {
    printf("Error in IOCTL!\r\n");
  }

  printf("The result is the following == %s ==\r\n", msg);
  return 0;
}
