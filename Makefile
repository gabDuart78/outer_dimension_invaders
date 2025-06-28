CC = gcc
CFLAGS = -Wall -Iinclude -Werror
ALLEGRO_FLAGS = -lallegro -lallegro_primitives -lallegro_ttf -lallegro_font -lallegro_image \
				-lallegro_audio -lallegro_acodec -lallegro_memfile
OTHER_FLAGS = -lm

INCLUDE_DIR = include
OBJ_DIR = build
BIN_DIR = bin
GAME = outer_dimension_invaders
SRC_DIR = src
TARGET = $(BIN_DIR)/$(GAME)
MAKEFILE = Makefile
ASSETS = assets
SCORES_DIR = scores
SCORES_FILE = scores.dat

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(SRCS:.c=.o))

# Compile .c to .o in corresponding build dir 
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(SCORES_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) ${ALLEGRO_FLAGS} -c $< -o $@ $(OTHER_FLAGS)

# # Link binary
$(TARGET) : $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) ${CFLAGS} ${OBJS} ${ALLEGRO_FLAGS} -o $@ $(OTHER_FLAGS)

# Default target
all: $(OBJS)

# Run the game
run: $(TARGET)
	cd $(BIN_DIR) && ./$(GAME)

# Zip game artifacts
zip: all 	
	@mkdir -p $(GAME)
	@mkdir -p $(GAME)/$(SCORES_DIR)
	@cp -R $(SRC_DIR) $(GAME)/$(SRC_DIR)
	@cp -R $(INCLUDE_DIR) $(GAME)/$(INCLUDE_DIR)
	@cp -R $(ASSETS) $(GAME)/$(ASSETS)
	@cp $(MAKEFILE) $(GAME)
	zip -r $(GAME).zip $(GAME)
	rm -r $(GAME) 

# Clean all build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm $(SCORES_DIR)/$(SCORES_FILE) 

.PHONY: all clean run
