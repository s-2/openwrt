KERNEL_LOADADDR := 0x40008000

define Device/mediatek_mt7629-rfb
  DEVICE_VENDOR := MediaTek
  DEVICE_MODEL := MT7629 rfb AP
  DEVICE_DTS := mt7629-rfb
  DEVICE_PACKAGES := swconfig
endef
TARGET_DEVICES += mediatek_mt7629-rfb

define Device/dlink_dir-1950-a1
  DEVICE_VENDOR := D-Link
  DEVICE_MODEL := DIR-1950 A1
  DEVICE_DTS := mt7629-dlink-dir-1950-a1
  DEVICE_DTS_DIR := ../dts
  UBINIZE_OPTS := -E 5
  BLOCKSIZE := 128k
  PAGESIZE := 2048
  KERNEL := $$(KERNEL) | pad-offset 128k 2048
  KERNEL_INITRAMFS := $$(KERNEL_INITRAMFS) | pad-to 128k
  IMAGES += recovery.bin
  IMAGE/sysupgrade.bin := sysupgrade-tar | append-metadata
  IMAGE/recovery.bin := append-kernel | pad-to 128k | append-ubi | \
	append-metadata
endef
TARGET_DEVICES += dlink_dir-1950-a1

define Device/iptime_a6004mx
  DEVICE_VENDOR := ipTIME
  DEVICE_MODEL := A6004MX
  DEVICE_DTS := mt7629-iptime-a6004mx
  DEVICE_DTS_DIR := ../dts
  DEVICE_PACKAGES := kmod-usb3 uboot-envtools
  UBINIZE_OPTS := -E 5
  BLOCKSIZE := 128k
  PAGESIZE := 2048
  KERNEL := $$(KERNEL) | pad-offset 128k 2048 | iptime-crc32 a6004mx
  KERNEL_INITRAMFS := $$(KERNEL_INITRAMFS) | pad-offset 128k 2048 | iptime-crc32 a6004mx
  IMAGES += recovery.bin
  IMAGE/sysupgrade.bin := sysupgrade-tar | append-metadata
  IMAGE/recovery.bin := append-kernel | pad-to 128k | append-ubi | append-metadata
endef
TARGET_DEVICES += iptime_a6004mx
