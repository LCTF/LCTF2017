<?php
define("SECRET_KEY", "2d3d16890f5d0a2411f729797c85c106"); 

function get_random_id(){
    $random_id = '';
    $str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
    for($i = 0; $i < 15; $i++){
        $random_id .= substr($str, rand(1, 61), 1);
    }
    return $random_id;
}

$con = mysql_connect("localhost", "ctf", "ctf");
if (!$con){
  die('Could not connect: ' . mysql_error());
}
mysql_select_db("web1", $con);

if($_SERVER['REMOTE_ADDR'] === '127.0.0.1') {
    $id = 'admin';
}else{
    $id = get_random_id();
}
?>