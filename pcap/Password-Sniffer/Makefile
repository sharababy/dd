CC = gcc
LD = gcc

# path #
SRC_PATH = src
BUILD_PATH = build

# code list #
SOURCES = $(wildcard $(SRC_PATH)/*.c)
OBJECTS = $(SOURCES:$(SRC_PATH)/%.c=$(BUILD_PATH)/%.o)
DEPENDENCIES = $(OBJECTS:%.o=%.d)s

# flags
INCLUDES = -I include

CCFLAGS = -Wall
LDFLAGS = -lpcap

all: directories program

# ------------------------------------------ #

.PHONY: directories
directories: ${BUILD_PATH}

${BUILD_PATH}:
	mkdir -p ${BUILD_PATH}

# ------------------------------------------ #

-include $(DEPENDENCIES)

.PHONY: program
program: password-sniffer

password-sniffer: ${OBJECTS}
	$(LD) $(LDFLAGS) $^ -o $@

${BUILD_PATH}/%.o: ${SRC_PATH}/%.c
	$(CC) $(LDFLAGS) $(CCFLAGS) $(INCLUDES) -MMD -c $< -o $@

# ------------------------------------------ #

.PHONY: clean
clean:
	rm -rf $(BUILD_PATH)
	rm -f password-sniffer

# ------------------------------------------ #