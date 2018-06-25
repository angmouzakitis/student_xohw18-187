all: 
	gcc -I../../LINUX_KERNEL_MODULE test_setupsfw_WRITE.c -o run.x

run:
	./run.x | grep PASS | wc -l

clean:

	rm -rf ./run.x 

