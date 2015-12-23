#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>
#include<asm/uaccess.h>

#define PROC_FILENAME "proc_test"
#define BUF_LEN 80

MODULE_LICENSE("GPL");

static char msg[BUF_LEN];
static char *msg_ptr;

static int my_proc_open (struct inode *, struct file *);
static ssize_t my_proc_read (struct file *, char *, size_t, loff_t);

static struct file_operations fops = {
  .open = my_proc_open,
  .read = my_proc_read
};

static int my_proc_open (struct inode* inode, struct file *filp) {
  sprintf(msg, "hello, world!\n");
  msg_ptr = msg;
  return 0;
}

static ssize_t my_proc_read (struct file *filp, char *buffer, size_t length, loff_t offset) {
  int bytes_read = 0;
  while ( length && *msg_ptr ) {
    put_user( *(msg_ptr++), buffer++ );
    length--;
    bytes_read++;
  }
  return bytes_read;
}

int init_module () {
  proc_create(PROC_FILENAME, 0, NULL, &fops);
  return 0;
}

void cleanup_module () {
  remove_proc_entry(PROC_FILENAME, NULL);
}
