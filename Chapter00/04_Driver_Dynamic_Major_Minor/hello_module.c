#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h> // for registering the device major number and name
#include <linux/cdev.h>


/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karim ");
MODULE_DESCRIPTION("A hello world Psuedo device driver");


// >> Driver module parameters
// Note: to make the parameter invisible in the '/sys/module/<module_name>' directory, pass '0' to the 'perm' parameter
// Note: grouped permissions macros are found in the header '**/include/linux/stat.h'
static int print_count = 1;
module_param(print_count, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(
    print_count,
    "An integer to be used for setting how many print to make"
);


// Define variables and needed functions for file_operations after registering the device driver
static const char* DEVICE_NAME = "my_pseudo_hello_device";
static dev_t device_number;

static struct cdev hello_cdev;

static const char* HELLO_CLASS_NAME = "class_hello";
static struct class* p_hello_class = NULL;

static const char* DEVICE_FILE_NAME = "hello_test_file";
static struct device* p_device = NULL;


// >> Char Device File Operations 
static int hello_driver_open(struct inode* device_inode, struct file* device_file) {
    printk("%s: Open is called!\n", __func__);
    return 0;
}

static int hello_driver_release(struct inode* device_inode, struct file* device_file) {
    printk("%s: Release is called!\n", __func__);
    return 0;
}

static ssize_t hello_driver_write(struct file *file, const char __user *buf,
                                  size_t len, loff_t *ppos) {
    pr_info("%s: Write is called!, i_ino=%lu\n", __func__,  file->f_inode->i_ino);
    return len; /* Not doing anything with the data */
}

static ssize_t hello_driver_read(struct file *file, char __user *buf,
                                 size_t len, loff_t *ppos) {
    pr_info("%s: Read is called!, i_ino=%lu\n", __func__, file->f_inode->i_ino);
    len = 0;
    return len; /* Not do anything with the data */
}

static const struct file_operations hello_fops = {
    .owner = THIS_MODULE,
    .open = hello_driver_open,
    .release = hello_driver_release,
    .read = hello_driver_read,
    .write = hello_driver_write,
};
static const struct file_operations* const p_hello_fops = &hello_fops;


/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init hello_init(void) {
    int i = 0;
    int ret_val;

    // check on the passed parameter 'print_count'
    if (print_count <= 0) {
        print_count = 1;
    }

    for (i=0; i<print_count; i++) {
        pr_info("%s: Hello, Kernel!\n", __func__);
    }

    // 1- allocate the device number with base_minor=0 and count=1
    ret_val = alloc_chrdev_region(&device_number, 0, 1, DEVICE_NAME);
    if (ret_val != 0) {
        pr_err("%s: Error%d - Could not allocate a device number\n", __func__, ret_val);
        return ret_val;
    }
    pr_info("%s: Device number allocated successfully with Major=%d, Minor=%d\n",
            __func__, MAJOR(device_number), MINOR(device_number));


    // 2- define driver to be character or block or network
    cdev_init(&hello_cdev, p_hello_fops);
    ret_val = cdev_add(&hello_cdev, device_number, 1);
    if (ret_val < 0) {
        pr_err("%s: Error %d - Could not add the character device\n", __func__, ret_val);
        unregister_chrdev_region(device_number, 1);
        return ret_val;
    }
    pr_info("%s: Character device addded successfully!\n", __func__);


    // 3- generate device file
    // first create the class
    p_hello_class = class_create(THIS_MODULE, HELLO_CLASS_NAME);
    if (p_hello_class == NULL) {
        pr_err("%s: Error - Could not create '%s' class\n", __func__, HELLO_CLASS_NAME);
        cdev_del(&hello_cdev);
        unregister_chrdev_region(device_number, 1);
        return -1;
    }

    // then, create the device file
    p_device = device_create(p_hello_class, NULL, device_number, NULL, DEVICE_FILE_NAME);
    if (p_device == NULL) {
        pr_err("%s: Error - Could not create '%s' device file\n", __func__, DEVICE_FILE_NAME);
        class_destroy(p_hello_class);
        cdev_del(&hello_cdev);
        unregister_chrdev_region(device_number, 1);
        return -1;
    }

    pr_info("%s: Hello Device Driver is Active!\n", __func__);

    return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit hello_exit(void) {
    // cleanup
    device_destroy(p_hello_class, device_number);
    class_destroy(p_hello_class);
    cdev_del(&hello_cdev);
    unregister_chrdev_region(device_number, 1); // unregister allocated device number

    // unregister the device
    // unregister_chrdev(MAJOR(device_number), DEVICE_NAME);

    pr_info("%s: Goodbye, Kernel\n", __func__);
}

module_init(hello_init);
module_exit(hello_exit);
