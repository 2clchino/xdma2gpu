#include <linux/module.h>
#include "test.h"
MODULE_AUTHOR("Shun Kasai");
// MODULE_DESCRIPTION(DRV_MODULE_DESC);
// MODULE_VERSION(DRV_MODULE_VERSION);
MODULE_LICENSE("GPL");
static int test_init(void)
{
    printk("Hello my module\n");
    return 0;
}

int say_hello(void){
  printk("hello from empty module\n");
  return 0;
}
EXPORT_SYMBOL(say_hello);

static void test_exit(void)
{
    printk("Bye bye my module\n");
}

module_init(test_init);
module_exit(test_exit);
