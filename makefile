# Set Project Name
PROJECT_NAME := ts0207_tz3000_zed

# Set the serial port number for downloading the firmware
DOWNLOAD_PORT := COM3

BOARD_NAME ?= ZG_222Z

BOARD_ZG_222Z  = 1
BOARD_ZG_222ZA = 2
BOARD_SNZB_05 = 3

ifeq ($(BOARD_NAME),ZG_222ZA)
	CHIP_FLASH_SIZE = 1024
	BOARD = $(BOARD_ZG_222ZA)
else
	ifeq ($(BOARD_NAME),ZG_222Z)
		CHIP_FLASH_SIZE = 512
		BOARD = $(BOARD_ZG_222Z)
	else
		ifeq ($(BOARD_NAME),SNZB_05)
			CHIP_FLASH_SIZE = 512
			BOARD = $(BOARD_SNZB_05)
		else
			CHIP_FLASH_SIZE = 512
			BOARD = $(BOARD_ZG_222Z)
		endif
	endif
endif



COMPILE_OS = $(shell uname -o)
LINUX_OS = GNU/Linux

ifeq ($(COMPILE_OS),$(LINUX_OS))	
	COMPILE_PREFIX = /opt/tc32/bin/tc32
else
	COMPILE_PREFIX = C:/TelinkSDK/opt/tc32/bin/tc32
	WIN32 = -DWIN32=1
endif

ifeq ($(CHIP_FLASH_SIZE),512)
	PFX_NAME = 512K
else
	ifeq ($(CHIP_FLASH_SIZE),1024)
		PFX_NAME = 1M
	else
		PFX_NAME = UNKNOWN
	endif
endif

AS      = $(COMPILE_PREFIX)-elf-as
CC      = $(COMPILE_PREFIX)-elf-gcc
LD      = $(COMPILE_PREFIX)-elf-ld
NM      = $(COMPILE_PREFIX)-elf-nm
OBJCOPY = $(COMPILE_PREFIX)-elf-objcopy
OBJDUMP = $(COMPILE_PREFIX)-elf-objdump
ARCH	= $(COMPILE_PREFIX)-elf-ar
SIZE	= $(COMPILE_PREFIX)-elf-size

LIBS := -lzb_ed -ldrivers_8258

DEVICE_TYPE = -DEND_DEVICE=1
MCU_TYPE = -DMCU_CORE_8258=1
BOOT_FLAG = -DMCU_CORE_8258 -DMCU_STARTUP_8258 -DCHIP_FLASH_SIZE=$(CHIP_FLASH_SIZE) -DBOARD=$(BOARD)

SDK_PATH := ./tl_zigbee_sdk
SRC_PATH := ./src
OUT_PATH := ./out
BIN_PATH := ./bin
MAKE_INCLUDES := ./make
TOOLS_PATH := ./tools
BOOT_FILE := $(OUT_PATH)/bootloader.bin
VERSION_RELEASE := V$(shell awk -F " " '/APP_RELEASE/ {gsub("0x",""); printf "%.1f", $$3/10.0; exit}' $(SRC_PATH)/include/version_cfg.h)
VERSION_BUILD := $(shell awk -F " " '/APP_BUILD/ {gsub("0x",""); printf "%02d", $$3; exit}' ./src/include/version_cfg.h)
ZCL_VERSION_FILE := $(shell git log -1 --format=%cd --date=format:%Y%m%d -- src |  sed -e "'s/./\'&\',/g'" -e "'s/.$$//'")
BOOT_SIZE := $(shell ls -l $(BOOT_FILE) | awk '{print $$5}')


TL_CHECK = $(TOOLS_PATH)/tl_check_fw.py
MAKE_OTA = $(TOOLS_PATH)/make_ota.py
MAKE_OTA_TUYA = $(TOOLS_PATH)/make_ota_tuya.py

INCLUDE_PATHS := \
-I$(SDK_PATH)/platform \
-I$(SDK_PATH)/proj/common \
-I$(SDK_PATH)/proj \
-I$(SDK_PATH)/zigbee/common/includes \
-I$(SDK_PATH)/zigbee/zbapi \
-I$(SDK_PATH)/zigbee/bdb/includes \
-I$(SDK_PATH)/zigbee/gp \
-I$(SDK_PATH)/zigbee/zcl \
-I$(SDK_PATH)/zigbee/ota \
-I$(SDK_PATH)/zbhci \
-I$(SRC_PATH) \
-I$(SRC_PATH)/include \
-I$(SRC_PATH)/zcl \
-I$(SRC_PATH)/common \
-I./common
 

LS_FLAGS := $(SDK_PATH)/platform/boot/8258/boot_8258.link

GCC_FLAGS := \
-ffunction-sections \
-fdata-sections \
-Wall \
-O2 \
-fpack-struct \
-fshort-enums \
-finline-small-functions \
-std=gnu99 \
-fshort-wchar \
-fms-extensions

