#include<linux/init.h>
#include<linux/module.h>
#include<linux/version.h>
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init (void) {
  printk(KERN_ALERT "Hello, World! kernel version: %d.%d.%d\n",
    (LINUX_VERSION_CODE & 0XFF0000) >> 16,
    (LINUX_VERSION_CODE & 0x00FF00) >> 8,
    (LINUX_VERSION_CODE & 0x0000FF)
  );
  return 0;
}

static int hello_exit(void) {
  printk(KERN_ALERT "Goodbye!\n");
  return 0;
}

module_init(hello_init);
module_exit(hello_exit);
