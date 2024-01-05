
project(FTPS)

cmake_minimum_required(VERSION 3.24)

include_directories(.)
include_directories(client)
include_directories(server)

set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib)

set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib)

#依次设置头文件目录、库目录、要链接的库 (在add_executable 之前)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(openssl libcrypto)

add_executable(
        FTPS network.c
        network.h IO.c IO.h
        tls.c tls.h
        client/protocol_client.c client/protocol_client.h client/Client.c
        server/protocol_server.h server/Server.c client/init_client.c client/init_client.h client/Communicate.c client/Communicate.h client/Command_get.c client/Command_get.h client/Login.c client/Login.h client/Path_operation.c client/Path_operation.h client/File_operation.c client/File_operation.h client/TLS_com.c client/TLS_com.h client/Up_download.c client/Up_download.h server/init_server.c server/init_server.h server/login.c server/login.h server/up_download.c server/up_download.h server/tls_com.c server/tls_com.h server/command_deal.c server/command_deal.h server/file_operation.c server/file_operation.h server/communication.c server/communication.h server/path_operation.c server/path_operation.h Crypto/msg.h Crypto/gcm_encrypto.c Crypto/gcm_encrypto.h Crypto/secure_communication.c Crypto/secure_communication.h)

target_link_libraries(FTPS openssl)
