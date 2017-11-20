#LCTF2017 Android Payme -最简单的题

##env
	1. teamtoken，message，金额
	2. 每队初始金钱1k

##思路
	1. 无加固，只有JNI_OnLoad函数里对APK签名做了验证，修改之后调试即可;
	2. 先submit做请求，然后pay进行支付;
	3. 首先encode得到teamtoken,实际上就是做了一次md5;
	4. 客户端先传参数到server，然后server sign=md5传回。split处理服务器传回的sign过的params string（message=xxx&order=x&teamtoken=xxx&sign=xxx）
	5. app再次求md5,得到signagain post到server，server验证sign和sianagain是否相同。
    
##漏洞点
	1. 取变量值是直接取split之后vector的固定位置，造成覆盖;
	2. server第二次checksign未check金额是否为正，因此此时可以修改post参数值进行充值;（需要覆盖sign值，自己md5求sign进行篡改。）