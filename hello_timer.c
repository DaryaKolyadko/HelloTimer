#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/kernel.h> 
#include <linux/timer.h>


struct timer_list timer;
long delay = - 1;

void hello_world_function(unsigned long arg)
{
	printk(KERN_INFO "Hello, world!\n");
	mod_timer(&timer, jiffies + delay * HZ);
	if (delay == 0)
		del_timer(&timer);
}

 // method invoked on read of a sysfs file 
ssize_t show(struct kobject* kobj, struct kobj_attribute* attr, char* buffer) 
{
	return sprintf(buffer,"%ld\n", delay);
}

// method invoked on write of a sysfs file 
ssize_t store(struct kobject* kobj, struct kobj_attribute* attr, const char* buffer, size_t size) 
{
	kstrtol(buffer, 10, &delay);
	if (delay == 0) {
	      del_timer(&timer);
    }
    init_timer(&timer);
	timer.function = hello_world_function;
	timer.data = 0;
	add_timer(&timer);
	return size;
}

struct kobj_attribute hello_attr = {
	.attr = {
		.name = "HelloWorld",
		.mode = S_IWUSR | S_IRUGO,
	},
	.show = show,
	.store = store,
};

struct kobject* hello_kobj;

int init_module(void)
{
	hello_kobj = kobject_create_and_add("hello_world", NULL); // create a dir in sys/
	if (hello_kobj != NULL){
		int result = sysfs_create_file(hello_kobj, &hello_attr.attr);   // create an attribute file in hello_world 
		if (result == 0){ 
			return 0;
		}
		kobject_put(hello_kobj);
		return result;
	}
	return -ENOMEM;
}

void cleanup_module(void)
{
	del_timer(&timer);
	sysfs_remove_file(hello_kobj, &hello_attr.attr);
	kobject_put(hello_kobj);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HelloWorldPrinter");
MODULE_AUTHOR("Darya Kolyadko");