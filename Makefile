BUILD_DIR=build
ARTIFACT=canvas-lib
PACKAGE_DIR=pkg

.PHONY: init
init:
	emcmake cmake -S . -B $(BUILD_DIR) -G "Ninja"

.PHONY: build
build:
	cmake --build $(BUILD_DIR)
	cp $(BUILD_DIR)/$(ARTIFACT).wasm $(PACKAGE_DIR)/$(ARTIFACT).wasm
	cp $(BUILD_DIR)/$(ARTIFACT).wasm demo/public/$(ARTIFACT).wasm
	cp $(BUILD_DIR)/$(ARTIFACT).data demo/public/$(ARTIFACT).data
	cp $(BUILD_DIR)/$(ARTIFACT).mjs $(PACKAGE_DIR)/$(ARTIFACT).mjs

clean:
	rm -rf $(BUILD_DIR)