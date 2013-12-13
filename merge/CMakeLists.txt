project(kwaymerge)

Set(merge_src main.cc k_merge_tree.h)
ADD_EXECUTABLE(test_merge ${merge_src})
