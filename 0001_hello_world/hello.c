#include<linux/init.h>
#include<linux/module.h>
#include<linux/version.h>
#include<linux/moduleparam.h>
MODULE_LICENSE("Dual BSD/GPL");

static char *whom = "world";
static int howmany = 1;

module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);

static int hello_init (void) {
  int i;
  printk(KERN_ALERT "kernel version: %d.%d.%d\n",
    (LINUX_VERSION_CODE & 0XFF0000) >> 16,
    (LINUX_VERSION_CODE & 0x00FF00) >> 8,
    (LINUX_VERSION_CODE & 0x0000FF)
  );
  for (i = 0; i < howmany; i++) {
    printk(KERN_ALERT "Hello, %s\n",whom);
  }
  return 0;
}

static int hello_exit(void) {
  printk(KERN_ALERT "Goodbye!\n");
  return 0;
}

module_init(hello_init);
module_exit(hello_exit);
