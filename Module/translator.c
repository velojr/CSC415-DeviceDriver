/**************************************************************
* Course: CSC-415-02 Fall 2024
* Name: Juan Segura Rico
* Student ID: 921725126
* GitHub: velojr
* Project: Assignment 6 – Coding a Device Driver
*
* File: translator.c
*
* Description: Main file for our translation device driver 
*
**************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "translator"
#define IOCTL_SWITCH_MODE _IOW('t', 1, int)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Juan Segura Rico");
MODULE_DESCRIPTION("Translation device driver");

struct DeviceStruct {
    int called;
    char *translateEngToSpanish[5];
    char *translateSpanishToEng[5];
    int switched;  // 0 for English to Spanish, 1 for Spanish to English
    char *currWord;
};

static struct cdev cDevice;
static dev_t devno;

// Open function
static int DeviceOpen(struct inode *inode, struct file *file) {
    struct DeviceStruct *ds = vmalloc(sizeof(struct DeviceStruct));
    if (!ds) {
        pr_err("Failed to allocate memory for DeviceStruct\n");
        return -ENOMEM;
    }

    ds->called = 0;
    ds->switched = 0;
    ds->translateEngToSpanish[0] = "Hello";
    ds->translateEngToSpanish[1] = "Friend";
    ds->translateEngToSpanish[2] = "Parents";
    ds->translateEngToSpanish[3] = "Good morning";
    ds->translateEngToSpanish[4] = "Thanks";

    ds->translateSpanishToEng[0] = "Hola";
    ds->translateSpanishToEng[1] = "Amigo";
    ds->translateSpanishToEng[2] = "Padres";
    ds->translateSpanishToEng[3] = "Buenos dias";
    ds->translateSpanishToEng[4] = "Gracias";

    ds->currWord = vmalloc(30);
    if (!ds->currWord) {
        pr_err("Failed to allocate memory for currWord\n");
        vfree(ds);
        return -ENOMEM;
    }

    file->private_data = ds;
    pr_info("Device opened\n");
    return 0;
}

// Release function
static int DeviceRelease(struct inode *inode, struct file *file) {
    struct DeviceStruct *ds = file->private_data;

    if (ds) {
        vfree(ds->currWord);
        vfree(ds);
    }

    pr_info("Device released\n");
    return 0;
}

// Read function
static ssize_t DeviceRead(struct file *fs, char __user *buffer, size_t size, loff_t *offset) {
    struct DeviceStruct *ds = fs->private_data;
    char userInput[56];
    char *translatedWord = NULL;

    if (!ds || !buffer || size < 1) {
        return -EINVAL;
    }

    if (copy_from_user(userInput, buffer, size)) {
        return -EFAULT;
    }
    userInput[size - 1] = '\0';  // Null-terminate the input

    if (ds->switched == 0) {
        for (int i = 0; i < 5; i++) {
            if (strcmp(userInput, ds->translateEngToSpanish[i]) == 0) {
                translatedWord = ds->translateSpanishToEng[i];
                break;
            }
        }
    } else {
        for (int i = 0; i < 5; i++) {
            if (strcmp(userInput, ds->translateSpanishToEng[i]) == 0) {
                translatedWord = ds->translateEngToSpanish[i];
                break;
            }
        }
    }

    if (!translatedWord) {
        pr_info("Translation not found for: %s\n", userInput);
        return -ENOENT;
    }

    if (copy_to_user(buffer, translatedWord, strlen(translatedWord) + 1)) {
        return -EFAULT;
    }

    pr_info("Translated: %s -> %s\n", userInput, translatedWord);
    return strlen(translatedWord);
}

// Write function
static ssize_t DeviceWrite(struct file *fs, const char __user *buffer, size_t size, loff_t *offset) {
    struct DeviceStruct *ds = fs->private_data;

    if (!ds || !buffer) {
        return -EINVAL;
    }

    ds->called++;
    pr_info("Write operation called %d times\n", ds->called);
    return size;
}

// IOCTL function
static long DeviceIoctl(struct file *fs, unsigned int cmd, unsigned long arg) {
    struct DeviceStruct *ds = fs->private_data;
    if (!ds) return -EINVAL;

    switch (cmd) {
        case IOCTL_SWITCH_MODE:
            ds->switched = (int)arg;
            pr_info("Translation mode switched to %d\n", ds->switched);
            return 0;
        default:
            return -EINVAL;
    }
}

// File operations
static struct file_operations fops = {
    .open = DeviceOpen,
    .write = DeviceWrite,
    .release = DeviceRelease,
    .read = DeviceRead,
    .unlocked_ioctl = DeviceIoctl,
    .owner = THIS_MODULE,
};

// Translator initialization
static int __init TranslatorInit(void) {
    int result;

    result = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (result < 0) {
        pr_err("Failed to allocate major number\n");
        return result;
    }

    cdev_init(&cDevice, &fops);
    cDevice.owner = THIS_MODULE;

    result = cdev_add(&cDevice, devno, 1);
    if (result < 0) {
        unregister_chrdev_region(devno, 1);
        pr_err("Failed to add cdev: %d\n", result);
        return result;
    }

    pr_info("Translator driver loaded with major %d, minor %d\n", MAJOR(devno), MINOR(devno));
    return 0;
}

// Closing function
static void __exit TranslatorExit(void) {
    cdev_del(&cDevice);
    unregister_chrdev_region(devno, 1);
    pr_info("Translator driver unloaded\n");
}

module_init(TranslatorInit);
module_exit(TranslatorExit);

