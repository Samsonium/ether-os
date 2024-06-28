kernel:
ifeq ($(shell make -sqC src/kernel || echo 1), 1)
	$(MAKE) -C src/kernel install
endif
