## dbgctrl 
Nintendo Switch Debugger Library(libdbgctrl.a).  
This is a library for creating a simple tiny debugger.  

## Change log  
2024/02/20  
Supported software breakpoints.  

## API specifications  
[Here](https://unvirus.github.io/dbgctrl/index.html)  

## Sample Source Code  
Template is [here](https://github.com/unvirus/dbgctrl/tree/main/sysmodule). (including dbgctrl Library)    
The original source code is [here](https://github.com/switchbrew/switch-examples/tree/master/templates/sysmodule).  

## Configure
Modify sysmodule.json to enable this library.  
**"lowest_cpu_id":3** to **"lowest_cpu_id":0**  

Place libdbgctrl.a anywhere and edit Library path in Makefile.  


  
