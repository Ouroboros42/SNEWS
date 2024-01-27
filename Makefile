.PHONY = objects

obj_targs = $(addsuffix .o,$(basename $(1)))

common_srcs = $(wildcard common/*.cpp)
common_targs = $(call obj_targs,$(common_srcs))

$(common_targs): common/common.hpp

objects: $(common_targs)