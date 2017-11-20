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
    d =*************************************

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

    S = ******************************************
    R0 = ******************************************
    R1 = ******************************************
    R2 = ******************************************

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
        with open('./' + token_str + '/flag') as target:
            print(target.read())

if __name__ == '__main__':
    main()
