#include <linux/module.h>
#include <linux/kernel.h>
#define __NO_VERSION__
#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/pci.h>
#include <linux/pagemap.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include "gpumemdrv.h"
#include "gpuctl.h"
#include "nvfs-p2p.h"
MODULE_AUTHOR("Shun Kasai");
// MODULE_DESCRIPTION(DRV_MODULE_DESC);
// MODULE_VERSION(DRV_MODULE_VERSION);
MODULE_LICENSE("GPL v2");
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
static void free_nvp_callback(void *data)
{
  int res;
  struct gpumem_t *entry = (struct gpumem_t*)data;
  if(entry) {
    res = nvfs_nvidia_p2p_free_page_table(entry->page_table);
    if(res == 0) {
      printk(KERN_ERR"%s(): nvidia_p2p_free_page_table() - OK!\n", __FUNCTION__);
      //entry->virt_start = 0ULL;
      //entry->page_table = 0;
    } else {
      printk(KERN_ERR"%s(): Error in nvidia_p2p_free_page_table()\n", __FUNCTION__);
    }
  }
}

int nv_p2p_get(unsigned long arg){
  int error = 0;
  size_t pin_size = 0ULL;
  struct gpumem_t *entry = 0;
  struct gpudma_lock_t param;
  
  printk("hello!");
  
  if(copy_from_user(&param, (void *)arg, sizeof(struct gpudma_lock_t))) {
    printk(KERN_ERR"%s(): Error in copy_from_user()\n", __FUNCTION__);
    error = -EFAULT;
    goto do_exit;
  }
  entry = (struct gpumem_t*)kzalloc(sizeof(struct gpumem_t), GFP_KERNEL);
  if(!entry) {
    printk(KERN_ERR"%s(): Error allocate memory to mapping struct\n", __FUNCTION__);
    error = -ENOMEM;
    goto do_exit;
  }
  printk("allocate memory to mapping successfull! %lld", param.size);
  INIT_LIST_HEAD(&entry->list);
  entry->handle = entry;
  
  entry->virt_start = (param.addr & GPU_BOUND_MASK);
  pin_size = (param.addr + param.size - entry->virt_start);
  if(!pin_size) {
    printk(KERN_ERR"%s(): Error invalid memory size!\n", __FUNCTION__);
    error = -EINVAL;
    goto do_free_mem;
  }
  printk("virtual_address : %lld\nlength : %ld", entry->virt_start, pin_size);
  printk("&entry->list : %pr", &param);
  printk("page_table : %pr", &entry->page_table);
  error = nvfs_nvidia_p2p_get_pages(0, 0, entry->virt_start, pin_size, &entry->page_table, free_nvp_callback, entry);
  if(error != 0) {
    printk(KERN_ERR"%s(): Error in nvidia_p2p_get_pages()\n", __FUNCTION__);
    printk("error num is %d", error);
    error = -EINVAL;
    goto do_free_mem;
  }
  printk("successfull!");
  
  param.page_count = entry->page_table->entries;
  param.handle = entry;
  
  printk(KERN_ERR"%s(): param.handle: %p\n", __FUNCTION__, param.handle);
  
  if(copy_to_user((void *)arg, &param, sizeof(struct gpudma_lock_t))) {
    printk(KERN_ERR"%s(): Error in copy_from_user()\n", __FUNCTION__);
    error = -EFAULT;
    goto do_unlock_pages;
  }
  
  // list_add_tail(&entry->list, &drv->table_list);
  
  printk(KERN_ERR"%s(): Add new entry. handle: %p\n", __FUNCTION__, entry->handle);
  
 do_unlock_pages:
  nvfs_nvidia_p2p_put_pages(0, 0, entry->virt_start, entry->page_table);
 do_free_mem:
  kfree(entry);
 do_exit:
  if (error != 0)
    printk("error!");
  return error;
}
EXPORT_SYMBOL(nv_p2p_get);

static void test_exit(void)
{
  printk("Bye bye my module\n");
}

module_init(test_init);
module_exit(test_exit);
