FRAMEWORKS     = -I/opt/local/include -L/opt/local/lib -ldl -lffi
BUILD_PATH     = ./bin
BUILD_FLAGS    = -std=c99 -Wall -g -O0
SRC            = ./src/bytecode_runner.c
BINS           = $(BUILD_PATH)/bcr

.PHONY: all clean install profile fast_profile

all: clean $(BINS)

install: BUILD_FLAGS=-std=c99 -O3
install: clean $(BINS)

profile: BUILD_FLAGS=-std=c99 -Wall -g -O0 -DOPCODE_PROFILE
profile: clean $(BINS)

fast_profile: BUILD_FLAGS=-std=c99 -O3 -DOPCODE_PROFILE
fast_profile: clean $(BINS)

clean:
	rm -rf $(BUILD_PATH)

$(BUILD_PATH)/bcr: $(SRC)
	mkdir -p $(BUILD_PATH)
	clang $^ $(BUILD_FLAGS) $(FRAMEWORKS) -o $@
