EXECUTABLE := feedreader
BUILD_DIR := cmake-build-release
PACK := xsadov06


.PHONY: $(EXECUTABLE)
$(EXECUTABLE):
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug  ..
	cmake --build $(BUILD_DIR)
	cp $(BUILD_DIR)/$(EXECUTABLE) .
