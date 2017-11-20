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