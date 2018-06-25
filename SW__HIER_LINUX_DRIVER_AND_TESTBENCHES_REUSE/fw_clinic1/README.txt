Steps to run clinic1 

1)	make 		   : to create run_readok.x executable for clinic1
2a)	./run_readok.x     : to run the executable (read patient data)
2b)	./run_readdenied.x : to run the executable (invalid read patient data)
2c)	./run_writedenied.x: to run the executable (invalid write patient data)
3)	make clean 	   : to remove it

Or run all the scenarios with 'make runtests'. The logs are located in 'logs_all'.
