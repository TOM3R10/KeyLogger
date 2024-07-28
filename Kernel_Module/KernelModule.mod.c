#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
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
__used __section(__versions) = {
	{ 0xdd8f8694, "module_layout" },
	{ 0xdd02ac5a, "misc_deregister" },
	{ 0xc5850110, "printk" },
	{ 0x9ed554b3, "unregister_keyboard_notifier" },
	{ 0x978f9593, "misc_register" },
	{ 0x96554810, "register_keyboard_notifier" },
	{ 0x4c9d28b0, "phys_base" },
	{ 0xa18bdd53, "remap_pfn_range" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0xcf2a6966, "up" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x81b395b3, "down_interruptible" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "18A4067977CCC52E95CF37B");