ifeq ($(strip $(ZCL_VERSION_FILE)),)
GCC_FLAGS += \
-DBUILD_DATE="{8,'2','0','2','3','1','1','1','7'}"
else
GCC_FLAGS += \
-DBUILD_DATE="{8,$(ZCL_VERSION_FILE)}"
endif

ifeq ($(CHECK_BL),1)
VERSION_BUILD = 00
GCC_FLAGS += \
-DCHECK_BOOTLOADER \
-DVERSION_BUILD \
-DAPP_BUILD=0x00
endif
  
GCC_FLAGS += \
$(DEVICE_TYPE) \
$(MCU_TYPE) \
-DBOOT_SIZE=$(BOOT_SIZE) \
-DBOARD=$(BOARD) \
-DCHIP_FLASH_SIZE=$(CHIP_FLASH_SIZE)


OBJ_SRCS := 
S_SRCS := 
ASM_SRCS := 
C_SRCS := 
S_UPPER_SRCS := 
O_SRCS := 
FLASH_IMAGE := 
ELFS := 
OBJS := 
LST := 
SIZEDUMMY := 


RM := rm -rf

# All of the sources participating in the build are defined here
-include $(MAKE_INCLUDES)/zdo.mk
-include $(MAKE_INCLUDES)/zcl.mk
-include $(MAKE_INCLUDES)/wwah.mk
-include $(MAKE_INCLUDES)/ss.mk
-include $(MAKE_INCLUDES)/ota.mk
-include $(MAKE_INCLUDES)/mac.mk
-include $(MAKE_INCLUDES)/gp.mk
-include $(MAKE_INCLUDES)/common.mk
-include $(MAKE_INCLUDES)/bdb.mk
-include $(MAKE_INCLUDES)/aps.mk
-include $(MAKE_INCLUDES)/af.mk
-include $(MAKE_INCLUDES)/zbhci.mk
-include $(MAKE_INCLUDES)/proj.mk
-include $(MAKE_INCLUDES)/platformS.mk
-include $(MAKE_INCLUDES)/div_mod.mk
-include $(MAKE_INCLUDES)/platform.mk
-include ./project.mk

# Add inputs and outputs from these tool invocations to the build variables 
LOWER_NAME := $(shell echo $(BOARD_NAME) | tr [:upper:] [:lower:])
FIRMWARE_NAME := $(LOWER_NAME)_$(PFX_NAME)_$(VERSION_RELEASE).$(VERSION_BUILD)
FIRMWARE_FILE := $(FIRMWARE_NAME).bin
BOOT_FILE := $(BIN_PATH)/bootloader_$(PFX_NAME).bin
APPENDIX := $(BIN_PATH)/marker.bin
LST_FILE := $(OUT_PATH)/$(FIRMWARE_NAME).lst
BIN_FILE := $(BIN_PATH)/$(FIRMWARE_NAME).bin 
#$(OUT_PATH)/$(PROJECT_NAME).bin
ELF_FILE := $(OUT_PATH)/$(FIRMWARE_NAME).elf


SIZEDUMMY += \
sizedummy \


# All Target
all: pre-build main-build

flash:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s -m we 0x8000 $(BIN_FILE)

flash-512k:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s -m we 0 $(BIN_FILE)

flash-orig-write:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s -m we 0 $(BIN_PATH)/ts0207_tz3000_orig.bin
	
flash-orig-read-512k:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s -m rf 0 0x80000 ts0207_tz3000_orig.bin
	
flash-orig-read-1m:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s -m rf 0 0x100000 ts0207_tz3000_orig.bin
	
flash-orig-bootloader-read:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s -m rf 0 0x8000 ts0207_tz3000_orig_bootloadter.bin
	
erase-flash:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s ea
	
#0x0 0xfc000
#@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s es 0xff000 0x1000
	
erase-flash-fimware:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s es 0x8000 0xF4000
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s es 0xff000 0x1000

erase-flash-bootloader:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s es 0x0 0x8000

erase-flash-macaddr:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s es 0xFF000 0x1000

flash-bootloader:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s -m we 0 $(BOOTLOADER)
	
test-flash:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -s i

	

reset:
	@python3 $(TOOLS_PATH)/TlsrPgm.py -p$(DOWNLOAD_PORT) -z11 -a 100 -s -t50 -a2550 -m -w i


# Main-build Target
main-build: clean-project $(ELF_FILE) secondary-outputs

# Tool invocations
$(ELF_FILE): $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 C Linker'
	$(LD) --gc-sections -L $(SDK_PATH)/zigbee/lib/tc32 -L $(SDK_PATH)/platform/lib -L $(SDK_PATH)/platform/tc32 -T $(LS_FLAGS) -o "$(ELF_FILE)" $(OBJS) $(USER_OBJS) $(LIBS) 
	@echo 'Finished building target: $@'
	@echo ' '

# "$(BOOT_FILE).o"	

