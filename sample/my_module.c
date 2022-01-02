#include "my_module.h"

#include <linux/fs.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <asm/uaccess.h>

MODULE_DESCRIPTION("My example module");
MODULE_AUTHOR("MDC");
MODULE_LICENSE("GPL");

static int my_init(void);
static void my_exit(void);
static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);
static long device_ioctl(struct file*, unsigned int, unsigned long);

module_init(my_init);
module_exit(my_exit);

static struct file_operations Fops = {
  .owner = THIS_MODULE,
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release,
  .unlocked_ioctl = device_ioctl
};

static int Major;
static YENET_HOST_IF_REGS* Ptr;

static int my_init(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &Fops);

    if (Major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);

    return 0;
}

static void my_exit(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
    pr_debug("My Module is unloaded!\n");
}

static int device_open(struct inode* node, struct file* file_ptr)
{
    if (!request_mem_region(ISA_CARD_ADDR, ISA_SEGMENT_LEN, DEVICE_NAME)) return -EADDRINUSE;
    if (!(Ptr = (YENET_HOST_IF_REGS*)ioremap_nocache(ISA_CARD_ADDR, ISA_SEGMENT_LEN))) return -EADDRNOTAVAIL;
    
    return 0;
}

static int device_release(struct inode* node, struct file* file_ptr)
{
    iounmap(Ptr);
    release_mem_region(ISA_CARD_ADDR, ISA_SEGMENT_LEN);
    return 0;
}

static ssize_t device_read(struct file* filp, /* see include/linux/fs.h   */
    char* buffer,      /* buffer to fill with data */
    size_t length,     /* length of the buffer     */
    loff_t* offset)
{
    if (length <= 0) return 0;
    if ((*offset + length) > sizeof(YENET_HOST_IF_REGS))
    if (copy_to_user(buffer, Ptr + *offset, length) > 0) return -EFAULT;

    return 0;
}

static ssize_t device_write(struct file* filp, const char* buf, size_t len, loff_t* off)
{
    printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
    return -EINVAL;
}

static long device_ioctl(struct file* file_ptr, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
    case IOCTL_RESET:
        ynb_set_cmd(Ptr, YN_CMD_RST_CER);
        break;
    case IOCTL_GET_STATUS:
    {
        uint16_t s = ynb_get_sts(Ptr);
        if (copy_to_user((uint16_t*)arg, &s, sizeof(s))) return -EACCES;
        break;
    }
    default:
        return -EINVAL;
    }

    return 0;
}
