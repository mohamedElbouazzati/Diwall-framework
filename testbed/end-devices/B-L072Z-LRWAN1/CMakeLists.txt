project(dummyMac)
cmake_minimum_required(VERSION 3.6)

# Set LoRaMac-node path
set(LoRaMacPath ${CMAKE_CURRENT_SOURCE_DIR}/LoRaMac-node/src)

# Add LoRaMac node repository
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LoRaMac-node/src)

#---------------------------------------------------------------------------------------
# Options
#---------------------------------------------------------------------------------------

# Allow switching of target platform
list(APPEND APPLICATION_LIST Lofrasea)

# Allow switching of secure-elements
set(SECURE_ELEMENT_LIST SOFT_SE LR1110_SE)
set(SECURE_ELEMENT SOFT_SE CACHE STRING "Default secure element is SOFT_SE")
set_property(CACHE SECURE_ELEMENT PROPERTY STRINGS ${SECURE_ELEMENT_LIST})

# Switch for USB-Uart support, enable it for some Applications who needs it.
option(USE_USB_CDC "Use USB-Uart" OFF)

# Switch for debugger support.
option(USE_DEBUGGER "Use Debugger" ON)

# Switch for Class B support of LoRaMac.
option(CLASSB_ENABLED "Class B support of LoRaMac" OFF)

#---------------------------------------------------------------------------------------
# Applications
#---------------------------------------------------------------------------------------
if(APPLICATION STREQUAL dummyMac)
	message("App dummyMac")
        #---------------------------------------------------------------------------------------
        # Target Boards: B-L072Z-LRWAN1 #TODO remove maybe
        #---------------------------------------------------------------------------------------
        # Configure toolchain for B-L072Z-LRWAN1
        set(LINKER_SCRIPT ${LoRaMacPath}/boards/B-L072Z-LRWAN1/cmsis/arm-gcc/stm32l072xx_flash.ld)
        include(stm32l0)

        # Build platform specific board implementation
        add_subdirectory(${LoRaMacPath}/boards/B-L072Z-LRWAN1)
        
        # Configure radio
        set(RADIO sx1276 CACHE INTERNAL "Radio sx1276 selected")
        
        #---------------------------------------------------------------------------------------
        # General Components
        #---------------------------------------------------------------------------------------
        
        add_subdirectory(${LoRaMacPath}/boards)
        add_subdirectory(${LoRaMacPath}/radio)
        add_subdirectory(${LoRaMacPath}/system)
        add_subdirectory(${LoRaMacPath}/mac)
        add_subdirectory(${LoRaMacPath}/peripherals)

	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dummyMac)
endif()
