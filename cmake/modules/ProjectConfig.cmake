# set LOG_USE_COLOR
if((${PROJECT_TARGET} STREQUAL "TARGET_RT1051"))
    set (LOG_USE_COLOR 0 CACHE INTERNAL "")
else()
    set (LOG_USE_COLOR 1 CACHE INTERNAL "")
endif()

# add LOG_SENSITIVE_DATA enable option
option(LOG_SENSITIVE_DATA "LOG_SENSITIVE_DATA" OFF)
if (${LOG_SENSITIVE_DATA} STREQUAL "ON")
    set (LOG_SENSITIVE_DATA_ENABLED 1 CACHE INTERNAL "")
else()
    set (LOG_SENSITIVE_DATA_ENABLED 0 CACHE INTERNAL "")
endif()

# add SystemView enable option
option(SYSTEMVIEW "SYSTEMVIEW" OFF)
if((${PROJECT_TARGET} STREQUAL "TARGET_RT1051") AND (${SYSTEMVIEW} STREQUAL "ON"))
    set (SYSTEM_VIEW_ENABLED 1 CACHE INTERNAL "")
    set (LOG_REDIRECT "RTT_SYSTEMVIEW" CACHE INTERNAL "")
else()
    set (SYSTEMVIEW 0FF CACHE INTERNAL "")
    set (SYSTEM_VIEW_ENABLED 0 CACHE INTERNAL "")
    set (LOG_REDIRECT "RTT_JLINK" CACHE INTERNAL "")
endif()

# add USB-CDC echo test enable option
option(USBCDC_ECHO "USBCDC_ECHO" OFF)
if (${USBCDC_ECHO} STREQUAL "ON")
    set (USBCDC_ECHO_ENABLED 1 CACHE INTERNAL "")
else()
    set (USBCDC_ECHO_ENABLED 0 CACHE INTERNAL "")
endif()

# add Debug LUART enable option
if (${LOG_REDIRECT} STREQUAL "RTT_LUART")
    set (LOG_LUART_ENABLED 1 CACHE INTERNAL "")
else()
    set (LOG_LUART_ENABLED 0 CACHE INTERNAL "")
endif()

# add Development Configuration option
option(WITH_DEVELOPMENT_FEATURES "Include development features" OFF)
set(DEVELOPER_SETTINGS_OPTIONS_DEFAULT ${WITH_DEVELOPMENT_FEATURES} CACHE INTERNAL "")
set(ENABLE_DEVELOPER_MODE_ENDPOINT_DEFAULT ${WITH_DEVELOPMENT_FEATURES} CACHE INTERNAL "")

# add Mudita USB Vendor/Product IDs
option(MUDITA_USB_ID "Enables using Mudita registered USB Vendor ID and Pure Phone USB Product ID" OFF)
if (MUDITA_USB_ID)
    set (USB_DEVICE_VENDOR_ID  0x3310 CACHE INTERNAL "Sets USB_DEVICE_VENDOR_ID to Mudita Vendor ID")
    set (USB_DEVICE_PRODUCT_ID 0x0100 CACHE INTERNAL "Sets USB_DEVICE_PRODUCT_ID to Mudita Pure Product ID")
else()
    set (USB_DEVICE_VENDOR_ID  0x045E CACHE INTERNAL "Sets USB_DEVICE_VENDOR_ID to Microsoft Vendor ID")
    set (USB_DEVICE_PRODUCT_ID 0x0622 CACHE INTERNAL "Sets USB_DEVICE_PRODUCT_ID to Windows MTP Simulator Product ID")
endif()

option(PURE_HW_TARGET "PURE_HW_TARGET" T7)
if (${PURE_HW_TARGET} STREQUAL "T6")
    message("Building for T6")
    set (MEMORY_LINKER_FILE "memory_T6.ld")
    set (PROJECT_CONFIG_USER_DYNMEM_SIZE 9*1024*1024 CACHE INTERNAL "")
    set (PURE_SDRAM_64_MB 0 CACHE INTERNAL "")
else()
    message("Building for T7 - 64MB SDRAM !")
    set (MEMORY_LINKER_FILE "memory_T7.ld")
    set (PROJECT_CONFIG_USER_DYNMEM_SIZE 28*1024*1024 CACHE INTERNAL "")
    set (PURE_SDRAM_64_MB 1 CACHE INTERNAL "")
endif()

#Config options described in README.md
set(PROJECT_CONFIG_DEFINITIONS
        LOG_USE_COLOR=${LOG_USE_COLOR}
        LOG_SENSITIVE_DATA_ENABLED=${LOG_SENSITIVE_DATA_ENABLED}
        LOG_REDIRECT=${LOG_REDIRECT}
        SYSTEM_VIEW_ENABLED=${SYSTEM_VIEW_ENABLED}
        USBCDC_ECHO_ENABLED=${USBCDC_ECHO_ENABLED}
        LOG_LUART_ENABLED=${LOG_LUART_ENABLED}
        PROJECT_CONFIG_USER_DYNMEM_SIZE=${PROJECT_CONFIG_USER_DYNMEM_SIZE}
        PURE_SDRAM_64_MB=${PURE_SDRAM_64_MB}
        USB_DEVICE_VENDOR_ID=${USB_DEVICE_VENDOR_ID}
        USB_DEVICE_PRODUCT_ID=${USB_DEVICE_PRODUCT_ID}
        MAGIC_ENUM_RANGE_MAX=256
        CACHE INTERNAL ""
        )

message(STATUS "BlueKitchen selected")
set(BT_STACK "BlueKitchen")

if(${PROJECT_TARGET} STREQUAL "TARGET_RT1051")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -Xlinker --gc-sections -Xlinker --sort-section=alignment -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -Xlinker -print-memory-usage -T ${LDSCRIPTSDIR}/libs.ld -T ${LDSCRIPTSDIR}/${MEMORY_LINKER_FILE} -T ${LDSCRIPTSDIR}/sections.ld -nostartfiles" CACHE INTERNAL "")
endif()
