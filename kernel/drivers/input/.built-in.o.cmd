cmd_drivers/input/built-in.o :=  arm-eabi-ld -EL    -r -o drivers/input/built-in.o drivers/input/input-core.o drivers/input/mousedev.o drivers/input/evdev.o drivers/input/keyboard/built-in.o drivers/input/misc/built-in.o 
