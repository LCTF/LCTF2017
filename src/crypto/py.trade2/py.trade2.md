# py.trade2


### 0X00. 题目原文

```python
#!/usr/bin/env python3
from charm.toolbox.pairinggroup  import *
from Crypto.Util.number import long_to_bytes, bytes_to_long, inverse
from urllib import parse, request

logo = """
_|          _|_|_|  _|_|_|_|_|  _|_|_|_|  
_|        _|            _|      _|        
_|        _|            _|      _|_|_|    
_|        _|            _|      _|        
_|_|_|_|    _|_|_|      _|      _|        



  _|_|      _|      _|  _|_|_|_|_|  
_|    _|  _|  _|  _|_|          _|
    _|    _|  _|    _|        _|
  _|      _|  _|    _|      _|
_|_|_|_|    _|      _|    _|
"""

def sign(message, G, k, g, h, S):
    d = ********************************************
        
    message = bytes(message, 'UTF-8')
    message = bytes_to_long(message)

    if message == 0:
        message = G.random(ZR)

    mid = S**k
    mid = G.serialize(mid)
    mid = bytes_to_long(mid)
    P = G.pair_prod(g**mid, h**(message + d*k))
    
    return G.serialize(P)
    

def check_token(token, name):
    url = 'http://lctf.pwnhub.cn/api/ucenter/teams/'+token+'/verify/'
    req = request.Request(url=url)
    try:
        res = request.urlopen(req)
        if res.code == 200:
            return True
    except :
        pass

    return False    
           

def main():
    print(logo)

    S = ***********************************************
    R0 = ************************************************
    R1 = ************************************************
    R2 = ************************************************

    S1 = S + R0
    S2 = S + R0*2

    G = PairingGroup('SS512')
    g = b'1:HniHI3b/eK111pzcIdKZKJCK9S7QiL5xItmJ9iTvEaGGEVuM4hGc2cMRqhNwsV29BN/QpqhopD+2XgUaTdQMqQA='
    h = b'2:OGpVSq03JR4dWKsDZ+6DBJ6Qwy2E4jaNA6HsWJZNP1vhHe2wYjLUvw990iouBG8XQVEbKr+uLNc3k9n4JDAJOgA='
    g = G.deserialize(g)
    h = G.deserialize(h)

    token_str = input("token:>>")
    name = input("team name:>>")
    if not check_token(token_str, name):
        return 0
    else:
        try:
            token = bytes(token_str,'UTF-8')
            token = bytes_to_long(token)
        except :
            return 0
    
    if token%2 ==1:
        point = G.pair_prod(g**token, h**R1) * G.pair_prod(g**S1, h)
    else:
        point = G.pair_prod(g**token, h**R2) * G.pair_prod(g**S2, h)
    print(G.serialize(point))

    S = G.pair_prod(g,h)**S
    k = G.serialize(S)
    k = bytes_to_long(k)
    
    message = 'abcd'
    signed = sign(message, G, k, g, h, S)
    print('signed of "abcd":>>', signed)
    
    signed_from_challenger = input('sign of "show me flag":>>')
    if str(sign('show me flag', G, k, g, h, S)) == signed_from_challenger:
        with open('./flag2') as target:
            print(target.read())

if __name__ == '__main__':
    main()

```

### 0X01. 思路

比赛结束前三小时crypto2放出了hint：“这不止是一道crypto题目，它还是一道……”

两道题目只有几行代码不同，crypto2中**不允许**用户提交自己的字符串。只会返回sign('abcd', ...)。无法选择明文了，允许输入的地方只有三个：token，队名，和最后的变量 signed_from_challenger。其中‘队名’这个变量是打酱油的，丝毫用处都没有。（此处偷偷谴责一下写token校验api的兄台 :-P）

读完代码后应该可以意识到crypto2里sign函数也几乎是打酱油的，全程只有可能执行sign('abcd')与sign('show me flag')。

那么问题肯定出在前面那一坨代码上了。

读一下前面的代码，意识到前面的代码其实是在双线性对映射出的那个群中一点e(g, h)的指数上实现了一个两层递归的 [shamir门限方案][2]。这个shamir树型结构也是CP-ABE（Cipher Policy - Attributes Based Encryption）的基础结构。

图示：

![shamir递归树][3]

题目中如下代码实现了这颗树。

```
if token%2 ==1:
    point = G.pair_prod(g**token, h**R1) * G.pair_prod(g**S1, h)
else:
    point = G.pair_prod(g**token, h**R2) * G.pair_prod(g**S2, h)
print(G.serialize(point))
```

在实际代码中，奇数选手将得到 e(g, h)^(token*R1 + S1)；偶数选手将得到 e(g, h)^(token*R2 + S2)。从更靠前的代码可以看到S1，S2的来源：已知S1，S2时，它们组成了一个简单的二元一次方程组。如果想要恢复出sign函数输入中的S和k，就需要先拿到S，或者拿到S的一些特征，比如说e(g, h)^S。

```
S1 = S + R0
S2 = S + R0*2
```

对于S，如果只知道S1，或者只知道S2，是无法解出S的。毕竟“K元一次方程需要至少K个一组才可能有解，否则一定有无穷多解”。

