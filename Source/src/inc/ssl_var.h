#ifndef P7_RSA_CTL_SRV__H
#define P7_RSA_CTL_SRV__H
#define RSA_CLIENT_CERT         "/usr/certificate/client.crt"
#define RSA_CLIENT_KEY          "/usr/certificate/client.key"

#define RSA_CLIENT_CA_CERT      "/usr/certificates/server_certificate.pem"
#define RSA_CLIENT_CA_PATH      "sys$common:[syshlp.examples.ssl]"

#define RSA_SERVER_CERT         "/usr/certificate/server.crt"
#define RSA_SERVER_KEY          "/usr/certificate/server.key"

#define RSA_SERVER_CA_CERT      "/usr/certificate/server_ca.crt"
#define RSA_SERVER_CA_PATH      "sys$common:[syshlp.examples.ssl]"

/*Cipher list to be used*/
#define CIPHER_LIST "AES128-SHA"


SSL_CTX              		*SslCtx;
SSL                  		*nSsl;
SSL_METHOD           		*SslMeth;


#endif

