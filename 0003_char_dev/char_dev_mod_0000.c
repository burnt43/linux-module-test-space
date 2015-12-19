#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<asm/uaccess.h>

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS     0
#define DEVICE_NAME "chardev"
#define BUF_LEN     80

static int major;
static int device_open_count = 0;

static char msg[BUF_LEN];
static char *msg_ptr;

static struct file_operations fops = {
  .read    = device_read,
  .write   = device_write,
  .open    = device_open,
  .release = device_release
};

int init_module(void) {
  major = register_chrdev(0, DEVICE_NAME, &fops);

  if ( major < 0 ) {
    printk( KERN_ALERT "registering failed: %d\n", major );
    return major;
  }

  printk( KERN_INFO "Major Number: %d\n", major );
  printk( KERN_INFO "mknod /dev/%s c %d 0\n", DEVICE_NAME, major );

  return SUCCESS;
}

void cleanup_module(void) {
  unregister_chrdev(major,DEVICE_NAME);
}

static int device_open( struct inode *inode, struct file *file ) {
  static int counter = 0;
  if (device_open_count)
    return -EBUSY;

  device_open_count++;
  sprintf(msg, "%d\n", counter++);
  msg_ptr = msg;
  try_module_get(THIS_MODULE);

  return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file) {
  device_open_count--;
  module_put(THIS_MODULE);

  return SUCCESS;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
  int bytes_read = 0;
  if (*msg_ptr == 0)
    return 0;

  while (length && *msg_ptr) {
    put_user(*(msg_ptr++), buffer++);
    length--;
    bytes_read++;
  }

  return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
  printk(KERN_ALERT "not supported\n");
  return -EINVAL;
}
