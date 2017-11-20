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
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
</head>
<body background="./images/1.jpg">
<object type="application/x-shockwave-flash" style="outline:none;" data="http://cdn.abowman.com/widgets/hamster/hamster.swf?" width="300" height="225"><param name="movie" value="http://cdn.abowman.com/widgets/hamster/hamster.swf?"></param><param name="AllowScriptAccess" value="always"></param><param name="wmode" value="opaque"></param></object>
<p style="color:orange">你好啊，但是你好像不是XDSEC的人,所以我就不给你flag啦~~</p>
</body>
</html>