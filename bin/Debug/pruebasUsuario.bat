@ECHO off
::Archivos
for %%f in (Usuario*) do (
   for %%p in (100) do (
       for %%c in (0.8) do (
           for %%m in (0.075, 0.125) do (
               for %%s in (1) do (
                   sudoku.exe %%f %%p %%c %%m %%s
               )
           )
       )
   )
)
