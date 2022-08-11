#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karim ");
MODULE_DESCRIPTION("A hello world Psuedo device driver");


int print_count = 0;
module_param(print_count, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(print_count, "An integer to be used for setting how many print to make");
// Note: to make the parameter invisible in the '/sys/module/<module_name>' directory, pass '0' to the perm parameter
// Note: grouped permissions macros are found in the header '**/include/linux/stat.h'

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init my_init(void) {
    int i = 0;
    for (i=0; i<print_count; i++) {
        printk("Hello, Kernel!\n");
    }
    return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit my_exit(void) {
    int i = 0;
    for (i=0; i<print_count; i++) {
        printk("Goodbye, Kernel\n");
    }
}

module_init(my_init);
module_exit(my_exit);
