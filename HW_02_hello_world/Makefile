# Путь к исходникам ядра (может потребоваться изменить)
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
# Текущая директория с исходниками
PWD := $(shell pwd)

CLANG_FORMAT_VERS ?= 19
CLANG_FORMAT := clang-format-$(CLANG_FORMAT_VERS)
CLANG_FORMAT_FLAGS += -i
FORMAT_FILES := $(SRC_DIR)/*.c

# Цель по умолчанию — сборка модулей
all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

format:
	$(CLANG_FORMAT) $(CLANG_FORMAT_FLAGS) $(FORMAT_FILES)

check:
	@echo "Complete"

# Очистка собранных файлов
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