对于选手能直接得到的 e(g, h)^(token*R1 + S1) ，有两个未知量R1，S1，在只有一个token时也是有无穷多解的。因此需要两个奇数token，两个偶数token才有可能恢复出S（这里对应给出的hint：它不只是一道crypto题目，它还是一道社工题）。在实际操作中，我们只能恢复到e(g, h)^S，不过这已经足够我们求出'show me flag'的sign啦。

因为我们只能得到'abcd'的sign，即 e(g^(S^k), h^('abcd' + d*k))
（注：此处代码前，S被赋值成了e(g, h)^S，详情见题目原文）
在我们能得到S = e(g, h)^S，且可以根据S求得k时，我们就可以给任意消息做签名了。

### 0X02. writeUp.py

```
from charm.toolbox.pairinggroup import *
from Crypto.Util.number import bytes_to_long, inverse

def main():
    G = PairingGroup('SS512')
    g = b'1:HniHI3b/eK111pzcIdKZKJCK9S7QiL5xItmJ9iTvEaGGEVuM4hGc2cMRqhNwsV29BN/QpqhopD+2XgUaTdQMqQA='
    h = b'2:OGpVSq03JR4dWKsDZ+6DBJ6Qwy2E4jaNA6HsWJZNP1vhHe2wYjLUvw990iouBG8XQVEbKr+uLNc3k9n4JDAJOgA='

    # 4 different token. 2 even 2 odd
    t1 = bytes_to_long(b'4795968fe0bf73a1e39e6fec844dee01')
    S1 = b'3:cOlYveeItjU4ZHh8B58RjWUYJwdtFi/FXzqtd2GnnqEMJ9AFKzNjV90eUoPDLkinkWsdmbYTJxFTq5bvucwVHE98Uvw2laNvrsCFY9Mw766YdEPAtj7smBt/tIDl+u1mORufxZX8Q31F3dJjnzEoYhlxRZ9e9JFVtK7nW2Di6Iw='
    t2 = bytes_to_long(b'f11ca9db1f547b71a1b9592659553814')
    S2 = b'3:NjqqiCxaQtlFS1FEsSD+jmuO9Z0srysMi4K1nVCg2yAxJRjX62PPMSbY5JAa+Y4Ap25p9+u1EZ05f1RSwOXyZiIAZoDoS0crKDHRLJtE40aswcnPaf1JklMGBOGLdBUOZ3+nknLRDLACyBFnTW8y6FnHzLICGruBHisLhschvHM='
    t3 = bytes_to_long(b'97fddec1d9e630075803fc67d4220b05')
    S3 = b'3:GYcIbust8E1tcYZghIgC4x6YhrAyJUvy0lHHUxfvIOD7S/ann03RFrhO4qKb0jQ4vcU7pHJPv9Q+WDDPV/mAcH224dIfSyGcv91adl0tuhS6z0Fr4tBz03YUFUcGvAvi7bHvjnywwAjkTe1ZmMybyUnc9bMTPUxIZ3kli2b3PRs='
    t4 = bytes_to_long(b'adafcd958bbe176dd9cc96ef3aaa6438')
    S4 = b'3:R7Zhznj9aRtEv9ifZfLf9aqt4PSZzrMCSXuxkwZDdLEC2pqRPC1dWtP41BLR0UbbZVbTyOuojif9HYVuDu7oFSMTtj3zUxwXUW2x5sCYnkY3MOhSKM9JJxzAktSF0H2rIVvw4iBhQoh6Ecy3qRYfjZSha4Bc729DXHbYx0sMxd4='

    # SS512's order. get from G.order()
    order = 730750818665451621361119245571504901405976559617

    #init
    g = G.deserialize(g)
    h = G.deserialize(h)
    S1 = G.deserialize(S1)
    S2 = G.deserialize(S2)
    S3 = G.deserialize(S3)
    S4 = G.deserialize(S4)

    #compute x**S1
    t_S1 = ((S1**t3)/(S3**t1))**inverse((t3-t1), G.order())
    print(t_S1)

    #compute x**S2
    t_S2 = ((S2**t4)/(S4**t2))**inverse((t4-t2), G.order())
    print(t_S2)

    #compute x**S
    t = (t_S1*t_S1)/t_S2
    print(t)

    #compute k
    k = G.serialize(t)
    k = bytes_to_long(k)

    #compute mid
    mid = t**k
    mid = G.serialize(mid)
    mid = bytes_to_long(mid)

    #compute sign
    signed = b'3:loZKMHi9WWkS46zTQyidX5546U2Sg/JLnNi18X2KRklZdJSth4Kyj5FPg0J8sVpc9hyClgIo2P8xOGsRK6Zxc2AW6euFkyaOUWI9ZmYp2AhE0kcOypR4vASF9vWYtNqj0qlsExtMThSUtS53HYHCczbxcxA2Vcr/tkFagicyU30='
    signed = G.deserialize(signed)
    message = bytes_to_long(b'abcd')
    signed = signed/(G.pair_prod(g, h)**(mid*message))
    
    message = bytes_to_long(b'show me flag')
    signed = signed*(G.pair_prod(g, h)**(mid*message))
    print(str(G.serialize(signed)))


if __name__ == '__main__':
    main()
```

  [1]: https://jhuisi.github.io/charm/install_source.html
  [2]: https://en.wikipedia.org/wiki/Shamir%27s_Secret_Sharing
  [3]: http://on-img.com/chart_image/5a12706de4b0143a78b4a9d4.png