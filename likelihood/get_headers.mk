-include $(DEPS)

.PHONY : $(OBJS)

$(OBJS) : % :
	@echo $(filter %.hpp,$^)