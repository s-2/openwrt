#!/bin/sh
. /lib/functions.sh

board=$(board_name)

case "$board" in
dlink,dap-1330-a1|\
dlink,dap-1365-a1)
	uci set wireless.@wifi-device[0].disabled=0
	uci set wireless.@wifi-iface[0].ssid="$(dd if=$(find_mtd_part 'mp') bs=1 skip=$((0x4E)) count=10 2>/dev/null)"
	uci set wireless.@wifi-iface[0].key="$(dd if=$(find_mtd_part 'mp') bs=1 skip=$((0x6F)) count=10 2>/dev/null)"
	uci set wireless.@wifi-iface[0].encryption="psk2+aes"
	uci commit wireless
	;;
esac

exit 0
