#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");                      /* The license type -- this affects available functionality */
MODULE_AUTHOR("MadMax");                    /* The author -- visible when you use modinfo */
MODULE_DESCRIPTION("A simple ioctl lkm");   /* The description -- see modinfo */
MODULE_VERSION("0.1");                      /* A version number to inform users */

#define DRVNAME "VidIoc"
#define DRVPATH "/dev/" DRVNAME
#define NUMBER_OF_MINOR_DEVICE (0)

char msg[256];
static struct class *basic_driver_class;
static int basic_major_number = 0;
static struct cdev basic_cdev;

static long ioctl_call(struct file *filp, unsigned int uiVidCmd,
                       unsigned long arg);

static struct file_operations fops = {
    .unlocked_ioctl = ioctl_call,
};

static long ioctl_call(struct file *filp, unsigned int uiVidCmd,
                       unsigned long arg) {
  void *argp = (void *)arg;

  printk("ioctl is called\r\n");

  sprintf(msg, "ioctl lkm received: %s", (char *) argp);
  copy_to_user(argp, &msg, strlen(msg));

  printk("return from ioctl\r\n");

  return 0;
}

static void setup_cdev(struct cdev *dev, int minor,
                       struct file_operations *fops) {
  int err = -1;
  /*
   * MKDEV call creates a device number i.e.
   * combination of major and minor number
   */
  int devno = MKDEV(basic_major_number, minor);
  /* Initiliaze character dev with fops */
  cdev_init(dev, fops);
  /* owner and operations initialized */
  dev->owner = THIS_MODULE;
  dev->ops = fops;
  /* add the character device to the system */
  /*
   * Here 1 means only 1 minor number, you can give 2 for 2 minor device, the
   * last param is the count of minor number enrolled
   */
  err = cdev_add(dev, devno, 1);

  if (err) {
    printk(KERN_NOTICE "Couldn't add cdev");
  }
}

static int __init chardev_init(void) {
  int result;
  dev_t dev;

  printk("Welcome!! Device Init now..\r\n");

  /*
   * alloc_chrdev_region registering the character device
   * a major number will be dynamically allocated here
   * alloc_chrdev_region(&dev_num, FIRST_MINOR, COUNT, DEVICE_NAME);
  */
  result = alloc_chrdev_region(&dev, 0, NUMBER_OF_MINOR_DEVICE, DRVNAME);

  if (result < 0) {
    printk("Error in allocating device");
    return -1;
  }

  /* add the driver to /-sys/class/chardrv */
  if ((basic_driver_class = class_create(THIS_MODULE, "chardrv")) == NULL) {
    unregister_chrdev_region(dev, 1);
    return -1;
  }

  /* add the driver to /dev/pSeudoDrv -- here */
  if (device_create(basic_driver_class, NULL, dev, NULL, DRVNAME) == NULL) {
    class_destroy(basic_driver_class);
    unregister_chrdev_region(dev, 1);
    return -1;
  }

  /* let's see what major number was assigned by the Kernel */
  basic_major_number = MAJOR(dev);
  printk("Kernel assigned major number is %d ..\r\n", basic_major_number);

  /* Now setup the cdev */
  setup_cdev(&basic_cdev, NUMBER_OF_MINOR_DEVICE, &fops);

  return 0;
}

static void __exit chardev_exit(void) {
  /* A reverse - destroy mechansim -- the way it was created */
  printk("Releasing Simple Devs -- %s\r\n", __FUNCTION__);
  /* delete the character driver added */
  cdev_del(&basic_cdev);
  /* destroy the device created */
  device_destroy(basic_driver_class, MKDEV(basic_major_number, 0));
  /* destroy the class created */
  class_destroy(basic_driver_class);
  /* unregister the chr dev */
  unregister_chrdev(basic_major_number, NUMBER_OF_MINOR_DEVICE);
}

module_init(chardev_init);
module_exit(chardev_exit);
