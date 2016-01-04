# sun - socket unmasker

Simple utility to reveal socket information hiding beyond bare `ls -l` output over some PID.  
Usage: `sh sun.sh <PID>`  
One may try also to narrow output by using a simple **grep**, e.g.:  
```
sh sun.sh $(pgrep firefox) | grep socket
```
This would print sockets information of a running Firefox instance.