/*
 * This is a demo Linux kernel module.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/mm.h>
#include <linux/signal.h>

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

#define GPIO_EXTIPSELL 0x100
#define GPIO_EXTIPSELH 0x104
#define GPIO_EXTIRISE 0x108
#define GPIO_EXTIFALL 0x10c
#define GPIO_IEN 0x110
#define GPIO_IFC 0x11c
#define GPIO_PC_DIN (0x1c + 0x48)
#define GPIO_PC_MODEL (0x04 + 0x48)
#define GPIO_PC_DOUT (0x0c + 0x48)

 static int my_remove(struct platform_device *dev);
 static int my_open(struct inode *inode, struct file *filp);
 static int my_release(struct inode *inode, struct file *filp);
 static int my_probe(struct platform_device *dev);
 static ssize_t my_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
 static irqreturn_t interrupt_handler(int irq, void *dev_id, struct pt_regs *regs);




 struct inputoutput{
 	void* extipsell;
 	void* extipselh;
 	void* extirise;
 	void* extifall;
 	void* ien;
 	void* ifc;
 	void* pc_din;
 	void* pc_model;
 	void* pc_dout;
 };

 struct gamepad{
 	int active;
 	int irq_even;
 	int irq_odd;
 	dev_t devno;
 	struct fasync_struct *async_queue;
 	struct class *my_class;
 	struct resource *mem;
 	struct resource *res;
 	struct cdev my_cdev;
 	struct inputoutput gpio;


 };

 static struct gamepad gamepadDriver;






 //TODO: Bruk ioread() og iowrite()

 //request_irq()
 //request_mem_region()
 //ioremap_nocache()

 //GPIO Even: IRQ number  = 17, Platform IRQ index = 0, Platform mem index = 0.
 //GPIO Odd: IRQ number  = 18, Platform IRQ index = 1, Platform mem index = 0.
 //Timer 3: IRQ number  = 19, Platform IRQ index = 2, Platform mem index = 1.
 //DMA: IRQ number  = 20, Platform IRQ index = 3, Platform mem index = 2.
 //DAC IRQ number  = 21, Platform IRQ index = 4, Platform mem index = 3.
 static irqreturn_t interrupt_handler(int irq, void *dev_id, struct pt_regs *regs){
 	iowrite32(0xff, gamepadDriver.gpio.ifc);
 	kill_fasync((&gamepadDriver.async_queue), SIGIO, POLL_IN);
 	return IRQ_HANDLED;
 }

 static int my_open(struct inode *inode, struct file *filp){
 	if(gamepadDriver.active == 0){
 		gamepadDriver.mem = request_mem_region(gamepadDriver.res->start, gamepadDriver.res->end - gamepadDriver.res->start, "tdt4258");

 		if(gamepadDriver.mem == 0){
 			printk(KERN_ERR "Alt gikk til helvete med lasting av minnet");
 		}

 		iowrite32(0x33333333, gamepadDriver.gpio.pc_model);
 		iowrite32(0xff, gamepadDriver.gpio.pc_dout);
 		iowrite32(0x22222222, gamepadDriver.gpio.extipsell);
 		iowrite32(0xff, gamepadDriver.gpio.extirise);
 		iowrite32(0xff, gamepadDriver.gpio.extifall);
 		iowrite32(0xff, gamepadDriver.gpio.ien);

 		request_irq(gamepadDriver.irq_even, (irq_handler_t) interrupt_handler, 0, "driver-gamepad", &gamepadDriver.my_cdev);
 		request_irq(gamepadDriver.irq_odd, (irq_handler_t) interrupt_handler, 0, "driver-gamepad", &gamepadDriver.my_cdev);
 	}
 	gamepadDriver.active+=1;
 	printk("driver opened.");
 	return 0;
 }


static int my_release(struct inode *inode, struct file *filp){
 	gamepadDriver.active-=1;

 		if(gamepadDriver.active == 0){

 		iowrite32(0, gamepadDriver.gpio.pc_model);
 		iowrite32(0, gamepadDriver.gpio.pc_dout);
 		//iowrite32(0, gamepadDriver.gpio.extipsell);
 		iowrite32(0, gamepadDriver.gpio.extirise);
 		iowrite32(0, gamepadDriver.gpio.extifall);
 		iowrite32(0, gamepadDriver.gpio.ien);

 		free_irq(gamepadDriver.irq_even, &gamepadDriver.my_cdev);
 		free_irq(gamepadDriver.irq_odd, &gamepadDriver.my_cdev);

 		release_mem_region(gamepadDriver.res->start, gamepadDriver.res->end - gamepadDriver.res->start);
 	}
 	//gamepadDriver.active+=1;
 	printk("driver opened.");
 	return 0;
 }

 static ssize_t my_read(struct file *filp, char __user *buff, size_t count, loff_t *offp){
 	printk("myread");
 	int8_t data = ~ioread8(gamepadDriver.gpio.pc_din);
 	copy_to_user(buff, &data,1);
 	return 0;
 }

 static int my_fasync(int fd, struct file *filp, int mode){
 	return fasync_helper(fd, filp, mode, &(gamepadDriver.async_queue));
 }



static struct file_operations my_fops = {
 	.owner = THIS_MODULE,
 	.read = my_read,
 	.open = my_open	,
 	.release = my_release,
 	.fasync = my_fasync
 };

 static int my_probe(struct platform_device *dev){
 	int err;
 	gamepadDriver.active=0;
 	err = alloc_chrdev_region(&gamepadDriver.devno,0,1,"driver-gamepad");
 	if(err < 0){
 		printk(KERN_ERR "Minneallokering feilet");
 	}
 	printk("Min hentet\n");
 	gamepadDriver.res = platform_get_resource(dev, IORESOURCE_MEM,0);
 	if(!gamepadDriver.res){
 		printk(KERN_ERR "Failed to get resource");
 	}
 	printk("ressurser hentet\n");
 	gamepadDriver.my_class = class_create(THIS_MODULE, "driver-gamepad");
 	device_create(gamepadDriver.my_class, NULL, gamepadDriver.devno,NULL, "driver-gamepad");

 	gamepadDriver.gpio.extipsell = ioremap_nocache(gamepadDriver.res->start + GPIO_EXTIPSELL, 4);
 	gamepadDriver.gpio.extipselh = ioremap_nocache(gamepadDriver.res->start + GPIO_EXTIPSELH, 4);
 	gamepadDriver.gpio.extirise = ioremap_nocache(gamepadDriver.res->start + GPIO_EXTIRISE, 4);
 	gamepadDriver.gpio.extifall = ioremap_nocache(gamepadDriver.res->start + GPIO_EXTIFALL, 4);
 	gamepadDriver.gpio.ien = ioremap_nocache(gamepadDriver.res->start + GPIO_IEN, 4);
 	gamepadDriver.gpio.ifc = ioremap_nocache(gamepadDriver.res->start + GPIO_IFC, 4);
 	gamepadDriver.gpio.pc_din = ioremap_nocache(gamepadDriver.res->start + GPIO_PC_DIN, 4);
 	gamepadDriver.gpio.pc_model = ioremap_nocache(gamepadDriver.res->start + GPIO_PC_MODEL, 4);
 	gamepadDriver.gpio.pc_dout = ioremap_nocache(gamepadDriver.res->start + GPIO_PC_DOUT, 4);

 	gamepadDriver.irq_even = platform_get_irq(dev,0);
 	gamepadDriver.irq_odd = platform_get_irq(dev, 1);

 	cdev_init(&gamepadDriver.my_cdev, &my_fops);
 	err=cdev_add(&gamepadDriver.my_cdev, gamepadDriver.devno,1);
 	if(err <0){
 		printk(KERN_ERR "Failed to activate char driver./n");
 	}
 	printk("probe fullført\n");
 	return 0;

 }

 static int my_remove(struct platform_device *dev){
 	printk(KERN_ERR"removed");
 	return 0;
 }

 static const struct of_device_id my_of_match[] = {
 	{ .compatible = "tdt4258"},
 	{ }
 };

MODULE_DEVICE_TABLE(of, my_of_match);

 static struct platform_driver my_device = {
 	.probe = my_probe,
 	.remove = my_remove,
 	.driver = {
 		.name = "my",
 		.owner = THIS_MODULE,
 		.of_match_table = my_of_match,
 	},
 };



// res->start
// res->end



static int __init template_init(void){
	platform_driver_register(&my_device);
	printk("Hello World, here is your module speaking\n");
	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void)
{
	cdev_del(&gamepadDriver.my_cdev);
	unregister_chrdev_region(gamepadDriver.devno, 1);
	printk("Short life for a small module...\n");
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

