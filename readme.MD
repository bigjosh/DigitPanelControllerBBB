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
 
1. Disable `wicd`...
  ```
  update-rc.d wicd disable
  ```
  
  If you don't disable it, it will muck with the wifi adapter when you least expect it and cause unpredicable problems. This took me forever to figure out. Argh. 
  
2. Disable HDMI. (NB: Skip for now, might now be nessisary)

 AdaFruit says that leaving HDMI on interfere with Wifi becuase it is so close to the adapter. 
 ```
 mkdir /mnt/boot
 mount /dev/mmcblk0p1/mnt/boot
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
  ssid=Digit57
  interface=wlan0
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

    
1. Update `/etc/network/interfaces` with (change the `wlan0` to whatever your was in the previous step)...

  ```
  allow-hotplug wlan0
  iface wlan0 inet static
  address 192.168.8.1
  netmask 255.255.255.0
  ```
 
 Note that you really need the hotplug line or it doesn't work well. You also have to load `hostapd` here becuase it will crash if loaded before the interface is up.  

 Note that the `eth0` lines can not be commented out or else NetworkManager will take over network configuration and kill the above wlan0 static ip. Arg. 

1. Enable `hostapd` by editing `/etc/default/hostapd` and updating the line...
  ```
 DAEMON_CONF="/etc/hostapd/hostapd.conf"
 ``` 

1. Disable `udhcpd`...
  ```
  update-rc.d udhcpd disable
  ```
  
  Note that is would be nice to be able to use `udhcpd`, but there does not seem to be a way to have it give out different addresses on different interfaces, and no way to run multiple copies bound to different interfaces.
  
1.  Install dnsmasq...
  ```
  apt-get install dnsmasq
  ```  
  
1. Set up dnsmasq...

  ```
  #Have all DNS requests return this local machine
  # note that we need to hard code this because despite documentation, 
  # 0.0.0.0 as a special address that means "this machine" does not seem to work
  address=/#/192.168.8.1

  #enable dhcp server
  #only send a default gateway and dns server to captive interfaces
  dhcp-range=192.168.8.50,192.168.8.250
  dhcp-range=set:nocapture, 192.168.7.1,192.168.7.1

  dhcp-option=tag:nocapture, option:router
  dhcp-option=tag:nocapture, option:dns-server


  #only listen for DNS and DHCP requests on the wifi and USB networks
  interface=wlan0
  interface=usb0
  ```
  
1. Enable the dnsmasq service

  ```
  systemctl enable dnsmasq.service
  ```    

1. Install the iptables-persistent service....
  ```
  apt-get install iptables-persistent
  ```
  ...and answer `Y` when it ask if you want to save the rules.
  This will save the iptables rules we made so that they load automatically on bootup. 
  
1. Add a redirect to steer all internet http traffic from the Wifi to our special server...
  ```
  iptables -t nat -A PREROUTING -i wlan0 -p tcp --dport 80 -j REDIRECT --to-port 81
  ```
  
1. Set up Apache2 to listen on port 81 in `/etc/apache2/ports.conf`...
  ```
  NameVirtualHost *:80
  Listen 80
  ```
  
1. Add apache config file to `/etc/apache2/sites-available/digit57.conf`...
  ```
  <VirtualHost *:80>
        RedirectMatch temp / http://digit57.com/test.html
  </VirtualHost>

  <VirtualHost *:80>

        ServerAdmin webmaster@localhost

        ServerName digit57.com
        DocumentRoot /root/cewt/digit57.com/www

        <Directory /root/cewt/digit57.com/www>
                Options Indexes FollowSymLinks MultiViews
                AllowOverride None
                Order allow,deny
                allow from all
        </Directory>

        ScriptAlias /cgi-bin/ /usr/lib/cgi-bin/
        <Directory "/usr/lib/cgi-bin">
                AllowOverride None
                Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch
                Order allow,deny
                Allow from all
        </Directory>

        ErrorLog ${APACHE_LOG_DIR}/error.log

        # Possible values include: debug, info, notice, warn, error, crit,
        # alert, emerg.
        LogLevel warn

        CustomLog ${APACHE_LOG_DIR}/access.log combined
  </VirtualHost>
  ```
1. Kill bonescript so we can use port 80...
   ```
   systemctl disable bonescript.socket
   ```
   
   
   
