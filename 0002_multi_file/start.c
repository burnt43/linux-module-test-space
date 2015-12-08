#include<linux/kernel.h>
#include<linux/module.h>
MODULE_LICENSE("GPL");

int init_module(void) {
  printk(KERN_ALERT "INIT");
  return 0;
}
