@echo ----------------------------------------------------------------
@echo Programacion Avanzada Parcheesi
@echo ----------------------------------------------------------------
@echo Proceso Iniciado
@echo ----------------------------------------------------------------

c:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat

cl main.cc parcheesi.cc player.cc piece.cc
cl.exe /c piece.cc
cl.exe /c player.cc
cl.exe /c parcheesi.cc
cl.exe /c main.cc

cl.exe *.obj /Efparcheesi.exe
@echo ----------------------------------------------------------------
@echo Proceso Finalizado :D
@echo ----------------------------------------------------------------