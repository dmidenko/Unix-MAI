#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/time.h>

#define DEVICE_NAME "mydv"

static int Major;
static int Device_Open = 0;

static int drelease(struct inode *, struct file *);
static int dopen(struct inode *, struct file *);
static long dioctl (struct file *, unsigned int, unsigned long);
static ssize_t dwrite(struct file *, const char *, size_t , loff_t *);

static struct file_operations fops = {
        // we dont have read bsc we may not have it
        .release = drelease,
        .open = dopen,
        .write = dwrite,
        .unlocked_ioctl = dioctl
};

typedef struct timespec tms;

typedef struct{
    tms ts;
    char data[50];
} msg;

#define BUFF _IOR('a','a', int *)
#define MSG_G _IOWR('a','b', msg *)

static int fptr = 0;
static int last_ptr = 0;
static msg buffer[20];


static int dopen(struct inode *ino, struct file *fl){
    if (Device_Open)
        return -EBUSY;

    printk(KERN_ALERT "Door opeeeeeeeeen\n");
    Device_Open++;
    try_module_get(THIS_MODULE);
    return 0;
}

static ssize_t dwrite(struct file *fl, const char *buff, size_t len, loff_t * off){
    printk(KERN_ALERT "Writing msg in buffer.\n");
    if (len > 49) len = 49;

    if(copy_from_user(buffer[last_ptr].data, buff, len)){
        printk(KERN_ALERT "Error copy from user\n");
        return 0;
    }
    buffer[last_ptr].data[len] = '\0';

    printk(KERN_ALERT "Inserting msg in buffer. Pos:%i\n", last_ptr);

    last_ptr = (last_ptr + 1) % 20;
    if (last_ptr == fptr)
        fptr = (fptr + 1) % 20;

    return len;
}

static int drelease(struct inode *inode, struct file *file){
    Device_Open--;
    printk(KERN_ALERT "Dev close\n");
    module_put(THIS_MODULE);
    return 0;
}

static long dioctl (struct file *file, unsigned int cmd, unsigned long arg){
    msg req;
    int num;
    printk(KERN_INFO "IOCTL = %i\n", cmd);
    switch(cmd) {
        case BUFF:
            num = fptr > last_ptr ? (20 - fptr + last_ptr) : (last_ptr - fptr);
            copy_to_user((int*) arg, &num, sizeof(num));
            break;
        case MSG_G:
            copy_from_user(&req, (int*) arg, sizeof(req));
            num = (fptr + *(int*)&req) % 20;
            printk(KERN_INFO "Get msg with num %d\n", num);
            req = buffer[num];
            copy_to_user( (int*) arg, &req, sizeof(req));
            break;
    }
    return 0;
}

static int mod_init(void){
    printk(KERN_ALERT "Hello !!!\n");
    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "I was assigned major number %d.\n", Major);
    return 0;
}

static void mod_exit(void){
    printk(KERN_ALERT "Bye !!!\n");
    unregister_chrdev(Major, DEVICE_NAME);
}

module_init(mod_init);
module_exit(mod_exit);