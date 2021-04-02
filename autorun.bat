@ echo off
:Start
	if not exist build\ goto Builder

:Runner
	start cmd /c build\Debug\circle_example.exe 0.6 0.5 32
	start cmd /c build\Debug\circle_example.exe 0.8 0.4 32 45 135
	start cmd /c build\Debug\circle_example.exe 0.8 0.7 32 0 270
	start cmd /c build\Debug\circle_example.exe 0.5 0.25 64 0 360 0 1 0
	exit

:Builder 
	if not exist shader\ goto Exception
	if not exist src\ goto Exception

	cmake . -Bbuild
	cmake --build build --config Debug

	goto Runner

:Exception
	echo ERROR:compile_folder_not_found_:(
	pause 
	goto Start

