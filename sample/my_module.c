#include "my_module.h"

#include <linux/fs.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/stat.h>

MODULE_DESCRIPTION("My example module");
MODULE_AUTHOR("MDC2012");
MODULE_LICENSE("GPL");

#define my_print(str) pr_debug(MY_DEVICE_NAME ": %s\n", str)

static int my_init(void);
static void my_exit(void);
static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);
static long device_ioctl(struct file*, unsigned int, unsigned long);
static char* get_devnode(struct device*, umode_t*);

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
static struct class* Cls;
static dev_t Dev;
static struct cdev Cdev;

static YENET_HOST_IF_REGS* Ptr;

static char* get_devnode(struct device* dev, umode_t* mode)
{
    if (mode) *mode = S_IRWXU | S_IRGRP | S_IROTH;
    return NULL;
}

static int my_init(void)
{
    if (alloc_chrdev_region(&Dev, 0, 1, MY_DEVICE_NAME) < 0) return -EFAULT;

    if ((Cls = class_create(Fops.owner, MY_DEVICE_NAME "_drv")) == NULL)
    {
        pr_alert(MY_DEVICE_NAME ": Registering class failed.\n");
        unregister_chrdev_region(Dev, 1);
        return -EFAULT;
    }
    Cls->devnode = get_devnode;

    if (device_create(Cls, NULL, Dev, NULL, MY_DEV_FILE_NAME) == NULL)
    {
        pr_alert(MY_DEVICE_NAME ": Failed to create device file.\n");
        class_destroy(Cls);
        unregister_chrdev_region(Dev, 1);
        return -EFAULT;
    }
    
    cdev_init(&Cdev, &Fops);
    if (cdev_add(&Cdev, Dev, 1) < 0)
    {
        device_destroy(Cls, Dev);
        class_destroy(Cls);
        unregister_chrdev_region(Dev, 1);
        return -EFAULT;
    }

    pr_info(MY_DEVICE_NAME ": I was assigned major number %d. I created /dev/%s to talk to you.\n", 
        MAJOR(Dev), MY_DEV_FILE_NAME);

    /*pr_info(MY_DEVICE_NAME ": I was assigned major number %d. To talk to\n", Major);
    pr_info("\tthe driver, create a dev file with\n");
    pr_info("\t'mknod /dev/%s c %d 0'.\n", MY_DEVICE_NAME, Major);*/

    return 0;
}

static void my_exit(void)
{
    cdev_del(&Cdev);
    device_destroy(Cls, Dev);
    class_destroy(Cls);
    unregister_chrdev_region(Dev, 1);
    pr_info(MY_DEVICE_NAME ": I was unloaded!\n");
}

static int device_open(struct inode* node, struct file* file_ptr)
{
    my_print("I'm being opened...");

    if (!request_mem_region(ISA_CARD_ADDR, ISA_SEGMENT_LEN, MY_DEVICE_NAME)) return -EADDRINUSE;
    my_print("Request memory region success.");

    if (!(Ptr = (YENET_HOST_IF_REGS*)ioremap_nocache(ISA_CARD_ADDR, ISA_SEGMENT_LEN))) return -EADDRNOTAVAIL;
    my_print("Remap success.");

    my_print("Open success.");
    return 0;
}

static int device_release(struct inode* node, struct file* file_ptr)
{
    my_print("I'm being released...");

    iounmap(Ptr);
    release_mem_region(ISA_CARD_ADDR, ISA_SEGMENT_LEN);
    return 0;
}

static ssize_t device_read(struct file* filp, /* see include/linux/fs.h   */
    char* buffer,      /* buffer to fill with data */
    size_t length,     /* length of the buffer     */
    loff_t* offset)
{
    my_print("I'm being read...");
    if (length <= 0) return 0;
    if ((*offset + length) > sizeof(YENET_HOST_IF_REGS)) return -EINVAL;

    my_print("Parameters are valid.");
    if (copy_to_user(buffer, Ptr + *offset, length) > 0) return -EFAULT;

    my_print("Read success.");
    return 0;
}

static ssize_t device_write(struct file* filp, const char* buf, size_t len, loff_t* off)
{
    pr_alert("Sorry, this operation isn't supported.\n");
    return -EINVAL;
}

static long device_ioctl(struct file* file_ptr, unsigned int cmd, unsigned long arg)
{
    my_print("I'm being controlled...");

    switch (cmd)
    {
    case IOCTL_RESET:
        pr_debug("CMD register address = %p\n", (void*)&(Ptr->cmd));
        ynb_set_cmd(Ptr, YN_CMD_RST_CER);
        break;
    case IOCTL_GET_STATUS:
    {
        uint16_t s;
        pr_debug("STATUS register address = %p\n", (void*)&(Ptr->sts));
        s = ynb_get_sts(Ptr);
        if (copy_to_user((uint16_t*)arg, &s, sizeof(s))) return -EACCES;
        break;
    }
    default:
        my_print("Nonexistant IOCTL! Doing nothing.");
        return -EINVAL;
    }

    my_print("Control success");
    return 0;
}
