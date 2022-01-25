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
	{ 0xbd0c4c0c, "kmalloc_caches" },
	{ 0x409bcb62, "mutex_unlock" },
	{ 0xc5850110, "printk" },
	{ 0xde80cd09, "ioremap" },
	{ 0x2ab7989d, "mutex_lock" },
	{ 0x868784cb, "__symbol_get" },
	{ 0x2ea2c95c, "__x86_indirect_thunk_rax" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xdbee476f, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0x6e9dd606, "__symbol_put" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "63F62E1C3DE63FEE0D7F626");
