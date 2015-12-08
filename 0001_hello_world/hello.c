#include<linux/init.h>
#include<linux/module.h>
#include<linux/version.h>
#include<linux/moduleparam.h>
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Hello World");
MODULE_AUTHOR("James Carson <jms.crsn@gmail.com>");
MODULE_SUPPORTED_DEVICE("foobar");

static char *whom  = "world";
static int howmany = 1;
static int myarray[2] = {0, 0};

module_param(howmany, int, 0);
MODULE_PARM_DESC(howmany, "print whom this many times");

module_param(whom, charp, 0);
MODULE_PARM_DESC(whom, "name to print");

module_param_array(myarray,int,NULL,0);
MODULE_PARM_DESC(myarray, "values to print");

static int __init hello_init (void) {
  int i;
  printk(KERN_ALERT "kernel version: %d.%d.%d\n",
    (LINUX_VERSION_CODE & 0XFF0000) >> 16,
    (LINUX_VERSION_CODE & 0x00FF00) >> 8,
    (LINUX_VERSION_CODE & 0x0000FF)
  );
  for (i = 0; i < howmany; i++) {
    printk(KERN_ALERT "Hello, %s\n",whom);
  }
  printk(KERN_ALERT "myarray[0]: %d",myarray[0]);
  printk(KERN_ALERT "myarray[1]: %d",myarray[1]);
  return 0;
}

static int __exit hello_exit(void) {
  printk(KERN_ALERT "Goodbye!\n");
  return 0;
}

module_init(hello_init);
module_exit(hello_exit);
