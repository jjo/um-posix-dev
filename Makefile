default: all
clean all:
	for i in clase*.d*;do \
		$(MAKE) -C $$i $@;\
	done
