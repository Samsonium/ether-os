kernel:
ifeq ($(shell make -sqC src/kernel || echo 1), 1)
	$(MAKE) -C src/kernel install
	$(MAKE) -C src/kernel after
endif

clean:
	find . -name \*.o -type f -delete
	find . -name \*.d -type f -delete
	rm -f src/kernel/kernel.bin
