all:
    gcc -o c1-term.out -DC1 clinet_input.c
    gcc -o c1-disp.out -DC1 clinet_monitor.c
    gcc -o c2-term.out -DC2 clinet_input.c
    gcc -o c2-disp.out -DC2 clinet_monitor.c
    gcc -o c3-term.out -DC3 clinet_input.c
    gcc -o c3-disp.out -DC3 clinet_monitor.c

clean:
    rm clinet_input clinet_monitor