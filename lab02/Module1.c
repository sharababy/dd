/*
	Source Code
	Compilig Requires Linux Headers
		sudo apt-get update
		sudo apt-get install linux-headers-$(uname -r)
*/

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>

static int __init init_mod (void) /*Constructor*/
{
	printk (KERN_INFO "**************Bassi's Module Started**************\n");
	printk (KERN_INFO "This is printed by Bassi's Module\n");
	printk (KERN_INFO"Module is calculating square using bit shift\n");
	int cal = 1;

	printk (KERN_INFO"%d\n",cal<<2);
	
	return 0;
}

static void __exit end_mod (void) /*Destructor*/
{

	printk (KERN_INFO "**************Bassis's Module Has Ended**************\n");
}

module_init (init_mod);
module_exit (end_mod);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Vedant Bassi");
MODULE_DESCRIPTION ("Test Driver Module");