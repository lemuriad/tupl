########################################################################################
# banners.cmake
#
# collection of font art for Universal

macro (print_ascii_banner)
    message("")
    message(" _________  ___  ___  ________  ___") 
    message("|\\___   ___\\\\  \\|\\  \\|\\   __  \\|\\  \\")
    message("\\|___ \\  \\_\\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\")
    message("     \\ \\  \\ \\ \\  \\\\\\  \\ \\   ____\\ \\  \\    ")
    message("      \\ \\  \\ \\ \\  \\\\\\  \\ \\  \\___|\\ \\  \\____")
    message("       \\ \\__\\ \\ \\_______\\ \\__\\    \\ \\_______\\")
    message("        \\|__|  \\|_______|\\|__|     \\|_______|")
    message("")
endmacro (print_ascii_banner)

####
# macro to print out a header
macro (print_blocks_banner)
    message("")
    message("")
endmacro (print_blocks_banner)

macro (print_header)
    print_ascii_banner()
endmacro (print_header)

macro (print_footer)
    print_ascii_banner()
endmacro (print_footer)
