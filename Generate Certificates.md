> 

## 1. generate key

```Python
#私钥
openssl genpkey -algorithm RSA -out private.pem
#公钥
openssl rsa -pubout -in private.pem -out public.pem
```



## 2. compile

```python
#服务端
cd server
gcc -g -o server *.c ../*.c ../Crypto/*.c -lssl -lcrypto
./server 

#客户端 
cd client
gcc -g -o client *.c ../*.c ../Crypto/*.c -lssl -lcrypto
./client 

```