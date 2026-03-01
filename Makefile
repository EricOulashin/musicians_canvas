# Musician's Canvas - Multi-track Music Recording & Mixing
# Build using: make
#
# Dependencies (Ubuntu/Debian):
#   sudo apt install qt6-base-dev qt6-multimedia-dev libfluidsynth-dev librtmidi-dev cmake
#
# Dependencies (Fedora):
#   sudo dnf install qt6-qtbase-devel qt6-qtmultimedia-devel fluidsynth-devel rtmidi-devel cmake

.PHONY: all clean distclean run

BUILD_DIR = build

all:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. -DCMAKE_CXX_STANDARD=17 && $(MAKE)

.PHONY: run-virtual-midi-keyboard
run-virtual-midi-keyboard: all
	@./$(BUILD_DIR)/virtual_midi_keyboard

clean:
	@if [ -d $(BUILD_DIR) ]; then \
		cd $(BUILD_DIR) && $(MAKE) clean; \
	fi

distclean: clean
	@rm -rf $(BUILD_DIR)

run: all
	@./$(BUILD_DIR)/musicians_canvas
