
PostBuild_Cmds=cmd\RenameByTime Debug\PeTool.exe D
PostBuild_Cmds=cmd\RenameByTime Release\PeTool.exe R

PostBuild_Cmds=cmd\CopyByTime Debug\PeTool.exe D
PostBuild_Cmds=cmd\CopyByTime Release\PeTool.exe R

PostBuild_Cmds=cscript cmd\CopyByTime.vbs Debug\PeTool.exe D
PostBuild_Cmds=cscript cmd\CopyByTime.js Release\PeTool.exe R

PostBuild_Cmds=copy /Y Debug\PeTool.exe Debug\PeTool%date:~0,4%%date:~5,2%%date:~8,2%%time:~0,2%%time:~3,2%_D.exe
PostBuild_Cmds=copy /Y Release\PeTool.exe Release\PeTool%date:~0,4%%date:~5,2%%date:~8,2%%time:~0,2%%time:~3,2%_R.exe

PostBuild_Cmds=echo F|xcopy /Y Debug\PeTool.exe Debug\PeTool%date:~0,4%%date:~5,2%%date:~8,2%%time:~0,2%%time:~3,2%_D.exe
PostBuild_Cmds=echo F|xcopy /Y Release\PeTool.exe Release\PeTool%date:~0,4%%date:~5,2%%date:~8,2%%time:~0,2%%time:~3,2%_R.exe