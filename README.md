## dbgctrl  
Nintendo Switch Debugger Library.  

## API specifications  
[Here](https://unvirus.github.io/dbgctrl/index.html)  

## Sample Source Code  
Template is [here](https://github.com/unvirus/dbgctrl/tree/main/sysmodule). (including libdbgctrl.a)    
The original source code is [here](https://github.com/switchbrew/switch-examples/tree/master/templates/sysmodule).  

## Configure
Modify sysmodule.json to enable this library.  
**"lowest_cpu_id":3** to **"lowest_cpu_id":0**  

Place libdbgctrl.a anywhere and edit Library path in Makefile.  


  
