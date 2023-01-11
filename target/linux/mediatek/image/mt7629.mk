ifneq ($(KERNEL),6.1)
DTS_DIR := $(DTS_DIR)/mediatek
endif

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

define Device/netgear_ex6250-v2
  DEVICE_VENDOR := NETGEAR
  DEVICE_MODEL := EX6250
  DEVICE_VARIANT := v2
  DEVICE_ALT0_VENDOR := NETGEAR
  DEVICE_ALT0_MODEL := EX6400
  DEVICE_ALT0_VARIANT := v3
  DEVICE_ALT1_VENDOR := NETGEAR
  DEVICE_ALT1_MODEL := EX6410
  DEVICE_ALT1_VARIANT := v2
  DEVICE_ALT2_VENDOR := NETGEAR
  DEVICE_ALT2_MODEL := EX6470
  DEVICE_DTS := mt7629-netgear-ex6250-v2
  DEVICE_DTS_DIR := ../dts
  DEVICE_PACKAGES := uboot-envtools
  NETGEAR_ENC_MODEL := EX6250v2
  NETGEAR_ENC_REGION := US
  NETGEAR_ENC_HW_ID_LIST := 1010000003630000_NETGEAR;1010000003540000_NETGEAR
  NETGEAR_ENC_MODEL_LIST := EX6250v2;EX6400v3;EX6470;EX6410v2
  IMAGE_SIZE := 13120k
  IMAGES += factory.img
  IMAGE/factory.img := append-kernel | pad-to 128k | append-rootfs | \
	pad-rootfs | check-size | netgear-encrypted-factory
endef
TARGET_DEVICES += netgear_ex6250-v2

define Device/tplink_eap225-v5
  DEVICE_VENDOR := TP-Link
  DEVICE_MODEL := EAP225
  DEVICE_VARIANT := v5
  DEVICE_DTS := mt7629-tplink_eap225-v5
  DEVICE_DTS_DIR := ../dts
endef
TARGET_DEVICES += tplink_eap225-v5
