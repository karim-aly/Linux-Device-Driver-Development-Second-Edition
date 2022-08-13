#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h> // for registering the device major number and name

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karim ");
MODULE_DESCRIPTION("A module to unregister a device major number");


int major_number = 50;
module_param(major_number, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(major_number, "An integer to be used for setting the major number to unregister");

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init remove_init(void) {
    printk("%s: Module loaded successfully with Major=%d\n", __FUNCTION__, major_number);

    unregister_chrdev(major_number, "my_pseudo_hello_device");

    return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit remove_exit(void) {
    printk("%s: Goodbye, Kernel\n", __FUNCTION__);
}

module_init(remove_init);
module_exit(remove_exit);
