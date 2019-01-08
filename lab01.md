# Lab 01

## Question1 : List atleast 50 paths with files & explain what it does.

Hardware Drivers in OS X are typically in the form of Kernel Extensions and the primary location is `/System/Library/Extensions/` however they can also be within an `Application Bundle`. You can use `kextunload` to unload a Kernel extension. To see what Kernel extensions are loaded use the `kextstat` command in a Terminal.

