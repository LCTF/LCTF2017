<?php
error_reporting(0);
session_start();
define("METHOD", "aes-128-cbc");
include('config.php');

function show_page(){
    echo '<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Login Form</title>
  <link rel="stylesheet" type="text/css" href="css/login.css" />
</head>
<body>
  <div class="login">
    <h1>后台登录</h1>
    <form method="post">
        <input type="text" name="username" placeholder="Username" required="required" />
        <input type="password" name="password" placeholder="Password" required="required" />
        <button type="submit" class="btn btn-primary btn-block btn-large">Login</button>
    </form>
</div>
</body>
</html>
';
}

function get_random_token(){
    $random_token = '';
    $str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
    for($i = 0; $i < 16; $i++){
        $random_token .= substr($str, rand(1, 61), 1);
    }
    return $random_token;
}

function get_identity(){
	global $id;
    $token = get_random_token();
    $c = openssl_encrypt($id, METHOD, SECRET_KEY, OPENSSL_RAW_DATA, $token);
    $_SESSION['id'] = base64_encode($c);
    setcookie("token", base64_encode($token));
    if($id === 'admin'){
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

if(isset($_POST['username'])&&isset($_POST['password'])){
	$username = mysql_real_escape_string($_POST['username']);
	$password = $_POST['password'];
	$result = mysql_query("select password from users where username='" . $username . "'", $con);
	$row = mysql_fetch_array($result);
	if($row['password'] === md5($password)){
  		get_identity();
  		header('location: ./admin.php');
  	}else{
  		die('Login failed.');
  	}
}else{
	if(test_identity()){
        header('location: ./admin.php');
	}else{
        show_page();
    }
}
?>
