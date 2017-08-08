This is a simple tool used to record input and output of a commandline tool. It is initially developped to use in ChainLp which will call kindlegen.exe
Example use(ChainLP)
rename the original kindlegen.exe to kindlegen_2.exe, put cmdfwd.exe in the same dir, run the gui tool. When it is running kindlegen.exe, the command line, environment vars, current directory will be saved in kindlegen.log. then it will try to call kindlegen_j.exe if it exists. then kindlegen_2.exe will be called with same command line and current directory settings. output will be saved in kindlegen.log.

This tool is useful to analysis how close sourced gui called command line tools and do extra stuff before calling the command line tools. for example, in kindlegen_j.exe, it can process the temp epub files before it is converted to mobi format(like fixing the toc error).
