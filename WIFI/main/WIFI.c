/*  WiFi softAP Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "lwip/sockets.h"
#include <lwip/netdb.h>

/* The examples use WiFi configuration that you can set via project configuration menu.

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID      "i dare you to join"
#define EXAMPLE_ESP_WIFI_PASS      "monsters12"
#define EXAMPLE_ESP_WIFI_CHANNEL   1
#define EXAMPLE_MAX_STA_CONN       WIFI_AUTH_WPA_WPA2_PSK

// UDP definitions
#define CONFIG_EXAMPLE_IPV4 192.168.4.1
#define CONFIG_EXAMPLE_IPV6 /* insert IPv6 */
#define PORT 12345

static const char *TAG = "wifi softAP";
static const char *payload = "AP message successfully received";

// UDP Server Task
static void udp_server_task(void *pvParameters)
{
  char rx_buffer[128];
  char addr_str[128];
  int addr_family = (int)pvParameters;
  int ip_protocol = 0;
  struct sockaddr_in6 dest_addr;

  while (1) {
    if (addr_family == AF_INET) {
      struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
      dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
      dest_addr_ip4->sin_family = AF_INET;
      dest_addr_ip4->sin_port = htons(PORT);
      ip_protocol = IPPROTO_IP;
    } else if (addr_family == AF_INET6) {
      bzero(&dest_addr.sin6_addr.un, sizeof(dest_addr.sin6_addr.un));
      dest_addr.sin6_family = AF_INET6;
      dest_addr.sin6_port = htons(PORT);
      ip_protocol = IPPROTO_IPV6;
    }

    int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
    if (sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        break;
    }
    ESP_LOGI(TAG, "Socket created");

  #if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
    int enable = 1;
    lwip_setsockopt(sock, IPPROTO_IP, IP_PKTINFO, &enable, sizeof(enable));
  #endif

  #if defined(CONFIG_EXAMPLE_IPV4) && defined(CONFIG_EXAMPLE_IPV6)
    if (addr_family == AF_INET6) {
      // Note that by default IPV6 binds to both protocols, it is must be disabled
      // if both protocols used at the same time (used in CI)
      int opt = 1;
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
      setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
      }
  #endif
  
  // Set timeout
  struct timeval timeout;
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;
  setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
    int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
    }
    ESP_LOGI(TAG, "Socket bound, port %d", PORT);

    struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
    socklen_t socklen = sizeof(source_addr);

  #if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
    struct iovec iov;
    struct msghdr msg;
    struct cmsghdr *cmsgtmp;
    u8_t cmsg_buf[CMSG_SPACE(sizeof(struct in_pktinfo))];

    iov.iov_base = rx_buffer;
    iov.iov_len = sizeof(rx_buffer);
    msg.msg_control = cmsg_buf;
    msg.msg_controllen = sizeof(cmsg_buf);
    msg.msg_flags = 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_name = (struct sockaddr *)&source_addr;
    msg.msg_namelen = socklen;
  #endif

    while (1) {
      ESP_LOGI(TAG, "Waiting for data");
      #if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
        int len = recvmsg(sock, &msg, 0);
      #else
        int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
      #endif
      // Error occurred during receiving
      if (len < 0) {
        ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
        break;
      }
      // Data received
      else {
        // Get the sender's ip address as string
        if (source_addr.ss_family == PF_INET) {
          inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
          #if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
            for ( cmsgtmp = CMSG_FIRSTHDR(&msg); cmsgtmp != NULL; cmsgtmp = CMSG_NXTHDR(&msg, cmsgtmp) ) {
              if ( cmsgtmp->cmsg_level == IPPROTO_IP && cmsgtmp->cmsg_type == IP_PKTINFO ) {
                struct in_pktinfo *pktinfo;
                pktinfo = (struct in_pktinfo*)CMSG_DATA(cmsgtmp);
                ESP_LOGI(TAG, "dest ip: %s", inet_ntoa(pktinfo->ipi_addr));
              }
            }
      #endif
        } else if (source_addr.ss_family == PF_INET6) {
          inet6_ntoa_r(((struct sockaddr_in6 *)&source_addr)->sin6_addr, addr_str, sizeof(addr_str) - 1);
        }

        rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string...
        ESP_LOGI(TAG, "Received %d bytes from client %s:", len, addr_str);
        ESP_LOGI(TAG, "%s", rx_buffer);

        int err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
        if (err < 0) {
          ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
          break;
        }
      }
    }

    if (sock != -1) {
      ESP_LOGE(TAG, "Shutting down socket and restarting...");
      shutdown(sock, 0);
      close(sock);
    }
  }
  vTaskDelete(NULL);
}


static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                  int32_t event_id, void* event_data){
  if (event_id == WIFI_EVENT_AP_STACONNECTED) {
      wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
      ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
               MAC2STR(event->mac), event->aid);
  } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
      wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
      ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d, reason=%d",
               MAC2STR(event->mac), event->aid, event->reason);
  }
}

void wifi_init_softap(void)
{
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_ap();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                      ESP_EVENT_ANY_ID,
                                                      &wifi_event_handler,
                                                      NULL,
                                                      NULL));

  wifi_config_t wifi_config = {
      .ap = {
          .ssid = EXAMPLE_ESP_WIFI_SSID,
          .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
          .channel = EXAMPLE_ESP_WIFI_CHANNEL,
          .password = EXAMPLE_ESP_WIFI_PASS,
          .max_connection = EXAMPLE_MAX_STA_CONN,
  #ifdef CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT
          .authmode = WIFI_AUTH_WPA3_PSK,
          .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
  #else /* CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT */
          .authmode = WIFI_AUTH_WPA2_PSK,
  #endif
          .pmf_cfg = {
                  .required = true,
          },
      },
  };
  if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
      wifi_config.ap.authmode = WIFI_AUTH_OPEN;
  }

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
           EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}

void app_main(void)
{
  //Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
  wifi_init_softap();

  
  // Initialize UDP Server
  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  //ESP_ERROR_CHECK(esp_event_loop_create_default());

  /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
   * Read "Establishing Wi-Fi or Ethernet Connection" section in
   * examples/protocols/README.md for more information about this function.
   */
  //ESP_ERROR_CHECK(example_connect());

  #ifdef CONFIG_EXAMPLE_IPV4
    xTaskCreate(udp_server_task, "udp_server", 4096, (void*)AF_INET, 5, NULL);
  #endif
  #ifdef CONFIG_EXAMPLE_IPV6
    xTaskCreate(udp_server_task, "udp_server", 4096, (void*)AF_INET6, 5, NULL);
  #endif
}






