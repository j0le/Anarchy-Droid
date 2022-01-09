
// compile this with
//   go build -ldflags="-H windowsgui"
// to make this a Windows GUI application. Otherwise the call to AllocConsole will fail

package main

import (
	"os"
	"syscall"
	"time"

	"golang.org/x/sys/windows"
)

func main() {
	var kernel32_dll = windows.NewLazySystemDLL("Kernel32.dll")
	var user32_dll = windows.NewLazySystemDLL("User32.dll")

	var proc_GetConsoleWindow = kernel32_dll.NewProc("GetConsoleWindow")
	var proc_ShowWindow = user32_dll.NewProc("ShowWindow")
	var proc_AllocConsole = kernel32_dll.NewProc("AllocConsole")
	//var proc_FreeConsole = kernel32_dll.NewProc("FreeConsole")

	//proc_FreeConsole.Call()

	successful_alloc_console, _, _ := proc_AllocConsole.Call()
	if successful_alloc_console == 0 {
		os.Exit(22)
		return
	}

	//if err != syscall.Errno(0) {
	//	os.Exit(44)
	//	return
	//}

	var console_hwnd uintptr
	console_hwnd, _, err2 := proc_GetConsoleWindow.Call()

	if err2 != syscall.Errno(0) {
		os.Exit(2)
		return
	}

	if console_hwnd == 0 {
		os.Exit(3)
		return
	}

	for {

		proc_ShowWindow.Call(console_hwnd, 0) // hide

		time.Sleep(1 * time.Second)

		proc_ShowWindow.Call(console_hwnd, 5) // show

		time.Sleep(1 * time.Second)
	}

}
