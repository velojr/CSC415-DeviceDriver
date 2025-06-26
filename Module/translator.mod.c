#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

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
	{ 0x122c3a7e, "_printk" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xe43acdf7, "cdev_init" },
	{ 0xea4a6df7, "cdev_add" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x999e8297, "vfree" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0xdad43328, "cdev_del" },
	{ 0xdcb764ad, "memset" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0xa65c6def, "alt_cb_patch_nops" },
	{ 0x98cf60b3, "strlen" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xbfa9f321, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "22E286ED4EC1C40F4D5B9EC");
