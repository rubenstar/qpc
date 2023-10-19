add_library(qpcQxklib qxk_mutex.c qxk_sema.c qxk_xthr.c qxk.c)
target_include_directories(qpcQxklib PUBLIC ../../include ../../ports/arm-cm/qxk/gnu)