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
cd server
gcc -g -o server *.c ../*.c ../Crypto/*.c -lssl -lcrypto
./server 
#-DSERVER指明是不是服务端，-DDHPATH指明DH公钥存放的位置， -DFTPPATH指明连接成功后服务端的当前路径


cd client
gcc -g -o client *.c ../*.c ../Crypto/*.c -lssl -lcrypto
./client 

```