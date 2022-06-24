#!/bin/sh
. /lib/functions.sh

board=$(board_name)

case "$board" in
dlink,dsp-w215-b1)
	ssid=$(cat /sys/bus/nvmem/devices/mtd$(find_mtd_index 'mp')/cells/label-ssid@4e,0)
	pin=$(cat /sys/bus/nvmem/devices/mtd$(find_mtd_index 'mp')/cells/label-pin@3e,0)
	uci set wireless.@wifi-device[0].disabled=0
	uci set wireless.@wifi-iface[0].ssid="$ssid"
	uci set wireless.@wifi-iface[0].key="$ssid$pin"
	uci set wireless.@wifi-iface[0].encryption="psk2+aes"
	uci commit wireless
	;;
esac


