default: all
moreclean: clean clean-dox
clean all clean-dox dox:
	for i in clase*.d*;do \
		$(MAKE) -C $$i $@;\
	done
changes: Changelog

Changelog:
	cg-log -s > $(@)

.PHONY: Changelog
