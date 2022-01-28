#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe9a850a9, "module_layout" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x95b33eab, "dma_map_sg_attrs" },
	{ 0x2303cd5e, "kmem_cache_destroy" },
	{ 0x8bd18bae, "cdev_del" },
	{ 0xbd0c4c0c, "kmalloc_caches" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x6cdf1bb2, "cdev_init" },
	{ 0xfb8bf508, "put_devmap_managed_page" },
	{ 0x4f7795eb, "pci_write_config_word" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x2a8e0af1, "pci_read_config_byte" },
	{ 0x46cf10eb, "cachemode2protval" },
	{ 0xef5ad72a, "dma_unmap_sg_attrs" },
	{ 0x4ba4f67b, "dma_set_mask" },
	{ 0xdb991a49, "pcie_set_readrq" },
	{ 0xd304cfde, "boot_cpu_data" },
	{ 0xa8b23284, "pci_disable_device" },
	{ 0x6062f032, "pci_disable_msix" },
	{ 0x7a8cbab2, "set_page_dirty_lock" },
	{ 0x56470118, "__warn_printk" },
	{ 0x837b7b09, "__dynamic_pr_debug" },
	{ 0xc47a0a94, "device_destroy" },
	{ 0x9332df87, "kobject_set_name" },
	{ 0x6729d3df, "__get_user_4" },
	{ 0x87b8798d, "sg_next" },
	{ 0x7ae25f56, "pci_release_regions" },
	{ 0xbcd8e97b, "pcie_capability_clear_and_set_word" },
	{ 0x409bcb62, "mutex_unlock" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x999e8297, "vfree" },
	{ 0x7dc91c27, "dma_free_attrs" },
	{ 0x7a2af7b4, "cpu_number" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xa0fc649, "pv_ops" },
	{ 0xdaf80bbf, "dma_set_coherent_mask" },
	{ 0x9dbe91db, "kthread_create_on_node" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xaa44a707, "cpumask_next" },
	{ 0x2efb103d, "kthread_bind" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x17de3d5, "nr_cpu_ids" },
	{ 0xf650e0f2, "pci_set_master" },
	{ 0x212a39fd, "pci_alloc_irq_vectors_affinity" },
	{ 0xfb578fc5, "memset" },
	{ 0xfd829469, "pci_restore_state" },
	{ 0xd4bc44f2, "pci_iounmap" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x977f511b, "__mutex_init" },
	{ 0xc5850110, "printk" },
	{ 0xfef216eb, "_raw_spin_trylock" },
	{ 0x8b6d9a88, "kthread_stop" },
	{ 0x5a5a2271, "__cpu_online_mask" },
	{ 0xafe49cd0, "pci_aer_clear_nonfatal_status" },
	{ 0x785a664b, "pci_read_config_word" },
	{ 0x925817dc, "dma_alloc_attrs" },
	{ 0x7b9783fa, "kmem_cache_free" },
	{ 0x2ab7989d, "mutex_lock" },
	{ 0xb940a00, "finish_swait" },
	{ 0xfd048de0, "device_create" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x6091797f, "synchronize_rcu" },
	{ 0xe97a4d6a, "pci_enable_msi" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0xedc0610f, "pci_find_capability" },
	{ 0x82500450, "cdev_add" },
	{ 0x800473f, "__cond_resched" },
	{ 0x3a2f6702, "sg_alloc_table" },
	{ 0xe702f4e3, "kmem_cache_alloc" },
	{ 0xb601be4c, "__x86_indirect_thunk_rdx" },
	{ 0x618911fc, "numa_node" },
	{ 0xb2fd5ceb, "__put_user_4" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0x9cb986f2, "vmalloc_base" },
	{ 0x1000e51, "schedule" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0x1d24c881, "___ratelimit" },
	{ 0x3a13711e, "prepare_to_swait_event" },
	{ 0x2ea2c95c, "__x86_indirect_thunk_rax" },
	{ 0x6170ca00, "wake_up_process" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x85e26479, "pci_unregister_driver" },
	{ 0xdbee476f, "kmem_cache_alloc_trace" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xb19a5453, "__per_cpu_offset" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xfcbcd3b7, "kmem_cache_create" },
	{ 0x879e28e3, "pci_irq_vector" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0x37a0cba, "kfree" },
	{ 0xdfad5eb5, "remap_pfn_range" },
	{ 0x95cac1d, "pci_request_regions" },
	{ 0x6df1aaf1, "kernel_sigaction" },
	{ 0xdcac0db9, "pci_disable_msi" },
	{ 0x4ffc799e, "__pci_register_driver" },
	{ 0x3a0bca50, "class_destroy" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x608741b5, "__init_swait_queue_head" },
	{ 0x7f5b4fe4, "sg_free_table" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x6da2bd80, "pci_iomap" },
	{ 0xe74345bd, "pci_enable_device_mem" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0x4a453f53, "iowrite32" },
	{ 0xb23f5abe, "pci_enable_device" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xd0210779, "param_ops_uint" },
	{ 0x43f84b37, "nv_p2p_get" },
	{ 0xbffcce80, "__class_create" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xf03f82f2, "__put_page" },
	{ 0xa78af5f3, "ioread32" },
	{ 0xce1c3796, "get_user_pages_fast" },
	{ 0xc80ab559, "swake_up_one" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xe2f63914, "pci_save_state" },
	{ 0x587f22d7, "devmap_managed_key" },
	{ 0x8a35b432, "sme_me_mask" },
};

MODULE_INFO(depends, "gpuctl");

MODULE_ALIAS("pci:v000010EEd00009048sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009044sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009042sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009041sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd0000903Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009038sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009028sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009018sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009034sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009024sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009014sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009032sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009022sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009012sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009031sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009021sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009011sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008011sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008012sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008014sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008018sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008021sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008022sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008024sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008028sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008031sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008032sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008034sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008038sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007011sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007012sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007014sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007018sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007021sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007022sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007024sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007028sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007031sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007032sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007034sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007038sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006828sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006830sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006928sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006930sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006A28sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006A30sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006D30sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00004808sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00004828sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00004908sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00004A28sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00004B28sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00002808sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D0Fd0000F000sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D0Fd0000F001sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "90BDB88246CD59778C146ED");
