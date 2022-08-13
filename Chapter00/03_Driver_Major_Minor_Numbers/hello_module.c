#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h> // for registering the device major number and name

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karim ");
MODULE_DESCRIPTION("A hello world Psuedo device driver");

// >> Driver module parameters
// Note: to make the parameter invisible in the '/sys/module/<module_name>' directory, pass '0' to the 'perm' parameter
// Note: grouped permissions macros are found in the header '**/include/linux/stat.h'
static int print_count = 1;
MODULE_PARM_DESC(print_count, "An integer to be used for setting how many print to make");
module_param(print_count, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);


// Define variables and needed for file_operations after registering the device driver
static const unsigned int MAJOR_NUMBER = 50;
static const char* DEVICE_NAME = "my_pseudo_hello_device";


// >> Char Device File Operations 
static int hello_driver_open(struct inode* device_inode, struct file* device_file) {
    printk("%s: Open is called!\n", __FUNCTION__);
    return 0;
}

static int hello_driver_release(struct inode* device_inode, struct file* device_file) {
    printk("%s: Release is called!\n", __FUNCTION__);
    return 0;
}

static ssize_t hello_driver_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos) {
    pr_info("%s: Write is called!\n", __func__);
    return len; /* Not doing anything with the data */
}

static ssize_t hello_driver_read(struct file *file, char __user *buf, size_t len, loff_t *ppos) {
    pr_info("%s: Read is called!\n", __func__);
    return len; /* Not do anything with the data */
}

static const struct file_operations hello_fops = {
    .owner=THIS_MODULE,
    .open=hello_driver_open,
    .release=hello_driver_release,
    .read=hello_driver_read,
    .write=hello_driver_write,
};


/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init hello_init(void) {
    int i = 0;
    int ret_val;

    // check on the passed parameter 'print_count' not to be less than or equal zero
    if (print_count <= 0) {
        print_count = 1;
    }

    for (i=0; i<print_count; i++) {
        printk("%s: Hello, Kernel!\n", __FUNCTION__);
    }

    // register the char device using our static device major number
    // if '0' is passed, Linux  will  choose a major number and return the chosen value
    ret_val = register_chrdev(MAJOR_NUMBER, DEVICE_NAME, &hello_fops);
    if (ret_val < 0) {
        pr_err("%s: Error %d - Could not register device number with Major=%d\n", __FUNCTION__, ret_val, MAJOR_NUMBER);
        return -1;
    }

    printk("%s: Device(%s) registered successfully with Major=%d, Return_Value=%d\n", __FUNCTION__, DEVICE_NAME, MAJOR_NUMBER, ret_val);

    return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit hello_exit(void) {
    int i = 0;

    // unregister the device
    unregister_chrdev(MAJOR_NUMBER, DEVICE_NAME);
    printk("%s: Device(%s) unregistered successfully with Major=%d\n", __FUNCTION__, DEVICE_NAME, MAJOR_NUMBER);

    for (i=0; i<print_count; i++) {
        printk("%s: Goodbye, Kernel\n", __FUNCTION__);
    }
}

module_init(hello_init);
module_exit(hello_exit);
