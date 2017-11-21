## 2ez4u writeup

1. 分配一个large chunk大小的块
2. 自己在堆上事先伪造好一个largechunk的头
3. 利用uaf来修改large chunk的bknextsize，让bknextsize指向这里(需要构造的合适一点，绕过glibc的检查)，效果就是能malloc出这块地方。
4. 之后就是很常规的利用这个malloc出来的chunk来泄露libc，修改fastbin的fd
5. 修改main_arena上的top为free_hook上面一些的地方
6. 通过几次malloc，修改free_hook为system的地址
