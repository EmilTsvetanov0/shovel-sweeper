BUILD_DIR ?= build
EXECUTABLE ?= shovel-sweeper.exe

# Параметры по умолчанию
HEIGHT ?= 8
WIDTH ?= 8
MINES ?= 10

ifeq ($(OS),Windows_NT)
    RM = rmdir /S /Q
    MKDIR = mkdir
    RUN = .\\
else
    RM = rm -rf
    MKDIR = mkdir -p
    RUN = ./
endif

.PHONY: all
all: build run

.PHONY: configure
configure:
	$(MKDIR) $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G Ninja ..

.PHONY: build
build: configure
	cmake --build $(BUILD_DIR)

.PHONY: run
run:
	$(RUN)$(BUILD_DIR)/$(EXECUTABLE) -h $(HEIGHT) -w $(WIDTH) -m $(MINES)

.PHONY: run-beginner
run-beginner:
	$(MAKE) run HEIGHT=8 WIDTH=8 MINES=10

.PHONY: run-intermediate
run-intermediate:
	$(MAKE) run HEIGHT=16 WIDTH=16 MINES=40

.PHONY: run-expert
run-expert:
	$(MAKE) run HEIGHT=16 WIDTH=30 MINES=99

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)

.PHONY: rebuild
rebuild: clean all