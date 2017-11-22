<?php
error_reporting(0);
session_start();
include('config.php');

if(!$_SESSION['isadmin']){
	die('You are not admin');
}

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
}
?>
<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>adminpage</title>
	<link href="css/bootstrap.min.css" rel="stylesheet">
    <script src="js/jquery.min.js"></script>
    <script src="js/bootstrap.min.js"></script>
</head>
<body>
	<nav class="navbar navbar-default" role="navigation">
   <div class="navbar-header">
      <a class="navbar-brand" href="#">后台</a>
   </div>
   <div>
      <ul class="nav navbar-nav">
         <li class="active"><a href="#">编辑文章</a></li>
         <li><a href="#">设置</a></li>
      </ul>
   </div></nav>
   <div class="panel panel-success">
   <div class="panel-heading">
      <h1 class="panel-title">文章列表</h1>
   </div>
   <div class="panel-body">
      <li><a href='?id=1'>Welcome to myblog</a><br></li>
      <li><a href='?id=2'>Hello,world!</a><br></li>
      <li><a href='?id=3'>This is admin page</a><br></li>
   </div>
   </div>
</body>
</html>
