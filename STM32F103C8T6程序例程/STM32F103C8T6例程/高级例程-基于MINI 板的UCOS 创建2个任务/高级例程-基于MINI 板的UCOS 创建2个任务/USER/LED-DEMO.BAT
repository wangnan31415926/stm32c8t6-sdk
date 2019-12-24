SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\ProgramData\Anaconda3;C:\ProgramData\Anaconda3\Library\mingw-w64\bin;C:\ProgramData\Anaconda3\Library\usr\bin;C:\ProgramData\Anaconda3\Library\bin;C:\ProgramData\Anaconda3\Scripts;C:\windows\system32;C:\windows;C:\windows\System32\Wbem;C:\windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\Microsoft VS Code\bin;C:\MinGW\bin;C:\Program Files (x86)\GnuWin32\bin;C:\Python27;C:\Python27\Scripts;C:\Program Files\Git\cmd;C:\Program Files (x86)\Nordic Semiconductor\nrf5x\bin\;C:\Program Files\PuTTY\;C:\Program Files\CMake\bin;C:\Program Files (x86)\Atmel\sam-ba_2.12\drv\;C:\Program Files (x86)\Atmel\sam-ba_2.12
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=LED-DEMO
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\startup_stm32f10x_hd._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\core_cm3.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\system_stm32f10x.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\bsp.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\led.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_core.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_flag.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_mbox.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_mem.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_mutex.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_q.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_sem.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_task.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_time.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_tmr.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\os_cpu_a._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_cpu_c.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\os_dbg.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\STM32-DEMO.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\STM32-DEMO.axf" --i32combined --output ".\STM32-DEMO.hex"
