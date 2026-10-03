/* hello-4.c - Demonstrates module documentation. */
#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/init.h> /* Needed for the macros */
#define DRIVER_AUTHOR "Peter Jay Salzman p@dirac.org"
#define DRIVER_DESC "A sample driver"

static int __init init_hello_4(void) {
	printk(KERN_INFO "Hello, world 4\n");
	return 0;
}
static void __exit cleanup_hello_4(void) {
	printk(KERN_INFO "Goodbye, world 4\n");
}

module_init(init_hello_4);
module_exit(cleanup_hello_4);

MODULE_LICENSE("GPL"); /* Get rid of taint message by declaring code as GPL. */ 
MODULE_AUTHOR(DRIVER_AUTHOR); /* Who wrote this module? */
MODULE_DESCRIPTION(DRIVER_DESC); /* What does this module do */ 
MODULE_SUPPORTED_DEVICE("testdevice"); /* For documentation purposes. */

