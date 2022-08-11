# sudo apt install linux-headers-$(uname -r)
# sudo ln -s asm-generic asm
# sudo chmod 777 -R include

obj-m += hello_module_params.o

.RECIPEPREFIX = 	

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean