#!/bin/sh

HWMON="/sys/class/hwmon/hwmon11/temp1_input"

if [ -f "$HWMON" ]; then
    RAW=$(cat "$HWMON")
    TEMP_C=$((RAW / 1000))
    TEMP_DEC=$(((RAW % 1000) / 10))
    echo "{\"temperature\": ${TEMP_C}.${TEMP_DEC}}"
else
    echo "{\"error\": \"hwmon sensor not found\"}"
fi
