MODULES = kalyna
EXTENSION = kalyna     # the extersion's name
DATA = kalyna--0.0.1.sql    # script file to install
REGRESS = kalyna_test      # the test script file
CUSTOM_CC = gcc
#PG_CFLAGS = -I...

# for posgres build
export CPATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
