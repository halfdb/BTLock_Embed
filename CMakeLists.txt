cmake_minimum_required(VERSION 3.5)
project(BTLock_Simulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c90")

set(SOURCE_FILES App.c Actor.c BTLock.c OSAL_SimpleBLEPeripheral.c simpleBLEPeripheral.c SimpleBLEPeripheral_Main.c)
add_definitions(-DINT_HEAP_LEN=3072
        -DOSAL_CBTIMER_NUM_TASKS=1
        -DHAL_AES_DMA=TRUE
        -DHAL_DMA=TRUE
        -DxPLUS_BROADCASTER
        -DPOWER_SAVING
        -DHAL_LCD=TRUE
        -DHAL_LED=TRUE)
add_executable(BTLock_Simulator ${SOURCE_FILES})