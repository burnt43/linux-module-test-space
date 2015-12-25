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
void debug_inode(struct inode *);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Testing character device modules");

#define SUCCESS     0
#define DEVICE_NAME "chardev"
#define BUF_LEN     80

static int major;
static int device_open_count = 0;

static char msg[BUF_LEN];
static char *msg_ptr;

static char write_msg[BUF_LEN];
static char *write_ptr;

static struct file_operations fops = {
  .read    = device_read,
  .write   = device_write,
  .open    = device_open,
  .release = device_release
};

int init_module(void) {
  major = register_chrdev(0, DEVICE_NAME, &fops);

  if ( major < 0 ) {
    printk( KERN_ALERT "%s: registering failed: %d\n", DEVICE_NAME, major );
    return major;
  }

  printk(KERN_INFO "%s: major number=%d\n", DEVICE_NAME, major);
  printk(KERN_INFO "%s: run 'mknod /dev/%s c %d 0'\n",DEVICE_NAME, DEVICE_NAME, major);

  return SUCCESS;
}

void cleanup_module(void) {
  unregister_chrdev(major,DEVICE_NAME);
}

void debug_inode(struct inode *inode) {
  umode_t tmp_umode_result = 0; 
  char *tmp_message;

  printk(KERN_INFO "%s: --------------------------INODE DEBUG----------------------------\n", DEVICE_NAME);
  printk(KERN_INFO "%s: \033[0;31minode->i_mode\033[0;39m(umode_t): %d\n", DEVICE_NAME, inode->i_mode);
  tmp_umode_result = S_IFMT & inode->i_mode;
  switch ( tmp_umode_result ) {
    case S_IFSOCK:
      tmp_message = "S_IFSOCK";
      break;
    case S_IFLNK:
      tmp_message = "S_IFLNK";
      break;
    case S_IFREG:
      tmp_message = "S_IFREG";
      break;
    case S_IFBLK:
      tmp_message = "S_IFBLK";
      break;
    case S_IFDIR:
      tmp_message = "S_IFDIR";
      break;
    case S_IFCHR:
      tmp_message = "S_IFCHR";
      break;
    case S_IFIFO:
      tmp_message = "S_IFIFO";
      break;
    default:
      tmp_message = "unknown";
  }
  printk(KERN_INFO "%s: %d & %d (S_IFMT & inode->i_mode) = %d(%s)", DEVICE_NAME, S_IFMT, inode->i_mode, tmp_umode_result, tmp_message);
  printk(KERN_INFO "%s: -----------------------------------------------------------------\n", DEVICE_NAME);
}

static int device_open( struct inode *inode, struct file *file ) {
  static int counter = 0;
  debug_inode(inode);
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
  printk(KERN_INFO "%s: size_t length: %d\n", DEVICE_NAME, length);
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
  int bytes_written = 0;
  printk(KERN_INFO "%s: (buff=%s) (len=%d)\n", DEVICE_NAME, buff, len); 
  write_ptr = write_msg;
  while ( len ) {
    get_user( *(write_ptr++), buff++ );
    len--;
    bytes_written++;
  }
  *write_ptr = 0;
  printk(KERN_INFO "%s: read from user %s\n", DEVICE_NAME, write_msg);
  return bytes_written;
}
