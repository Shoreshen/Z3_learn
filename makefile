# $@  表示目标文件
# $^  表示所有的依赖文件
# $<  表示第一个依赖文件
# $?  表示比目标还要新的依赖文件列表
BRANCH = $(shell git symbolic-ref --short HEAD)
# git ====================================================================================
z3_pull:
	cd z3 && git pull
commit:
	git add -A
	@echo "Please type in commit comment: "; \
	read comment; \
	git commit -m"$$comment"
sync: commit
	git push -u z3_learn $(BRANCH)
reset_hard:
	git fetch && git reset --hard origin/$(BRANCH)

PHONY += commit sync sub_pull
# debug ==================================================================================
test_mirage.out: test/test_mirage.cpp
	g++ -g -o $@ $^ -Iz3/src/api/c++ -Iz3/src/api -Lz3/build -lz3

test_demogan.out: test/test_demogan.cpp
	g++ -g -o $@ $^ -Iz3/src/api/c++ -Iz3/src/api -Lz3/build -lz3
