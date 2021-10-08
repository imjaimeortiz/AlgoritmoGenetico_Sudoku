@ECHO off
::Archivos
for %%f in (Caso*) do (
   for %%p in (100, 150) do (
       for %%c in (0.8, 0.85, 0.9, 0.95) do (
           for %%m in (0.05, 0.075,0.1, 0.125) do (
               for %%s in (0, 1) do (
                   sudoku.exe %%f %%p %%c %%m %%s
               )
           )
       )
   )
)
