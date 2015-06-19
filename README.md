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
2. Disable HDMI. (NB: Skip for not, might now be nessisary)

 AdaFruit says that leaving HDMI on interfere with Wifi becuase it is so close to the adapter. 
 ```
 mkdir /mnt/boot
 mount /dev/mmcblk0p1 /mnt/boot
 nano /mnt/boot/uEnv.txt
 ```
 
 Look for the lines like...
 ```
 ##Disable HDMI
 #cape_disable=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONELT-HDMIN
 ```
 
 ...and remove the `#` from the second line so it looks like this...
 
 ```
 #Disable HDMI
 cape_disable=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONELT-HDMIN
 ```
 
2. Plug in a Wifi adapter that can be an Access Point. I used [one with a RT5370 chipset](https://www.amazon.com/Wifi-With-Antenna-For-Raspberry/dp/B00H95C0A2/ref=as_sl_pc_ss_til?tag=joshcom-20&linkCode=w01&linkId=ONO3SOUD47R4JW5J&creativeASIN=B00H95C0A2).



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
  driver=nl80211
  ssid=Digit57_C
  hw_mode=g
  channel=6
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

    
1. Update `/etc/network/interfaces` with (change the `wlan0` to whatever your was in the previous step)...

  ```
  allow-hotplug wlan0
  iface wlan0 inet static
  address 192.168.8.1
  netmask 255.255.255.0
  post-up /usr/sbin/hostapd -B /etc/hostapd/hostapd.conf
  ```
 
 Note that you really need the hotplug line or it doesn't work well. You also have to load `hostapd` here becuase it will crash if loaded before the interface is up.  
 
 We also start hostapd here becuase if you try to start it the normal way (before the wifi interface is up), it will crash.

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
  
    
    
