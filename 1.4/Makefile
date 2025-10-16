# Usage:
#   make run NAME=b_5
#   make run NAME=b_5 ASAN=1
#   make run NAME=b_5 DBG=1
#   make run NAME=b_5 ASAN=1 DBG=1
#   make build NAME=b_5 [ASAN=1] [DBG=1]
#   make clean

# -------- settings --------
NAME      ?=
SRC       := $(NAME).c
BIN       := $(NAME)

CFLAGS    := -Wall -Wextra -O2
LDFLAGS   := -lpthread

# Флаги от пользователя
ASAN      ?= 0
DBG       ?= 0

# AddressSanitizer
ifeq ($(ASAN),1)
CFLAGS  += -fsanitize=address -fno-omit-frame-pointer
LDFLAGS += -fsanitize=address
endif

# Отладочная информация
ifeq ($(DBG),1)
CFLAGS  += -g
endif

# -------- targets --------
.PHONY: build run clean help

build:
	@if [ -z "$(NAME)" ]; then \
		echo "❗ Укажи имя файла без .c:  make build NAME=b_5 [ASAN=1] [DBG=1]"; \
		exit 1; \
	fi
	@if [ ! -f "$(SRC)" ]; then \
		echo "❗ Файл $(SRC) не найден"; \
		exit 1; \
	fi
	@echo "→ gcc $(SRC) -o $(BIN) $(CFLAGS) $(LDFLAGS)"
	@gcc $(SRC) -o $(BIN) $(CFLAGS) $(LDFLAGS)

run: build
	@echo "→ ./$(BIN)"
	@./$(BIN)

clean:
	@echo "→ удаляю бинарники (всё без расширения, кроме Makefile и .c/.h)"
	@find . -maxdepth 1 -type f \
		! -name '*.c' ! -name '*.h' \
		! -name 'Makefile' ! -name '.gitignore' \
		-exec rm -f {} +

help:
	@echo "Использование:"
	@echo "  make run  NAME=<имя_без_.c> [ASAN=1] [DBG=1]"
	@echo "  make build NAME=<имя_без_.c> [ASAN=1] [DBG=1]"
	@echo "Примеры:"
	@echo "  make run NAME=b_5"
	@echo "  make run NAME=b_5 ASAN=1"
	@echo "  make run NAME=b_5 DBG=1"
	@echo "  make run NAME=b_5 ASAN=1 DBG=1"
