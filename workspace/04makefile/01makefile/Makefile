test:
	make -C ./01src
	make -C ./02static
	make -C ./03dynamic
clean:
	make clean -C ./01src
	make clean -C ./02static
	make clean -C ./03dynamic
install:
	mv ./01src/main ./07bin