$(LST_FILE): $(ELF_FILE)
	@echo 'Invoking: TC32 Create Extended Listing'
	$(OBJDUMP) -x -D -l -S $(ELF_FILE)  > $(LST_FILE)
	@echo 'Finished building: $@'
	@echo ' '

 #$(BOOT_FILE)
 
ifeq ($(CHIP_FLASH_SIZE),512)
ifeq ($(BOARD_NAME),SNZB_05)
$(BIN_FILE): $(ELF_FILE)
	@echo 'Create Flash image (binary format)'
	@$(OBJCOPY) -v -O binary $(ELF_FILE)  $(BIN_FILE)
	@python3 $(TL_CHECK) $(BIN_FILE)
	@echo 'Create zigbee OTA file from' $(BIN_FILE)
	@python3 $(MAKE_OTA) -ot $(BOARD_NAME) $(BIN_FILE)
	@echo ' '
	@echo 'Create zigbee Tuya OTA file'
	@python3 $(MAKE_OTA_TUYA) -m 4742 -t 514 -o $(BIN_PATH)/1286-0202-1111114b-$(LOWER_NAME)-$(PFX_NAME).zigbee $(BIN_FILE) $(APPENDIX)
	@echo ' '
	@echo 'Finished building: $@'
	@echo ' '
else
$(BIN_FILE): $(ELF_FILE)
	@echo 'Create Flash image (binary format)'
	@$(OBJCOPY) -v -O binary $(ELF_FILE)  $(BIN_FILE)
	@python3 $(TL_CHECK) $(BIN_FILE)
	@echo 'Create zigbee OTA file from' $(BIN_FILE)
	@python3 $(MAKE_OTA) -ot $(BOARD_NAME) $(BIN_FILE)
	@echo ' '
	@echo 'Finished building: $@'
	@echo ' '
endif
else
ifeq ($(CHIP_FLASH_SIZE),1024)
ifeq ($(CHECK_BL),1)
$(BIN_FILE): $(ELF_FILE)
	@echo 'Create Flash image (binary format)'
	@$(OBJCOPY) -v -O binary $(ELF_FILE)  $(BIN_FILE)
	@python3 $(TL_CHECK) $(BIN_FILE)
	@echo 'Create zigbee Tuya OTA file'
	@python3 $(MAKE_OTA_TUYA) -m 4417 -t 54179 -o $(BIN_PATH)/1141-d3a3-1111114b-$(LOWER_NAME)-$(PFX_NAME).zigbee $(BIN_FILE) $(BOOT_FILE)
	@rm $(BIN_FILE)
	@echo ' '
	@echo 'Finished building: $@'
	@echo ' '
else
$(BIN_FILE): $(ELF_FILE)
	@echo 'Create Flash image (binary format)'
	@$(OBJCOPY) -v -O binary $(ELF_FILE)  $(BIN_FILE)
	@python3 $(TL_CHECK) $(BIN_FILE)
	@echo 'Create zigbee OTA file from' $(BIN_FILE)
	@python3 $(MAKE_OTA) -ot $(BOARD_NAME) $(BIN_FILE)
	@echo ' '
	@echo 'Finished building: $@'
	@echo ' '
endif
endif
endif
	

#$(OBJ_DIR)/bin_updater.o: $(OBJ_DIR)
#    @objcopy -I binary --output-target elf32-littlearm --rename-section .data=.bin_files ./updater.bin $@	 

sizedummy: $(ELF_FILE)
	@echo 'Invoking: Print Size'
	@$(SIZE) -t $(ELF_FILE)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	@echo $(INCLUDE_PATHS)
	-$(RM) $(FLASH_IMAGE) $(ELFS) $(OBJS) $(SIZEDUMMY) $(LST_FILE) $(ELF_FILE)
	-@echo ' '
	
# $(BIN_FILE)
#/$(PROJECT_NAME)_$(VERSION_RELEASE).$(VERSION_BUILD).bin 

clean-project:
	-$(RM) $(FLASH_IMAGE) $(ELFS) $(SIZEDUMMY) $(LST_FILE) $(ELF_FILE)
	-$(RM) -R $(OUT_PATH)/$(SRC_PATH)/*.o
	-@echo ' '

#	 $(BIN_FILE)
#/$(PROJECT_NAME)_$(VERSION_RELEASE).$(VERSION_BUILD).bin
	
pre-build:
	mkdir -p $(foreach s,$(OUT_DIR),$(OUT_PATH)$(s))
#	-" $(SDK_PATH)/tools/tl_link_load.sh" " $(SDK_PATH)/platform/boot/8258/boot_8258.link" "C:\TelinkSDK\SDK\tl_zigbee_sdk\build\tlsr_tc32/boot.link"
	-@echo ' '

post-build:
	-"$(TOOLS_PATH)/tl_check_fw.sh" $(OUT_PATH)/$(PROJECT_NAME) tc32
	-@echo ' '
	
secondary-outputs: $(BIN_FILE) $(LST_FILE) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents pre-build
.SECONDARY: main-build pre-build post-build
