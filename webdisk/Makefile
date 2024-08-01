# 目录定义
SRC_DIR := src
LIB_DIR := lib
OUT_DIR := bin

# 可执行文件
OUT := $(OUT_DIR)/main

# 源文件和中间文件
SRCS := $(wildcard $(SRC_DIR)/*.cc)
OBJS := $(patsubst $(SRC_DIR)/%.cc,$(LIB_DIR)/%.o,$(SRCS))

# 编译选项和编译器
COM_OP := -Wall -g -lhiredis -lredis++ -lpthread 
CC := g++

# 生成目标文件
$(OUT) : $(OBJS)
	$(CC) $^ -o $@ $(COM_OP)

# 生成中间文件规则
$(LIB_DIR)/%.o : $(SRC_DIR)/%.cc
	$(CC) -c $< -o $@ $(COM_OP)

# 清理和重建规则
.PHONY : clean rebuild
clean:
	rm -f $(OUT) $(OBJS)
rebuild: clean $(OUT)
