#
# copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=getdht12
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/getdht12
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:= Get humidity and temperature in DHT12
  DEPENDS:=+kmod-i2c-slave-dht12
endef

define Package/getdht12/description
 This package is a test program of the dht12 driver
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

#define Build/Configure
#TARGET_LDFLAGS :=
#endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -Wall \
	-o $(PKG_BUILD_DIR)/getdht12 $(PKG_BUILD_DIR)/getdht12.c
endef

define Package/getdht12/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/getdht12 $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,getdht12))
