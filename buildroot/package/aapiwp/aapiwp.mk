AAPIWP_VERSION = 0.1.0
AAPIWP_SITE = $(BR2_EXTERNAL_AAPIWP_PATH)/..
AAPIWP_SITE_METHOD = local

AAPIWP_DEPENDENCIES = \
	boost \
	dbus-cxx \
	libusb \
	protobuf \
	zlib

AAPIWP_CONF_OPTS = \
	-DBUILD_TESTING=OFF \
	-DCMAKE_BUILD_TYPE=Release

$(eval $(cmake-package))