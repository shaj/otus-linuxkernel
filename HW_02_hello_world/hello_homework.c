#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <asm-generic/errno-base.h>
#include <linux/slab.h>

#define BUFFER_SIZE (64)

static int idx_param; //
static char chval_param;
static char *str_param;

static int idx_set(const char *val, const struct kernel_param *kp)
{
	int res;
	int new_value;

	res = kstrtoint(val, 10, &new_value);
	if (res) {
		pr_err("error: idx must be numerical value\n");
		return res;
	}

	if (new_value < 0 || new_value > BUFFER_SIZE) {
		pr_err("Error: the value must be between 0 and %d!\n",
		       BUFFER_SIZE);
		return -EINVAL;
	}

	idx_param = new_value;

	return 0;
}

static int chval_set(const char *val, const struct kernel_param *kp)
{
	if ((*val >= 0x20) && (*val <= 0x7e)) {
		chval_param = *val;
		str_param[idx_param] = chval_param;
		pr_info("Setting up symbol %c at position %d\n", chval_param,
			idx_param);
	} else {
		pr_err("Symbol 0x%02Xd is unprintable", (int)(*val));
	}
	return param_set_charp(val, kp);
}

static const struct kernel_param_ops idx_ops = {
	.set = idx_set,
	.get = param_get_int,
};

static const struct kernel_param_ops chval_ops = {
	.set = chval_set,
	.get = param_get_byte,
};

// Объявляем параметр с callback
module_param_cb(idx_param, &idx_ops, &idx_param, 0664);
MODULE_PARM_DESC(idx_param, "Index (range: 0-BUFFER_SIZE)");
module_param_cb(chval_param, &chval_ops, &chval_param, 0664);
MODULE_PARM_DESC(chval_param, "Char to put at idx (range: is_alpha)");
module_param(str_param, charp, 0444);
MODULE_PARM_DESC(mystring, "A result string");

static int __init mod_init(void)
{
	str_param = kmalloc(BUFFER_SIZE + 1, GFP_KERNEL);
	if (str_param == 0) {
		return -ENOMEM;
	}
	memset(str_param, 0, BUFFER_SIZE + 1);

	pr_info("init complete\n");
	return 0;
}

static void __exit mod_exit(void)
{
	kfree(str_param);
	pr_info("exit\n");
}

module_init(mod_init);
module_exit(mod_exit);
MODULE_LICENSE("MIT");
MODULE_AUTHOR("Shaj");
MODULE_DESCRIPTION("A simple param-callback module for the Linux kernel");
