## toy writeup

### 漏洞

程序修改自https://github.com/skx/simple.vm

在peek和poke处修改了一下，把检查地址是否越界的部分给改了

### 利用

利用方法可能有很多，下面是我写exp的步骤

1. 通过store-string来malloc出合适的chunk，然后将其free
2. 将被free的chunk里的libc和heap的地址通过peek来读取到寄存器里
3. 修改被free的fastbin的fd，使其指向第10个寄存器（刚好在exit指令的函数句柄上面）_
4. 通过得到的libc地址来计算出system的地址，用concat指令来写入到exit指令的函数句柄上

