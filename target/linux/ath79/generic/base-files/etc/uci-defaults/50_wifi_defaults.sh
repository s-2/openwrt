#!/bin/sh
. /lib/functions.sh

board=$(board_name)

case "$board" in
dlink,dap-1120-a1|\
dlink,dap-1320-ax|\
dlink,dap-1320-bx|\
dlink,dap-1320-cx)
	uci set wireless.@wifi-device[0].disabled=0
	uci set wireless.@wifi-iface[0].ssid="$(dd if=$(find_mtd_part 'art') bs=1 skip=$((0xFFBC)) count=10 2>/dev/null)"
	uci set wireless.@wifi-iface[0].key="$(dd if=$(find_mtd_part 'art') bs=1 skip=$((0xFFDD)) count=10 2>/dev/null)"
	uci set wireless.@wifi-iface[0].encryption="psk2+aes"
	uci commit wireless
	;;
esac

exit 0
