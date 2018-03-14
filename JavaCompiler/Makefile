LIBS = -lpthread
TARGET = compiler
BUILD_DIR = build
SRCS = $(shell find . -path ./$(BUILD_DIR) -prune -o -name '*.cpp' -ls | awk '{$$1=$$2=$$3=$$4=$$5=$$6=$$7=$$8=$$9=$$10=""; print $0}' | sed 's/^\s*//' | grep -v -e '.zip\|.tar')
CC = g++
EXTRAFLAGS = -std=gnu++14

SHELL = /bin/bash
DEPENDENCY_LIST = $(BUILD_DIR)/depend
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

.PHONY: all clean

all: $(BUILD_DIR) $(DEPENDENCY_LIST) $(TARGET)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(EXTRAFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(EXTRAFLAGS) -c $< -o $@

$(DEPENDENCY_LIST): $(SRCS) | $(BUILD_DIR)
	$(RM) $(DEPENDENCY_LIST)
	$(CC) $(EXTRAFLAGS) -MM $^ | awk '{print "$(BUILD_DIR)/./" $$0;}' >> $(DEPENDENCY_LIST)

-include $(DEPENDENCY_LIST)

clean:
	$(RM) $(BUILD_DIR)/*.o *~ $(TARGET) $(DEPENDENCY_LIST)
