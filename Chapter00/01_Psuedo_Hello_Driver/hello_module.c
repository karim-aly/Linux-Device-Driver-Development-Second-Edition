#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karim ");
MODULE_DESCRIPTION("A hello world Psuedo device driver");


int print_count = 0;
module_param(print_count, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init my_init(void) {
    printk("Hello, Kernel!\n");
    return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit my_exit(void) {
    printk("Goodbye, Kernel\n");
}

module_init(my_init);
module_exit(my_exit);
