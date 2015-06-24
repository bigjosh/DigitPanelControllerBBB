# DigitPanelControllerBBB
Software for a BeagleBone Black-based, Wifi controlable LED display panel.

##Setup
1. Start with a new BBB. 

1. Make sure you have enough power. The USB port does not provide enough power for the the Wifi, so you need an AC adapter.

1. Use the flash updater to install the latest Debian software image from...

 http://beagleboard.org/latest-images

1. Update the image by running...
 ```
 cd /opt/scripts/tools/
 ./update_kernel.sh
 ```
1. Update the package index files by running...
  ```
  apt-get update
  ```

2. Plug in a Wifi adapter that can be an Access Point. I used [one with a RT5370 chipset](https://www.amazon.com/Wifi-With-Antenna-For-Raspberry/dp/B00H95C0A2/ref=as_sl_pc_ss_til?tag=joshcom-20&linkCode=w01&linkId=ONO3SOUD47R4JW5J&creativeASIN=B00H95C0A2).

1. Disable the `wicd` daemon by running...
  ```
  update-rc.d wicd disable
  ```
  Otherwise `wicd` will try to reconfigure the Wifi adapter and clobber all the changes we are about to make.

  TODO: Must find a way to have the port auto-DHCP any time cable is connected.   Once wicd is disabled, the ethernet port will no longer automatically DHCP when the cable is plugged in so the cable must be in at bootup. 
  
1. Find the name of the Wifi interface (it is `wlan0` for me)...
    ```
    iwconfig
    ```

1. Find the name of driver for the Wifi adapter based on the interface name (mine was `wlan0`)...
  ```
  ls -l /sys/class/net/wlan0/device/driver
  ```

1. Install the Access Point software by running...

 ```
 apt-get install hostapd
 ```
 
 ..and create `/etc/hostapd/hostapd.conf` as so...
 
  ```
  interface=wlan0
  ssid=Digit57_C
  hw_mode=g
  channel=6
  #driver=nl80211 
  #macaddr_acl=0
  #auth_algs=1
  #ignore_broadcast_ssid=0
  #wpa=2
  #passphrase must be 8 letters or longer
  #wpa_passphrase=nancy123
  #wpa_key_mgmt=WPA-PSK
  #wpa_pairwise=TKIP
  #rsn_pairwise=CCMP
  ```
NOTE: hostapd is very picky about the config file- there can't be able leading or trailing spaces on the lines. 

TODO: I am able to get away with commenting out driver. Are there cases where it must be specified? 
    
1. Update `/etc/network/interfaces` with (change the `wlan0` to whatever your was in the previous step)...

  ```
  auto eth0
  iface eth0 inet dhcp 
  
  allow-hotplug wlan0
  iface wlan0 inet static
  address 192.168.8.1
  netmask 255.255.255.0
  ```
 
 Note that you really need the hotplug line or it doesn't work reliably. 
  
 Note that the `eth0` lines must be uncommented or the interface will not come up. I think this is because `wicd` previously managed this interface and we needed to disable it above. 

1.  Install dnsmasq...
  ```
  apt-get install dnsmasq
  ```  
  
1. Set up dnsmasq...

  ```
  
  #Have all DNS requests return the local machine
  address=/#/127.0.0.1

  #enable dhcp server 
  dhcp-range=192.168.80.50,192.168.8.150,12h

  #only listen for DNS and DHCP requests on the wifi network
  interface=wlan0
  ```
  
1. Enable the dnsmasq service

  ```
  systemctl enable dnsmasq.service
  ```
  
3.
  
    
    
