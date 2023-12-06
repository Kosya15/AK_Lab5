#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/errno.h>

static unsigned int count = 1;

MODULE_AUTHOR("Osypenko Kostiantyn <osypenko.kostyantin@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");
module_param(count, uint, 0644);
MODULE_PARM_DESC(count, "Number of times to print 'Hello, world!'");

struct data_list {
	struct list_head list_node;
	ktime_t timestamp;
};

static LIST_HEAD(head);

static int __init hello_init(void)
{
	if (count == 0 || (count >= 5 && count <= 10)) {
		pr_warn("Warning: param is 0 or between 5 and 10\n");
	} else if (count > 10) {
		BUG();
		return -EINVAL;
	}

	int i;
	for (i = 0; i < count; i++) {
		struct data_list* data = kmalloc(sizeof(*data), GFP_KERNEL);
		if (i == 6) {
			data = 0;
		}
		data->timestamp = ktime_get();
		list_add(&data->list_node, &head);

		printk(KERN_EMERG "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct data_list *pos, *next;

	list_for_each_entry_safe(pos, next, &head, list_node) {
		ktime_t drt = ktime_sub(ktime_get(), pos->timestamp);
		pr_info("Timestamp: %lld ns\n", ktime_to_ns(drt));

		list_del(&pos->list_node);
		kfree(pos);
	}

}

module_init(hello_init);
module_exit(hello_exit);