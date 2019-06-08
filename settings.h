

// ***** Zabbix configurações relacionadas *****
// zabbix server or proxy - IP ou FQDN
const char * server = "IP_SERVER";

// zabbix trapper port
const uint16_t port = 10051;

// hostname do host no Zabbix
const char * hostname = "hostname_arduino";

// ***** WiFi configurações relacionadas *****
// WiFi SSID para conectar (case sensitive)
const char * ssid = "WIFISSID";

// WiFi Senha
const char * wifiKey = "PASSWORD";


// ***** Sensor configurações relacionadas *****
// enviar o intervalo de dados em segundos após a conexão e envio de dados 
const int sendDataIntervallSec = 60;


// ***** Application configurações relacionadas *****
// Número de ciclos de conexão Wi-Fi antes de ir dormir profundamente
int maxWifiConnectcycles = 20;
// tempo limite máximo de conexão em ms após conexão wifi e espera por conexão ao zabbix-server
const int maxConnectionTimeout = 5 * 1000;
// tempo limite máximo de transmissão em ms após conexão ao zabbix-server e início do envio de dados
const int maxTransmissionTimeout = 5 * 1000;

// ***** LEDs *****  
// on Errors
int RED_LED = 16;
// on enquanto espera por wlan e envia dados
int GREEN_LED = 2; // LOW para ligar e HIGH para desligar

// ****** Sensor DHT22 ******

#define DHTPIN            14
#define DHTTYPE           DHT22
