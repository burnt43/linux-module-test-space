#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>
#include<linux/seq_file.h>

#define mod_name "proc_test"

static int my_proc_open (struct inode *, struct file *);

static struct file_operations fops = {
  .owner   = THIS_MODULE,
  .open    = my_proc_open,
  .llseek  = seq_lseek,
  .read    = seq_read,
  .release = single_release
};

static int my_proc_open (struct inode *inode, struct file *file) {
  printk(KERN_INFO "%s: open\n", mod_name);
  return 0;
}

int init_module () {
  printk(KERN_INFO "%s: starting up\n", mod_name);
  proc_create_data(mod_name, 0, NULL, &fops, NULL);
  return 0;
}

void cleanup_module () {
  printk(KERN_INFO "%s: cleaning up\n", mod_name);
  remove_proc_entry(mod_name, NULL);
}
/*
struct proc_dir_entry *our_proc_file;

int procfile_read (char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data) {
  int result;
  printk(KERN_INFO "%s: procfile_read called \n", procfs_name);
  if (offset > 0) {
    result = 0;
  } else {
    result = sprintf(buffer, "Hello, World!\n");
  }
  return result;
}

int init_module () {
  our_proc_file = create_proc_entry(procfs_name, 0644, NULL);

  if (our_proc_file == NULL) {
    remove_proc_entry(procfs_name, &proc_root);
    printk(KERN_ALERT "%s: could not create proc_entry\n", procfs_name);
    return -ENOMEM;
  }

  our_proc_file->read_proc = procfile_read;
  our_proc_file->owner     = THIS_MODULE;
  our_proc_file->mode      = S_IFREG | S_IRUGO;
  our_proc_file->uid       = 0;
  our_proc_file->gid       = 0;
  our_proc_file->size      = 37;

  printk(KERN_INFO "%s: proc_file created\n", procfs_name
  return 0;
}

void cleanup_module () {
  remove_proc_entry(procfs_name, &proc_root);
  printk(KERN_INFO "%s: proc_file removed\n", procfs_name);
}
*/
