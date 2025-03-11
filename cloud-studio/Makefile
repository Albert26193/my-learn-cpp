
# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow for silent output
VERBOSE ?= 0
ifeq ($(VERBOSE),0)
    QUIET := @
endif

# Suppress directory messages
MAKEFLAGS += --no-print-directory

# Project directory
PROJ_DIR := $(shell pwd)
BUILD_DIR := $(PROJ_DIR)/build
GTEST_DIR := $(PROJ_DIR)/tools/gtest

# Define phony targets (targets that don't create files)
.PHONY: all clean help build test install-gtest cmake

# Default target
all: help

# Install Google Test (integrating install_gtest.sh functionality)
install-gtest:
	@echo "Installing Google Test..."
	$(QUIET)cd $(PROJ_DIR)/tools/googletest && \
	cmake -DCMAKE_INSTALL_PREFIX=$(PROJ_DIR)/tools/gtest . && \
	make install
	@echo "Google Test installed successfully to $(GTEST_DIR)"

# Create build directory and run CMake
cmake: install-gtest
	@echo "Running CMake configuration..."
	$(QUIET)mkdir -p $(BUILD_DIR)
	$(QUIET)cd $(BUILD_DIR) && cmake ..
	@echo "CMake configuration complete"

# Build the project
build: cmake
	@echo "Building project..."
	$(QUIET)cd $(BUILD_DIR) && make
	@echo "Build complete"

# Build and run tests
test: build
	@echo "Running tests..."
	$(QUIET)cd $(BUILD_DIR) && ./unit_test
	@echo "Tests complete"

# Just build the code (similar to cmake + build but with a more intuitive name)
include: build

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	$(QUIET)rm -rf $(BUILD_DIR)
	@echo "Clean complete"

clean-binary:
	@echo "Cleaning build artifacts..."
	$(QUIET)rm -rf $(BUILD_DIR)
	@echo "Clean complete"

# AWK-generated help section
help:
	@echo "Available targets:"
	@echo ""
	@awk 'BEGIN {FS = ":.*?# "; printf "\033[36m%-20s\033[0m %s\n", "Target", "Description"}' $(MAKEFILE_LIST)
	@awk 'BEGIN {FS = ":.*?# "} /^[a-zA-Z_-]+:.*?# / {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)
	@echo ""

# Target descriptions for help
all: # Default target (shows help)
install-gtest: # Install Google Test to the tools/gtest directory
cmake: # Configure the project with CMake
build: # Build the project using CMake
test: # Build and run the unit tests
include: # Alias for build target
clean: # Remove build artifacts
help: # Display this help message