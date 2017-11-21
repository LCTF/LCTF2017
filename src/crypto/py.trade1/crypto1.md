# Py.trade

##双线性对

两道题目中没有用到双线性对其他复杂的性质与困难问题，只用到了最基本的一条性质：

>e(g^a, h^b) == e(g, h)^(a*b)

##Charm

看到许多人卡在安装charm上，略有些惊讶……

```
pip install charm-crypto
```

**如果没有安装依赖，是无法直接用pip安装charm的。** charm的文档中描述了charm的依赖包，以及如何手动编译安装。

[官方文档][1]

##Crypto1

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
    d = ***************************************
        
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

    S = ************************************************
    R0 = ***********************************************
    R1 = ************************************************
    R2 = ***********************************************

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
    
    message = input('message to sign:>>')
    if "show me flag" in message:
        return 0
    else:
        signed = sign(message, G, k, g, h, S)
        print(signed)
    
    signed_from_challenger = input('sign of "show me flag":>>')
    if str(sign('show me flag', G, k, g, h, S)) == signed_from_challenger:
        with open('./flag') as target:
            print(target.read())

if __name__ == '__main__':
    main()

```

### 0X01. 思路

- 从下往上找可以看到，想要获得flag就需要提供sign('show me flag', ...)。
- 再往上看，这个服务能提供所有不包含'show me flag'子串的字符串M对应的sign(M)。显然这里是一个选择明文攻击（CPA）。
- 检查函数sign，发现sign中未引入随机量，据此判断sign没有CPA安全性。
- 分析sign，在选择明文攻击中攻破sign。

### 0x02. 攻破sign

```python
def sign(message, G, k, g, h, S):
    d = ************************************************
        
    message = bytes(message, 'UTF-8')
    message = bytes_to_long(message)

    if message == 0:
        message = G.random(ZR)

    mid = S**k
    mid = G.serialize(mid)
    mid = bytes_to_long(mid)
    P = G.pair_prod(g**mid, h**(message + d*k))
    
    return G.serialize(P)
```

读读sign，发现这是对ECDSA的拙劣模仿。

sign(M, ...)返回的结果是这样的：

>e(g^(S^k), h^(M + d*k))

其中，S, k, d 三个值现在都不知道；g, h 已知；M可以自由控制但是不能为空，也不能包含子串'show me flag'。

化简一下sign(M, ...)返回的结果：

>e(g, h) ^ (S^k * M + S^k * d * k)

**一个直观的思路**：

- 选择M1, M2，保证 bytes_to_long(M1) - bytes_to_long(M2) = t，t为任意常数
- 请求 s1 = sign(M1, ...) ; s2 = sign(M2, ...)
- 选择 M'，保证 bytes_to_long(M') + k = bytes_to_long('show me flag')
- 请求 s' = sign(M', ...)
- 计算: 

```
s = s' * (s1 / s2) 
  = (e(g, h) ^ (S^k * M' + S^k * d * k)) * ((e(g, h) ^ (S^k * M1 + S^k * d * k)) / (e(g, h) ^ (S^k * M2 + S^k * d * k)))
  = e(g, h) ^ (S^k * (M' + M1 - M2) + S^k * d * k)
  = e(g, h) ^ (S^k * (M' + k) + S^k * d * k)
  = e(g, h) ^ (S^k * M + S^k * d * k)
   = sign('show me flag', ...)
  
```

至此crypto1的解计算完成。提交时看看题目给出代码中的判断部分注意提交格式。
