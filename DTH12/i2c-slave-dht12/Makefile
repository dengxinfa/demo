#
#
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=i2c-slave-dht12
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/i2c-slave-dht12  
  SUBMENU:=I2C support
  TITLE:=DHT12 i2c support
  DEPENDS:=+kmod-i2c-core
  FILES:=$(PKG_BUILD_DIR)/i2c-slave-dht12.ko
  AUTOLOAD:=$(call AutoLoad,30,i2c-slave-dht12,1)
  KCONFIG:=
endef

define KernelPackage/DHT12/description
	This is a DHT12 i2c drivers
endef

EXTRA_KCONFIG:= \
	CONFIG_I2C_SLAVE_DHT12=m

EXTRA_CFLAGS:= \
        $(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
        $(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)"\
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)"\
        $(EXTRA_KCONFIG)

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
	$(MAKE_OPTS) modules
endef



$(eval $(call KernelPackage,i2c-slave-dht12))
