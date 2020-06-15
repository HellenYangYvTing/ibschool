#
#   appweb-vxworks-default.mk -- Makefile to build Embedthis Appweb for vxworks
#

PRODUCT            := appweb
VERSION            := 4.3.4
BUILD_NUMBER       := 0
PROFILE            := default
ARCH               := $(shell echo $(WIND_HOST_TYPE) | sed 's/-.*//')
CPU                := $(subst X86,PENTIUM,$(shell echo $(ARCH) | tr a-z A-Z))
OS                 := vxworks
CC                 := cc$(subst x86,pentium,$(ARCH))
LD                 := link
CONFIG             := $(OS)-$(ARCH)-$(PROFILE)
LBIN               := $(CONFIG)/bin

BIT_PACK_CGI       := 1
BIT_PACK_EJSCRIPT  := 0
BIT_PACK_ESP       := 1
BIT_PACK_EST       := 0
BIT_PACK_MATRIXSSL := 0
BIT_PACK_MDB       := 1
BIT_PACK_NANOSSL   := 0
BIT_PACK_OPENSSL   := 0
BIT_PACK_PCRE      := 1
BIT_PACK_PHP       := 0
BIT_PACK_SDB       := 0
BIT_PACK_SQLITE    := 0
BIT_PACK_SSL       := 0

ifeq ($(BIT_PACK_EST),1)
    BIT_PACK_SSL := 1
endif
ifeq ($(BIT_PACK_LIB),1)
    BIT_PACK_COMPILER := 1
endif
ifeq ($(BIT_PACK_MATRIXSSL),1)
    BIT_PACK_SSL := 1
endif
ifeq ($(BIT_PACK_MDB),1)
    BIT_PACK_ESP := 1
endif
ifeq ($(BIT_PACK_NANOSSL),1)
    BIT_PACK_SSL := 1
endif
ifeq ($(BIT_PACK_OPENSSL),1)
    BIT_PACK_SSL := 1
endif
ifeq ($(BIT_PACK_SDB),1)
    BIT_PACK_ESP := 1
    BIT_PACK_SQLITE := 1
endif

BIT_PACK_CGI_PATH         := cgi
BIT_PACK_COMPILER_PATH    := cc$(subst x86,pentium,$(ARCH))
BIT_PACK_DIR_PATH         := dir
BIT_PACK_DOXYGEN_PATH     := doxygen
BIT_PACK_DSI_PATH         := dsi
BIT_PACK_EJSCRIPT_PATH    := ejscript
BIT_PACK_ESP_PATH         := esp
BIT_PACK_EST_PATH         := est
BIT_PACK_LIB_PATH         := ar
BIT_PACK_LINK_PATH        := link
BIT_PACK_MAN_PATH         := man
BIT_PACK_MAN2HTML_PATH    := man2html
BIT_PACK_MATRIXSSL_PATH   := /usr/src/matrixssl
BIT_PACK_MDB_PATH         := mdb
BIT_PACK_NANOSSL_PATH     := /usr/src/nanossl
BIT_PACK_OPENSSL_PATH     := /usr/src/openssl
BIT_PACK_PCRE_PATH        := pcre
BIT_PACK_PHP_PATH         := /usr/src/php
BIT_PACK_PMAKER_PATH      := pmaker
BIT_PACK_SDB_PATH         := sdb
BIT_PACK_SQLITE_PATH      := sqlite
BIT_PACK_SSL_PATH         := ssl
BIT_PACK_UTEST_PATH       := utest
BIT_PACK_VXWORKS_PATH     := $(WIND_BASE)
BIT_PACK_ZIP_PATH         := zip

export WIND_HOME          := $(WIND_BASE)/..
export PATH               := $(WIND_GNU_PATH)/$(WIND_HOST_TYPE)/bin:$(PATH)

CFLAGS             += -fno-builtin -fno-defer-pop -fvolatile -w
DFLAGS             += -DVXWORKS -DRW_MULTI_THREAD -D_GNU_TOOL -DCPU=PENTIUM $(patsubst %,-D%,$(filter BIT_%,$(MAKEFLAGS))) -DBIT_PACK_CGI=$(BIT_PACK_CGI) -DBIT_PACK_EJSCRIPT=$(BIT_PACK_EJSCRIPT) -DBIT_PACK_ESP=$(BIT_PACK_ESP) -DBIT_PACK_EST=$(BIT_PACK_EST) -DBIT_PACK_MATRIXSSL=$(BIT_PACK_MATRIXSSL) -DBIT_PACK_MDB=$(BIT_PACK_MDB) -DBIT_PACK_NANOSSL=$(BIT_PACK_NANOSSL) -DBIT_PACK_OPENSSL=$(BIT_PACK_OPENSSL) -DBIT_PACK_PCRE=$(BIT_PACK_PCRE) -DBIT_PACK_PHP=$(BIT_PACK_PHP) -DBIT_PACK_SDB=$(BIT_PACK_SDB) -DBIT_PACK_SQLITE=$(BIT_PACK_SQLITE) -DBIT_PACK_SSL=$(BIT_PACK_SSL) 
IFLAGS             += -I$(CONFIG)/inc -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip
LDFLAGS            += '-Wl,-r'
LIBPATHS           += -L$(CONFIG)/bin
LIBS               += -lgcc

DEBUG              := debug
CFLAGS-debug       := -g
DFLAGS-debug       := -DBIT_DEBUG
LDFLAGS-debug      := -g
DFLAGS-release     := 
CFLAGS-release     := -O2
LDFLAGS-release    := 
CFLAGS             += $(CFLAGS-$(DEBUG))
DFLAGS             += $(DFLAGS-$(DEBUG))
LDFLAGS            += $(LDFLAGS-$(DEBUG))

BIT_ROOT_PREFIX    := deploy
BIT_BASE_PREFIX    := $(BIT_ROOT_PREFIX)
BIT_DATA_PREFIX    := $(BIT_VAPP_PREFIX)
BIT_STATE_PREFIX   := $(BIT_VAPP_PREFIX)
BIT_BIN_PREFIX     := $(BIT_VAPP_PREFIX)
BIT_INC_PREFIX     := $(BIT_VAPP_PREFIX)/inc
BIT_LIB_PREFIX     := $(BIT_VAPP_PREFIX)
BIT_MAN_PREFIX     := $(BIT_VAPP_PREFIX)
BIT_SBIN_PREFIX    := $(BIT_VAPP_PREFIX)
BIT_ETC_PREFIX     := $(BIT_VAPP_PREFIX)
BIT_WEB_PREFIX     := $(BIT_VAPP_PREFIX)/web
BIT_LOG_PREFIX     := $(BIT_VAPP_PREFIX)
BIT_SPOOL_PREFIX   := $(BIT_VAPP_PREFIX)
BIT_CACHE_PREFIX   := $(BIT_VAPP_PREFIX)
BIT_APP_PREFIX     := $(BIT_BASE_PREFIX)
BIT_VAPP_PREFIX    := $(BIT_APP_PREFIX)
BIT_SRC_PREFIX     := $(BIT_ROOT_PREFIX)/usr/src/$(PRODUCT)-$(VERSION)


TARGETS            += $(CONFIG)/bin/libmpr.out
TARGETS            += $(CONFIG)/bin/libmprssl.out
TARGETS            += $(CONFIG)/bin/appman.out
TARGETS            += $(CONFIG)/bin/makerom.out
TARGETS            += $(CONFIG)/bin/ca.crt
ifeq ($(BIT_PACK_PCRE),1)
TARGETS            += $(CONFIG)/bin/libpcre.out
endif
TARGETS            += $(CONFIG)/bin/libhttp.out
TARGETS            += $(CONFIG)/bin/http.out
ifeq ($(BIT_PACK_SQLITE),1)
TARGETS            += $(CONFIG)/bin/libsqlite3.out
endif
ifeq ($(BIT_PACK_SQLITE),1)
TARGETS            += $(CONFIG)/bin/sqlite.out
endif
TARGETS            += $(CONFIG)/bin/libappweb.out
ifeq ($(BIT_PACK_ESP),1)
TARGETS            += $(CONFIG)/bin/libmod_esp.out
endif
ifeq ($(BIT_PACK_ESP),1)
TARGETS            += $(CONFIG)/bin/esp.out
endif
ifeq ($(BIT_PACK_ESP),1)
TARGETS            += $(CONFIG)/bin/esp.conf
endif
ifeq ($(BIT_PACK_ESP),1)
TARGETS            += src/server/esp.conf
endif
ifeq ($(BIT_PACK_ESP),1)
TARGETS            += $(CONFIG)/bin/esp-www
endif
ifeq ($(BIT_PACK_EJSCRIPT),1)
TARGETS            += $(CONFIG)/bin/libejs.out
endif
ifeq ($(BIT_PACK_EJSCRIPT),1)
TARGETS            += $(CONFIG)/bin/ejs.out
endif
ifeq ($(BIT_PACK_EJSCRIPT),1)
TARGETS            += $(CONFIG)/bin/ejsc.out
endif
ifeq ($(BIT_PACK_EJSCRIPT),1)
TARGETS            += $(CONFIG)/bin/ejs.mod
endif
ifeq ($(BIT_PACK_CGI),1)
TARGETS            += $(CONFIG)/bin/libmod_cgi.out
endif
ifeq ($(BIT_PACK_EJSCRIPT),1)
TARGETS            += $(CONFIG)/bin/libmod_ejs.out
endif
ifeq ($(BIT_PACK_PHP),1)
TARGETS            += $(CONFIG)/bin/libmod_php.out
endif
ifeq ($(BIT_PACK_SSL),1)
TARGETS            += $(CONFIG)/bin/libmod_ssl.out
endif
TARGETS            += $(CONFIG)/bin/authpass.out
ifeq ($(BIT_PACK_CGI),1)
TARGETS            += $(CONFIG)/bin/cgiProgram.out
endif
TARGETS            += src/server/slink.c
TARGETS            += $(CONFIG)/bin/libslink.out
TARGETS            += $(CONFIG)/bin/appweb.out
TARGETS            += src/server/cache
TARGETS            += $(CONFIG)/bin/testAppweb.out
ifeq ($(BIT_PACK_CGI),1)
TARGETS            += test/cgi-bin/testScript
endif
ifeq ($(BIT_PACK_CGI),1)
TARGETS            += test/web/caching/cache.cgi
endif
ifeq ($(BIT_PACK_CGI),1)
TARGETS            += test/web/auth/basic/basic.cgi
endif
ifeq ($(BIT_PACK_CGI),1)
TARGETS            += test/cgi-bin/cgiProgram.out
endif
TARGETS            += test/web/js

unexport CDPATH

ifndef SHOW
.SILENT:
endif

all build compile: prep $(TARGETS)

.PHONY: prep

prep:
	@echo "      [Info] Use "make SHOW=1" to trace executed commands."
	@if [ "$(CONFIG)" = "" ] ; then echo WARNING: CONFIG not set ; exit 255 ; fi
	@if [ "$(BIT_APP_PREFIX)" = "" ] ; then echo WARNING: BIT_APP_PREFIX not set ; exit 255 ; fi
	@if [ "$(WIND_BASE)" = "" ] ; then echo WARNING: WIND_BASE not set. Run wrenv.sh. ; exit 255 ; fi
	@if [ "$(WIND_HOST_TYPE)" = "" ] ; then echo WARNING: WIND_HOST_TYPE not set. Run wrenv.sh. ; exit 255 ; fi
	@if [ "$(WIND_GNU_PATH)" = "" ] ; then echo WARNING: WIND_GNU_PATH not set. Run wrenv.sh. ; exit 255 ; fi
	@[ ! -x $(CONFIG)/bin ] && mkdir -p $(CONFIG)/bin; true
	@[ ! -x $(CONFIG)/inc ] && mkdir -p $(CONFIG)/inc; true
	@[ ! -x $(CONFIG)/obj ] && mkdir -p $(CONFIG)/obj; true
	@[ ! -f $(CONFIG)/inc/bitos.h ] && cp src/bitos.h $(CONFIG)/inc/bitos.h ; true
	@if ! diff $(CONFIG)/inc/bitos.h src/bitos.h >/dev/null ; then\
		cp src/bitos.h $(CONFIG)/inc/bitos.h  ; \
	fi; true
	@[ ! -f $(CONFIG)/inc/bit.h ] && cp projects/appweb-vxworks-default-bit.h $(CONFIG)/inc/bit.h ; true
	@if ! diff $(CONFIG)/inc/bit.h projects/appweb-vxworks-default-bit.h >/dev/null ; then\
		cp projects/appweb-vxworks-default-bit.h $(CONFIG)/inc/bit.h  ; \
	fi; true
	@if [ -f "$(CONFIG)/.makeflags" ] ; then \
		if [ "$(MAKEFLAGS)" != " ` cat $(CONFIG)/.makeflags`" ] ; then \
			echo "   [Warning] Make flags have changed since the last build: "`cat $(CONFIG)/.makeflags`"" ; \
		fi ; \
	fi
	@echo $(MAKEFLAGS) >$(CONFIG)/.makeflags

clean:
	rm -f "$(CONFIG)/bin/libmpr.out"
	rm -f "$(CONFIG)/bin/libmprssl.out"
	rm -f "$(CONFIG)/bin/appman.out"
	rm -f "$(CONFIG)/bin/makerom.out"
	rm -f "$(CONFIG)/bin/libest.out"
	rm -f "$(CONFIG)/bin/ca.crt"
	rm -f "$(CONFIG)/bin/libpcre.out"
	rm -f "$(CONFIG)/bin/libhttp.out"
	rm -f "$(CONFIG)/bin/http.out"
	rm -f "$(CONFIG)/bin/libsqlite3.out"
	rm -f "$(CONFIG)/bin/sqlite.out"
	rm -f "$(CONFIG)/bin/libappweb.out"
	rm -f "$(CONFIG)/bin/libmod_esp.out"
	rm -f "$(CONFIG)/bin/esp.out"
	rm -f "$(CONFIG)/bin/esp.conf"
	rm -f "src/server/esp.conf"
	rm -fr "$(CONFIG)/bin/esp-www"
	rm -f "$(CONFIG)/bin/libejs.out"
	rm -f "$(CONFIG)/bin/ejs.out"
	rm -f "$(CONFIG)/bin/ejsc.out"
	rm -f "$(CONFIG)/bin/libmod_cgi.out"
	rm -f "$(CONFIG)/bin/libmod_ejs.out"
	rm -f "$(CONFIG)/bin/libmod_php.out"
	rm -f "$(CONFIG)/bin/libmod_ssl.out"
	rm -f "$(CONFIG)/bin/authpass.out"
	rm -f "$(CONFIG)/bin/cgiProgram.out"
	rm -f "$(CONFIG)/bin/libslink.out"
	rm -f "$(CONFIG)/bin/appweb.out"
	rm -f "$(CONFIG)/bin/testAppweb.out"
	rm -fr "test/web/js"
	rm -f "$(CONFIG)/obj/mprLib.o"
	rm -f "$(CONFIG)/obj/mprSsl.o"
	rm -f "$(CONFIG)/obj/manager.o"
	rm -f "$(CONFIG)/obj/makerom.o"
	rm -f "$(CONFIG)/obj/estLib.o"
	rm -f "$(CONFIG)/obj/pcre.o"
	rm -f "$(CONFIG)/obj/httpLib.o"
	rm -f "$(CONFIG)/obj/http.o"
	rm -f "$(CONFIG)/obj/sqlite3.o"
	rm -f "$(CONFIG)/obj/sqlite.o"
	rm -f "$(CONFIG)/obj/config.o"
	rm -f "$(CONFIG)/obj/convenience.o"
	rm -f "$(CONFIG)/obj/dirHandler.o"
	rm -f "$(CONFIG)/obj/fileHandler.o"
	rm -f "$(CONFIG)/obj/log.o"
	rm -f "$(CONFIG)/obj/server.o"
	rm -f "$(CONFIG)/obj/edi.o"
	rm -f "$(CONFIG)/obj/espAbbrev.o"
	rm -f "$(CONFIG)/obj/espFramework.o"
	rm -f "$(CONFIG)/obj/espHandler.o"
	rm -f "$(CONFIG)/obj/espHtml.o"
	rm -f "$(CONFIG)/obj/espTemplate.o"
	rm -f "$(CONFIG)/obj/mdb.o"
	rm -f "$(CONFIG)/obj/sdb.o"
	rm -f "$(CONFIG)/obj/esp.o"
	rm -f "$(CONFIG)/obj/ejsLib.o"
	rm -f "$(CONFIG)/obj/ejs.o"
	rm -f "$(CONFIG)/obj/ejsc.o"
	rm -f "$(CONFIG)/obj/cgiHandler.o"
	rm -f "$(CONFIG)/obj/ejsHandler.o"
	rm -f "$(CONFIG)/obj/phpHandler.o"
	rm -f "$(CONFIG)/obj/sslModule.o"
	rm -f "$(CONFIG)/obj/authpass.o"
	rm -f "$(CONFIG)/obj/cgiProgram.o"
	rm -f "$(CONFIG)/obj/slink.o"
	rm -f "$(CONFIG)/obj/appweb.o"
	rm -f "$(CONFIG)/obj/testAppweb.o"
	rm -f "$(CONFIG)/obj/testHttp.o"

clobber: clean
	rm -fr ./$(CONFIG)



#
#   version
#
version: $(DEPS_1)
	@echo 4.3.4-0

#
#   mpr.h
#
$(CONFIG)/inc/mpr.h: $(DEPS_2)
	@echo '      [Copy] $(CONFIG)/inc/mpr.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/deps/mpr/mpr.h $(CONFIG)/inc/mpr.h

#
#   bit.h
#
$(CONFIG)/inc/bit.h: $(DEPS_3)
	@echo '      [Copy] $(CONFIG)/inc/bit.h'

#
#   bitos.h
#
DEPS_4 += $(CONFIG)/inc/bit.h

$(CONFIG)/inc/bitos.h: $(DEPS_4)
	@echo '      [Copy] $(CONFIG)/inc/bitos.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/bitos.h $(CONFIG)/inc/bitos.h

#
#   mprLib.o
#
DEPS_5 += $(CONFIG)/inc/bit.h
DEPS_5 += $(CONFIG)/inc/mpr.h
DEPS_5 += $(CONFIG)/inc/bitos.h

$(CONFIG)/obj/mprLib.o: \
    src/deps/mpr/mprLib.c $(DEPS_5)
	@echo '   [Compile] $(CONFIG)/obj/mprLib.o'
	$(CC) -c -o $(CONFIG)/obj/mprLib.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/mpr/mprLib.c

#
#   libmpr
#
DEPS_6 += $(CONFIG)/inc/mpr.h
DEPS_6 += $(CONFIG)/inc/bit.h
DEPS_6 += $(CONFIG)/inc/bitos.h
DEPS_6 += $(CONFIG)/obj/mprLib.o

$(CONFIG)/bin/libmpr.out: $(DEPS_6)
	@echo '      [Link] $(CONFIG)/bin/libmpr.out'
	$(CC) -r -o $(CONFIG)/bin/libmpr.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/mprLib.o" $(LIBS) 

#
#   est.h
#
$(CONFIG)/inc/est.h: $(DEPS_7)
	@echo '      [Copy] $(CONFIG)/inc/est.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/deps/est/est.h $(CONFIG)/inc/est.h

#
#   estLib.o
#
DEPS_8 += $(CONFIG)/inc/bit.h
DEPS_8 += $(CONFIG)/inc/est.h
DEPS_8 += $(CONFIG)/inc/bitos.h

$(CONFIG)/obj/estLib.o: \
    src/deps/est/estLib.c $(DEPS_8)
	@echo '   [Compile] $(CONFIG)/obj/estLib.o'
	$(CC) -c -o $(CONFIG)/obj/estLib.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/est/estLib.c

ifeq ($(BIT_PACK_EST),1)
#
#   libest
#
DEPS_9 += $(CONFIG)/inc/est.h
DEPS_9 += $(CONFIG)/inc/bit.h
DEPS_9 += $(CONFIG)/inc/bitos.h
DEPS_9 += $(CONFIG)/obj/estLib.o

$(CONFIG)/bin/libest.out: $(DEPS_9)
	@echo '      [Link] $(CONFIG)/bin/libest.out'
	$(CC) -r -o $(CONFIG)/bin/libest.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/estLib.o" $(LIBS) 
endif

#
#   mprSsl.o
#
DEPS_10 += $(CONFIG)/inc/bit.h
DEPS_10 += $(CONFIG)/inc/mpr.h
DEPS_10 += $(CONFIG)/inc/est.h

$(CONFIG)/obj/mprSsl.o: \
    src/deps/mpr/mprSsl.c $(DEPS_10)
	@echo '   [Compile] $(CONFIG)/obj/mprSsl.o'
	$(CC) -c -o $(CONFIG)/obj/mprSsl.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" "-I$(BIT_PACK_MATRIXSSL_PATH)" "-I$(BIT_PACK_MATRIXSSL_PATH)/matrixssl" "-I$(BIT_PACK_NANOSSL_PATH)/src" "-I$(BIT_PACK_OPENSSL_PATH)/include" src/deps/mpr/mprSsl.c

#
#   libmprssl
#
DEPS_11 += $(CONFIG)/inc/mpr.h
DEPS_11 += $(CONFIG)/inc/bit.h
DEPS_11 += $(CONFIG)/inc/bitos.h
DEPS_11 += $(CONFIG)/obj/mprLib.o
DEPS_11 += $(CONFIG)/bin/libmpr.out
DEPS_11 += $(CONFIG)/inc/est.h
DEPS_11 += $(CONFIG)/obj/estLib.o
ifeq ($(BIT_PACK_EST),1)
    DEPS_11 += $(CONFIG)/bin/libest.out
endif
DEPS_11 += $(CONFIG)/obj/mprSsl.o

ifeq ($(BIT_PACK_MATRIXSSL),1)
    LIBS_11 += -lmatrixssl
    LIBPATHS_11 += -L$(BIT_PACK_MATRIXSSL_PATH)
endif
ifeq ($(BIT_PACK_NANOSSL),1)
    LIBS_11 += -lssls
    LIBPATHS_11 += -L$(BIT_PACK_NANOSSL_PATH)/bin
endif
ifeq ($(BIT_PACK_OPENSSL),1)
    LIBS_11 += -lssl
    LIBPATHS_11 += -L$(BIT_PACK_OPENSSL_PATH)
endif
ifeq ($(BIT_PACK_OPENSSL),1)
    LIBS_11 += -lcrypto
    LIBPATHS_11 += -L$(BIT_PACK_OPENSSL_PATH)
endif

$(CONFIG)/bin/libmprssl.out: $(DEPS_11)
	@echo '      [Link] $(CONFIG)/bin/libmprssl.out'
	$(CC) -r -o $(CONFIG)/bin/libmprssl.out $(LDFLAGS) $(LIBPATHS)    "$(CONFIG)/obj/mprSsl.o" $(LIBPATHS_11) $(LIBS_11) $(LIBS_11) $(LIBS) 

#
#   manager.o
#
DEPS_12 += $(CONFIG)/inc/bit.h
DEPS_12 += $(CONFIG)/inc/mpr.h

$(CONFIG)/obj/manager.o: \
    src/deps/mpr/manager.c $(DEPS_12)
	@echo '   [Compile] $(CONFIG)/obj/manager.o'
	$(CC) -c -o $(CONFIG)/obj/manager.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/mpr/manager.c

#
#   manager
#
DEPS_13 += $(CONFIG)/inc/mpr.h
DEPS_13 += $(CONFIG)/inc/bit.h
DEPS_13 += $(CONFIG)/inc/bitos.h
DEPS_13 += $(CONFIG)/obj/mprLib.o
DEPS_13 += $(CONFIG)/bin/libmpr.out
DEPS_13 += $(CONFIG)/obj/manager.o

$(CONFIG)/bin/appman.out: $(DEPS_13)
	@echo '      [Link] $(CONFIG)/bin/appman.out'
	$(CC) -o $(CONFIG)/bin/appman.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/manager.o" $(LIBS) -Wl,-r 

#
#   makerom.o
#
DEPS_14 += $(CONFIG)/inc/bit.h
DEPS_14 += $(CONFIG)/inc/mpr.h

$(CONFIG)/obj/makerom.o: \
    src/deps/mpr/makerom.c $(DEPS_14)
	@echo '   [Compile] $(CONFIG)/obj/makerom.o'
	$(CC) -c -o $(CONFIG)/obj/makerom.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/mpr/makerom.c

#
#   makerom
#
DEPS_15 += $(CONFIG)/inc/mpr.h
DEPS_15 += $(CONFIG)/inc/bit.h
DEPS_15 += $(CONFIG)/inc/bitos.h
DEPS_15 += $(CONFIG)/obj/mprLib.o
DEPS_15 += $(CONFIG)/bin/libmpr.out
DEPS_15 += $(CONFIG)/obj/makerom.o

$(CONFIG)/bin/makerom.out: $(DEPS_15)
	@echo '      [Link] $(CONFIG)/bin/makerom.out'
	$(CC) -o $(CONFIG)/bin/makerom.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/makerom.o" $(LIBS) -Wl,-r 

#
#   ca-crt
#
DEPS_16 += src/deps/est/ca.crt

$(CONFIG)/bin/ca.crt: $(DEPS_16)
	@echo '      [Copy] $(CONFIG)/bin/ca.crt'
	mkdir -p "$(CONFIG)/bin"
	cp src/deps/est/ca.crt $(CONFIG)/bin/ca.crt

#
#   pcre.h
#
$(CONFIG)/inc/pcre.h: $(DEPS_17)
	@echo '      [Copy] $(CONFIG)/inc/pcre.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/deps/pcre/pcre.h $(CONFIG)/inc/pcre.h

#
#   pcre.o
#
DEPS_18 += $(CONFIG)/inc/bit.h
DEPS_18 += $(CONFIG)/inc/pcre.h

$(CONFIG)/obj/pcre.o: \
    src/deps/pcre/pcre.c $(DEPS_18)
	@echo '   [Compile] $(CONFIG)/obj/pcre.o'
	$(CC) -c -o $(CONFIG)/obj/pcre.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/pcre/pcre.c

ifeq ($(BIT_PACK_PCRE),1)
#
#   libpcre
#
DEPS_19 += $(CONFIG)/inc/pcre.h
DEPS_19 += $(CONFIG)/inc/bit.h
DEPS_19 += $(CONFIG)/obj/pcre.o

$(CONFIG)/bin/libpcre.out: $(DEPS_19)
	@echo '      [Link] $(CONFIG)/bin/libpcre.out'
	$(CC) -r -o $(CONFIG)/bin/libpcre.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/pcre.o" $(LIBS) 
endif

#
#   http.h
#
$(CONFIG)/inc/http.h: $(DEPS_20)
	@echo '      [Copy] $(CONFIG)/inc/http.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/deps/http/http.h $(CONFIG)/inc/http.h

#
#   httpLib.o
#
DEPS_21 += $(CONFIG)/inc/bit.h
DEPS_21 += $(CONFIG)/inc/http.h
DEPS_21 += $(CONFIG)/inc/mpr.h

$(CONFIG)/obj/httpLib.o: \
    src/deps/http/httpLib.c $(DEPS_21)
	@echo '   [Compile] $(CONFIG)/obj/httpLib.o'
	$(CC) -c -o $(CONFIG)/obj/httpLib.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/http/httpLib.c

#
#   libhttp
#
DEPS_22 += $(CONFIG)/inc/mpr.h
DEPS_22 += $(CONFIG)/inc/bit.h
DEPS_22 += $(CONFIG)/inc/bitos.h
DEPS_22 += $(CONFIG)/obj/mprLib.o
DEPS_22 += $(CONFIG)/bin/libmpr.out
DEPS_22 += $(CONFIG)/inc/pcre.h
DEPS_22 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_22 += $(CONFIG)/bin/libpcre.out
endif
DEPS_22 += $(CONFIG)/inc/http.h
DEPS_22 += $(CONFIG)/obj/httpLib.o

$(CONFIG)/bin/libhttp.out: $(DEPS_22)
	@echo '      [Link] $(CONFIG)/bin/libhttp.out'
	$(CC) -r -o $(CONFIG)/bin/libhttp.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/httpLib.o" $(LIBS) 

#
#   http.o
#
DEPS_23 += $(CONFIG)/inc/bit.h
DEPS_23 += $(CONFIG)/inc/http.h

$(CONFIG)/obj/http.o: \
    src/deps/http/http.c $(DEPS_23)
	@echo '   [Compile] $(CONFIG)/obj/http.o'
	$(CC) -c -o $(CONFIG)/obj/http.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/http/http.c

#
#   http
#
DEPS_24 += $(CONFIG)/inc/mpr.h
DEPS_24 += $(CONFIG)/inc/bit.h
DEPS_24 += $(CONFIG)/inc/bitos.h
DEPS_24 += $(CONFIG)/obj/mprLib.o
DEPS_24 += $(CONFIG)/bin/libmpr.out
DEPS_24 += $(CONFIG)/inc/pcre.h
DEPS_24 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_24 += $(CONFIG)/bin/libpcre.out
endif
DEPS_24 += $(CONFIG)/inc/http.h
DEPS_24 += $(CONFIG)/obj/httpLib.o
DEPS_24 += $(CONFIG)/bin/libhttp.out
DEPS_24 += $(CONFIG)/obj/http.o

$(CONFIG)/bin/http.out: $(DEPS_24)
	@echo '      [Link] $(CONFIG)/bin/http.out'
	$(CC) -o $(CONFIG)/bin/http.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/http.o" $(LIBS) -Wl,-r 

#
#   sqlite3.h
#
$(CONFIG)/inc/sqlite3.h: $(DEPS_25)
	@echo '      [Copy] $(CONFIG)/inc/sqlite3.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/deps/sqlite/sqlite3.h $(CONFIG)/inc/sqlite3.h

#
#   sqlite3.o
#
DEPS_26 += $(CONFIG)/inc/bit.h
DEPS_26 += $(CONFIG)/inc/sqlite3.h

$(CONFIG)/obj/sqlite3.o: \
    src/deps/sqlite/sqlite3.c $(DEPS_26)
	@echo '   [Compile] $(CONFIG)/obj/sqlite3.o'
	$(CC) -c -o $(CONFIG)/obj/sqlite3.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/sqlite/sqlite3.c

ifeq ($(BIT_PACK_SQLITE),1)
#
#   libsqlite3
#
DEPS_27 += $(CONFIG)/inc/sqlite3.h
DEPS_27 += $(CONFIG)/inc/bit.h
DEPS_27 += $(CONFIG)/obj/sqlite3.o

$(CONFIG)/bin/libsqlite3.out: $(DEPS_27)
	@echo '      [Link] $(CONFIG)/bin/libsqlite3.out'
	$(CC) -r -o $(CONFIG)/bin/libsqlite3.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/sqlite3.o" $(LIBS) 
endif

#
#   sqlite.o
#
DEPS_28 += $(CONFIG)/inc/bit.h
DEPS_28 += $(CONFIG)/inc/sqlite3.h

$(CONFIG)/obj/sqlite.o: \
    src/deps/sqlite/sqlite.c $(DEPS_28)
	@echo '   [Compile] $(CONFIG)/obj/sqlite.o'
	$(CC) -c -o $(CONFIG)/obj/sqlite.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/sqlite/sqlite.c

ifeq ($(BIT_PACK_SQLITE),1)
#
#   sqliteshell
#
DEPS_29 += $(CONFIG)/inc/sqlite3.h
DEPS_29 += $(CONFIG)/inc/bit.h
DEPS_29 += $(CONFIG)/obj/sqlite3.o
DEPS_29 += $(CONFIG)/bin/libsqlite3.out
DEPS_29 += $(CONFIG)/obj/sqlite.o

$(CONFIG)/bin/sqlite.out: $(DEPS_29)
	@echo '      [Link] $(CONFIG)/bin/sqlite.out'
	$(CC) -o $(CONFIG)/bin/sqlite.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/sqlite.o" $(LIBS) -Wl,-r 
endif

#
#   appweb.h
#
$(CONFIG)/inc/appweb.h: $(DEPS_30)
	@echo '      [Copy] $(CONFIG)/inc/appweb.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/appweb.h $(CONFIG)/inc/appweb.h

#
#   customize.h
#
$(CONFIG)/inc/customize.h: $(DEPS_31)
	@echo '      [Copy] $(CONFIG)/inc/customize.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/customize.h $(CONFIG)/inc/customize.h

#
#   config.o
#
DEPS_32 += $(CONFIG)/inc/bit.h
DEPS_32 += $(CONFIG)/inc/appweb.h
DEPS_32 += $(CONFIG)/inc/pcre.h
DEPS_32 += $(CONFIG)/inc/mpr.h
DEPS_32 += $(CONFIG)/inc/http.h
DEPS_32 += $(CONFIG)/inc/customize.h

$(CONFIG)/obj/config.o: \
    src/config.c $(DEPS_32)
	@echo '   [Compile] $(CONFIG)/obj/config.o'
	$(CC) -c -o $(CONFIG)/obj/config.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/config.c

#
#   convenience.o
#
DEPS_33 += $(CONFIG)/inc/bit.h
DEPS_33 += $(CONFIG)/inc/appweb.h

$(CONFIG)/obj/convenience.o: \
    src/convenience.c $(DEPS_33)
	@echo '   [Compile] $(CONFIG)/obj/convenience.o'
	$(CC) -c -o $(CONFIG)/obj/convenience.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/convenience.c

#
#   dirHandler.o
#
DEPS_34 += $(CONFIG)/inc/bit.h
DEPS_34 += $(CONFIG)/inc/appweb.h

$(CONFIG)/obj/dirHandler.o: \
    src/dirHandler.c $(DEPS_34)
	@echo '   [Compile] $(CONFIG)/obj/dirHandler.o'
	$(CC) -c -o $(CONFIG)/obj/dirHandler.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/dirHandler.c

#
#   fileHandler.o
#
DEPS_35 += $(CONFIG)/inc/bit.h
DEPS_35 += $(CONFIG)/inc/appweb.h

$(CONFIG)/obj/fileHandler.o: \
    src/fileHandler.c $(DEPS_35)
	@echo '   [Compile] $(CONFIG)/obj/fileHandler.o'
	$(CC) -c -o $(CONFIG)/obj/fileHandler.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/fileHandler.c

#
#   log.o
#
DEPS_36 += $(CONFIG)/inc/bit.h
DEPS_36 += $(CONFIG)/inc/appweb.h

$(CONFIG)/obj/log.o: \
    src/log.c $(DEPS_36)
	@echo '   [Compile] $(CONFIG)/obj/log.o'
	$(CC) -c -o $(CONFIG)/obj/log.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/log.c

#
#   server.o
#
DEPS_37 += $(CONFIG)/inc/bit.h
DEPS_37 += $(CONFIG)/inc/appweb.h

$(CONFIG)/obj/server.o: \
    src/server.c $(DEPS_37)
	@echo '   [Compile] $(CONFIG)/obj/server.o'
	$(CC) -c -o $(CONFIG)/obj/server.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/server.c

#
#   libappweb
#
DEPS_38 += $(CONFIG)/inc/mpr.h
DEPS_38 += $(CONFIG)/inc/bit.h
DEPS_38 += $(CONFIG)/inc/bitos.h
DEPS_38 += $(CONFIG)/obj/mprLib.o
DEPS_38 += $(CONFIG)/bin/libmpr.out
DEPS_38 += $(CONFIG)/inc/pcre.h
DEPS_38 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_38 += $(CONFIG)/bin/libpcre.out
endif
DEPS_38 += $(CONFIG)/inc/http.h
DEPS_38 += $(CONFIG)/obj/httpLib.o
DEPS_38 += $(CONFIG)/bin/libhttp.out
DEPS_38 += $(CONFIG)/inc/appweb.h
DEPS_38 += $(CONFIG)/inc/customize.h
DEPS_38 += $(CONFIG)/obj/config.o
DEPS_38 += $(CONFIG)/obj/convenience.o
DEPS_38 += $(CONFIG)/obj/dirHandler.o
DEPS_38 += $(CONFIG)/obj/fileHandler.o
DEPS_38 += $(CONFIG)/obj/log.o
DEPS_38 += $(CONFIG)/obj/server.o

$(CONFIG)/bin/libappweb.out: $(DEPS_38)
	@echo '      [Link] $(CONFIG)/bin/libappweb.out'
	$(CC) -r -o $(CONFIG)/bin/libappweb.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/config.o" "$(CONFIG)/obj/convenience.o" "$(CONFIG)/obj/dirHandler.o" "$(CONFIG)/obj/fileHandler.o" "$(CONFIG)/obj/log.o" "$(CONFIG)/obj/server.o" $(LIBS) 

#
#   edi.h
#
$(CONFIG)/inc/edi.h: $(DEPS_39)
	@echo '      [Copy] $(CONFIG)/inc/edi.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/esp/edi.h $(CONFIG)/inc/edi.h

#
#   esp-app.h
#
$(CONFIG)/inc/esp-app.h: $(DEPS_40)
	@echo '      [Copy] $(CONFIG)/inc/esp-app.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/esp/esp-app.h $(CONFIG)/inc/esp-app.h

#
#   esp.h
#
$(CONFIG)/inc/esp.h: $(DEPS_41)
	@echo '      [Copy] $(CONFIG)/inc/esp.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/esp/esp.h $(CONFIG)/inc/esp.h

#
#   mdb.h
#
$(CONFIG)/inc/mdb.h: $(DEPS_42)
	@echo '      [Copy] $(CONFIG)/inc/mdb.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/esp/mdb.h $(CONFIG)/inc/mdb.h

#
#   edi.o
#
DEPS_43 += $(CONFIG)/inc/bit.h
DEPS_43 += $(CONFIG)/inc/edi.h
DEPS_43 += $(CONFIG)/inc/pcre.h

$(CONFIG)/obj/edi.o: \
    src/esp/edi.c $(DEPS_43)
	@echo '   [Compile] $(CONFIG)/obj/edi.o'
	$(CC) -c -o $(CONFIG)/obj/edi.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/esp/edi.c

#
#   espAbbrev.o
#
DEPS_44 += $(CONFIG)/inc/bit.h
DEPS_44 += $(CONFIG)/inc/esp.h

$(CONFIG)/obj/espAbbrev.o: \
    src/esp/espAbbrev.c $(DEPS_44)
	@echo '   [Compile] $(CONFIG)/obj/espAbbrev.o'
	$(CC) -c -o $(CONFIG)/obj/espAbbrev.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/esp/espAbbrev.c

#
#   espFramework.o
#
DEPS_45 += $(CONFIG)/inc/bit.h
DEPS_45 += $(CONFIG)/inc/esp.h

$(CONFIG)/obj/espFramework.o: \
    src/esp/espFramework.c $(DEPS_45)
	@echo '   [Compile] $(CONFIG)/obj/espFramework.o'
	$(CC) -c -o $(CONFIG)/obj/espFramework.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/esp/espFramework.c

#
#   espHandler.o
#
DEPS_46 += $(CONFIG)/inc/bit.h
DEPS_46 += $(CONFIG)/inc/appweb.h
DEPS_46 += $(CONFIG)/inc/esp.h
DEPS_46 += $(CONFIG)/inc/edi.h

$(CONFIG)/obj/espHandler.o: \
    src/esp/espHandler.c $(DEPS_46)
	@echo '   [Compile] $(CONFIG)/obj/espHandler.o'
	$(CC) -c -o $(CONFIG)/obj/espHandler.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/esp/espHandler.c

#
#   espHtml.o
#
DEPS_47 += $(CONFIG)/inc/bit.h
DEPS_47 += $(CONFIG)/inc/esp.h
DEPS_47 += $(CONFIG)/inc/edi.h

$(CONFIG)/obj/espHtml.o: \
    src/esp/espHtml.c $(DEPS_47)
	@echo '   [Compile] $(CONFIG)/obj/espHtml.o'
	$(CC) -c -o $(CONFIG)/obj/espHtml.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/esp/espHtml.c

#
#   espTemplate.o
#
DEPS_48 += $(CONFIG)/inc/bit.h
DEPS_48 += $(CONFIG)/inc/esp.h

$(CONFIG)/obj/espTemplate.o: \
    src/esp/espTemplate.c $(DEPS_48)
	@echo '   [Compile] $(CONFIG)/obj/espTemplate.o'
	$(CC) -c -o $(CONFIG)/obj/espTemplate.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/esp/espTemplate.c

#
#   mdb.o
#
DEPS_49 += $(CONFIG)/inc/bit.h
DEPS_49 += $(CONFIG)/inc/appweb.h
DEPS_49 += $(CONFIG)/inc/edi.h
DEPS_49 += $(CONFIG)/inc/mdb.h
DEPS_49 += $(CONFIG)/inc/pcre.h

$(CONFIG)/obj/mdb.o: \
    src/esp/mdb.c $(DEPS_49)
	@echo '   [Compile] $(CONFIG)/obj/mdb.o'
	$(CC) -c -o $(CONFIG)/obj/mdb.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/esp/mdb.c

#
#   sdb.o
#
DEPS_50 += $(CONFIG)/inc/bit.h
DEPS_50 += $(CONFIG)/inc/appweb.h

$(CONFIG)/obj/sdb.o: \
    src/esp/sdb.c $(DEPS_50)
	@echo '   [Compile] $(CONFIG)/obj/sdb.o'
	$(CC) -c -o $(CONFIG)/obj/sdb.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/esp/sdb.c

ifeq ($(BIT_PACK_ESP),1)
#
#   libmod_esp
#
DEPS_51 += $(CONFIG)/inc/mpr.h
DEPS_51 += $(CONFIG)/inc/bit.h
DEPS_51 += $(CONFIG)/inc/bitos.h
DEPS_51 += $(CONFIG)/obj/mprLib.o
DEPS_51 += $(CONFIG)/bin/libmpr.out
DEPS_51 += $(CONFIG)/inc/pcre.h
DEPS_51 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_51 += $(CONFIG)/bin/libpcre.out
endif
DEPS_51 += $(CONFIG)/inc/http.h
DEPS_51 += $(CONFIG)/obj/httpLib.o
DEPS_51 += $(CONFIG)/bin/libhttp.out
DEPS_51 += $(CONFIG)/inc/appweb.h
DEPS_51 += $(CONFIG)/inc/customize.h
DEPS_51 += $(CONFIG)/obj/config.o
DEPS_51 += $(CONFIG)/obj/convenience.o
DEPS_51 += $(CONFIG)/obj/dirHandler.o
DEPS_51 += $(CONFIG)/obj/fileHandler.o
DEPS_51 += $(CONFIG)/obj/log.o
DEPS_51 += $(CONFIG)/obj/server.o
DEPS_51 += $(CONFIG)/bin/libappweb.out
DEPS_51 += $(CONFIG)/inc/edi.h
DEPS_51 += $(CONFIG)/inc/esp-app.h
DEPS_51 += $(CONFIG)/inc/esp.h
DEPS_51 += $(CONFIG)/inc/mdb.h
DEPS_51 += $(CONFIG)/obj/edi.o
DEPS_51 += $(CONFIG)/obj/espAbbrev.o
DEPS_51 += $(CONFIG)/obj/espFramework.o
DEPS_51 += $(CONFIG)/obj/espHandler.o
DEPS_51 += $(CONFIG)/obj/espHtml.o
DEPS_51 += $(CONFIG)/obj/espTemplate.o
DEPS_51 += $(CONFIG)/obj/mdb.o
DEPS_51 += $(CONFIG)/obj/sdb.o

$(CONFIG)/bin/libmod_esp.out: $(DEPS_51)
	@echo '      [Link] $(CONFIG)/bin/libmod_esp.out'
	$(CC) -r -o $(CONFIG)/bin/libmod_esp.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/edi.o" "$(CONFIG)/obj/espAbbrev.o" "$(CONFIG)/obj/espFramework.o" "$(CONFIG)/obj/espHandler.o" "$(CONFIG)/obj/espHtml.o" "$(CONFIG)/obj/espTemplate.o" "$(CONFIG)/obj/mdb.o" "$(CONFIG)/obj/sdb.o" $(LIBS) 
endif

#
#   esp.o
#
DEPS_52 += $(CONFIG)/inc/bit.h
DEPS_52 += $(CONFIG)/inc/esp.h

$(CONFIG)/obj/esp.o: \
    src/esp/esp.c $(DEPS_52)
	@echo '   [Compile] $(CONFIG)/obj/esp.o'
	$(CC) -c -o $(CONFIG)/obj/esp.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/esp/esp.c

ifeq ($(BIT_PACK_ESP),1)
#
#   espcmd
#
DEPS_53 += $(CONFIG)/inc/mpr.h
DEPS_53 += $(CONFIG)/inc/bit.h
DEPS_53 += $(CONFIG)/inc/bitos.h
DEPS_53 += $(CONFIG)/obj/mprLib.o
DEPS_53 += $(CONFIG)/bin/libmpr.out
DEPS_53 += $(CONFIG)/inc/pcre.h
DEPS_53 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_53 += $(CONFIG)/bin/libpcre.out
endif
DEPS_53 += $(CONFIG)/inc/http.h
DEPS_53 += $(CONFIG)/obj/httpLib.o
DEPS_53 += $(CONFIG)/bin/libhttp.out
DEPS_53 += $(CONFIG)/inc/appweb.h
DEPS_53 += $(CONFIG)/inc/customize.h
DEPS_53 += $(CONFIG)/obj/config.o
DEPS_53 += $(CONFIG)/obj/convenience.o
DEPS_53 += $(CONFIG)/obj/dirHandler.o
DEPS_53 += $(CONFIG)/obj/fileHandler.o
DEPS_53 += $(CONFIG)/obj/log.o
DEPS_53 += $(CONFIG)/obj/server.o
DEPS_53 += $(CONFIG)/bin/libappweb.out
DEPS_53 += $(CONFIG)/inc/edi.h
DEPS_53 += $(CONFIG)/obj/edi.o
DEPS_53 += $(CONFIG)/inc/esp.h
DEPS_53 += $(CONFIG)/obj/esp.o
DEPS_53 += $(CONFIG)/obj/espAbbrev.o
DEPS_53 += $(CONFIG)/obj/espFramework.o
DEPS_53 += $(CONFIG)/obj/espHandler.o
DEPS_53 += $(CONFIG)/obj/espHtml.o
DEPS_53 += $(CONFIG)/obj/espTemplate.o
DEPS_53 += $(CONFIG)/inc/mdb.h
DEPS_53 += $(CONFIG)/obj/mdb.o
DEPS_53 += $(CONFIG)/obj/sdb.o

$(CONFIG)/bin/esp.out: $(DEPS_53)
	@echo '      [Link] $(CONFIG)/bin/esp.out'
	$(CC) -o $(CONFIG)/bin/esp.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/edi.o" "$(CONFIG)/obj/esp.o" "$(CONFIG)/obj/espAbbrev.o" "$(CONFIG)/obj/espFramework.o" "$(CONFIG)/obj/espHandler.o" "$(CONFIG)/obj/espHtml.o" "$(CONFIG)/obj/espTemplate.o" "$(CONFIG)/obj/mdb.o" "$(CONFIG)/obj/sdb.o" $(LIBS) -Wl,-r 
endif

ifeq ($(BIT_PACK_ESP),1)
#
#   esp.conf
#
DEPS_54 += src/esp/esp.conf

$(CONFIG)/bin/esp.conf: $(DEPS_54)
	@echo '      [Copy] $(CONFIG)/bin/esp.conf'
	mkdir -p "$(CONFIG)/bin"
	cp src/esp/esp.conf $(CONFIG)/bin/esp.conf
endif

ifeq ($(BIT_PACK_ESP),1)
#
#   esp.conf.server
#
DEPS_55 += src/esp/esp.conf

src/server/esp.conf: $(DEPS_55)
	@echo '      [Copy] src/server/esp.conf'
	mkdir -p "src/server"
	cp src/esp/esp.conf src/server/esp.conf
endif

ifeq ($(BIT_PACK_ESP),1)
#
#   esp.www
#
DEPS_56 += src/esp/esp-www

$(CONFIG)/bin/esp-www: $(DEPS_56)
	@echo '      [Copy] $(CONFIG)/bin/esp-www'
	mkdir -p "$(CONFIG)/bin/esp-www"
	cp src/esp/esp-www/app.conf $(CONFIG)/bin/esp-www/app.conf
	cp src/esp/esp-www/appweb.conf $(CONFIG)/bin/esp-www/appweb.conf
	mkdir -p "$(CONFIG)/bin/esp-www/files/layouts"
	cp src/esp/esp-www/files/layouts/default.esp $(CONFIG)/bin/esp-www/files/layouts/default.esp
	mkdir -p "$(CONFIG)/bin/esp-www/files/static/images"
	cp src/esp/esp-www/files/static/images/banner.jpg $(CONFIG)/bin/esp-www/files/static/images/banner.jpg
	cp src/esp/esp-www/files/static/images/favicon.ico $(CONFIG)/bin/esp-www/files/static/images/favicon.ico
	cp src/esp/esp-www/files/static/images/splash.jpg $(CONFIG)/bin/esp-www/files/static/images/splash.jpg
	mkdir -p "$(CONFIG)/bin/esp-www/files/static"
	cp src/esp/esp-www/files/static/index.esp $(CONFIG)/bin/esp-www/files/static/index.esp
	mkdir -p "$(CONFIG)/bin/esp-www/files/static/js"
	cp src/esp/esp-www/files/static/js/jquery.esp.js $(CONFIG)/bin/esp-www/files/static/js/jquery.esp.js
	cp src/esp/esp-www/files/static/js/jquery.js $(CONFIG)/bin/esp-www/files/static/js/jquery.js
	cp src/esp/esp-www/files/static/js/jquery.min.js $(CONFIG)/bin/esp-www/files/static/js/jquery.min.js
	cp src/esp/esp-www/files/static/js/jquery.simplemodal.js $(CONFIG)/bin/esp-www/files/static/js/jquery.simplemodal.js
	cp src/esp/esp-www/files/static/js/jquery.tablesorter.js $(CONFIG)/bin/esp-www/files/static/js/jquery.tablesorter.js
	cp src/esp/esp-www/files/static/layout.css $(CONFIG)/bin/esp-www/files/static/layout.css
	mkdir -p "$(CONFIG)/bin/esp-www/files/static/themes"
	cp src/esp/esp-www/files/static/themes/default.css $(CONFIG)/bin/esp-www/files/static/themes/default.css
endif

#
#   ejs.h
#
$(CONFIG)/inc/ejs.h: $(DEPS_57)
	@echo '      [Copy] $(CONFIG)/inc/ejs.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/deps/ejs/ejs.h $(CONFIG)/inc/ejs.h

#
#   ejs.slots.h
#
$(CONFIG)/inc/ejs.slots.h: $(DEPS_58)
	@echo '      [Copy] $(CONFIG)/inc/ejs.slots.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/deps/ejs/ejs.slots.h $(CONFIG)/inc/ejs.slots.h

#
#   ejsByteGoto.h
#
$(CONFIG)/inc/ejsByteGoto.h: $(DEPS_59)
	@echo '      [Copy] $(CONFIG)/inc/ejsByteGoto.h'
	mkdir -p "$(CONFIG)/inc"
	cp src/deps/ejs/ejsByteGoto.h $(CONFIG)/inc/ejsByteGoto.h

#
#   ejsLib.o
#
DEPS_60 += $(CONFIG)/inc/bit.h
DEPS_60 += $(CONFIG)/inc/ejs.h
DEPS_60 += $(CONFIG)/inc/mpr.h
DEPS_60 += $(CONFIG)/inc/pcre.h
DEPS_60 += $(CONFIG)/inc/bitos.h
DEPS_60 += $(CONFIG)/inc/http.h
DEPS_60 += $(CONFIG)/inc/ejs.slots.h

$(CONFIG)/obj/ejsLib.o: \
    src/deps/ejs/ejsLib.c $(DEPS_60)
	@echo '   [Compile] $(CONFIG)/obj/ejsLib.o'
	$(CC) -c -o $(CONFIG)/obj/ejsLib.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/ejs/ejsLib.c

ifeq ($(BIT_PACK_EJSCRIPT),1)
#
#   libejs
#
DEPS_61 += $(CONFIG)/inc/mpr.h
DEPS_61 += $(CONFIG)/inc/bit.h
DEPS_61 += $(CONFIG)/inc/bitos.h
DEPS_61 += $(CONFIG)/obj/mprLib.o
DEPS_61 += $(CONFIG)/bin/libmpr.out
DEPS_61 += $(CONFIG)/inc/pcre.h
DEPS_61 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_61 += $(CONFIG)/bin/libpcre.out
endif
DEPS_61 += $(CONFIG)/inc/http.h
DEPS_61 += $(CONFIG)/obj/httpLib.o
DEPS_61 += $(CONFIG)/bin/libhttp.out
DEPS_61 += $(CONFIG)/inc/ejs.h
DEPS_61 += $(CONFIG)/inc/ejs.slots.h
DEPS_61 += $(CONFIG)/inc/ejsByteGoto.h
DEPS_61 += $(CONFIG)/obj/ejsLib.o

$(CONFIG)/bin/libejs.out: $(DEPS_61)
	@echo '      [Link] $(CONFIG)/bin/libejs.out'
	$(CC) -r -o $(CONFIG)/bin/libejs.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/ejsLib.o" $(LIBS) 
endif

#
#   ejs.o
#
DEPS_62 += $(CONFIG)/inc/bit.h
DEPS_62 += $(CONFIG)/inc/ejs.h

$(CONFIG)/obj/ejs.o: \
    src/deps/ejs/ejs.c $(DEPS_62)
	@echo '   [Compile] $(CONFIG)/obj/ejs.o'
	$(CC) -c -o $(CONFIG)/obj/ejs.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/ejs/ejs.c

ifeq ($(BIT_PACK_EJSCRIPT),1)
#
#   ejs
#
DEPS_63 += $(CONFIG)/inc/mpr.h
DEPS_63 += $(CONFIG)/inc/bit.h
DEPS_63 += $(CONFIG)/inc/bitos.h
DEPS_63 += $(CONFIG)/obj/mprLib.o
DEPS_63 += $(CONFIG)/bin/libmpr.out
DEPS_63 += $(CONFIG)/inc/pcre.h
DEPS_63 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_63 += $(CONFIG)/bin/libpcre.out
endif
DEPS_63 += $(CONFIG)/inc/http.h
DEPS_63 += $(CONFIG)/obj/httpLib.o
DEPS_63 += $(CONFIG)/bin/libhttp.out
DEPS_63 += $(CONFIG)/inc/ejs.h
DEPS_63 += $(CONFIG)/inc/ejs.slots.h
DEPS_63 += $(CONFIG)/inc/ejsByteGoto.h
DEPS_63 += $(CONFIG)/obj/ejsLib.o
DEPS_63 += $(CONFIG)/bin/libejs.out
DEPS_63 += $(CONFIG)/obj/ejs.o

$(CONFIG)/bin/ejs.out: $(DEPS_63)
	@echo '      [Link] $(CONFIG)/bin/ejs.out'
	$(CC) -o $(CONFIG)/bin/ejs.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/ejs.o" $(LIBS) -Wl,-r 
endif

#
#   ejsc.o
#
DEPS_64 += $(CONFIG)/inc/bit.h
DEPS_64 += $(CONFIG)/inc/ejs.h

$(CONFIG)/obj/ejsc.o: \
    src/deps/ejs/ejsc.c $(DEPS_64)
	@echo '   [Compile] $(CONFIG)/obj/ejsc.o'
	$(CC) -c -o $(CONFIG)/obj/ejsc.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/deps/ejs/ejsc.c

ifeq ($(BIT_PACK_EJSCRIPT),1)
#
#   ejsc
#
DEPS_65 += $(CONFIG)/inc/mpr.h
DEPS_65 += $(CONFIG)/inc/bit.h
DEPS_65 += $(CONFIG)/inc/bitos.h
DEPS_65 += $(CONFIG)/obj/mprLib.o
DEPS_65 += $(CONFIG)/bin/libmpr.out
DEPS_65 += $(CONFIG)/inc/pcre.h
DEPS_65 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_65 += $(CONFIG)/bin/libpcre.out
endif
DEPS_65 += $(CONFIG)/inc/http.h
DEPS_65 += $(CONFIG)/obj/httpLib.o
DEPS_65 += $(CONFIG)/bin/libhttp.out
DEPS_65 += $(CONFIG)/inc/ejs.h
DEPS_65 += $(CONFIG)/inc/ejs.slots.h
DEPS_65 += $(CONFIG)/inc/ejsByteGoto.h
DEPS_65 += $(CONFIG)/obj/ejsLib.o
DEPS_65 += $(CONFIG)/bin/libejs.out
DEPS_65 += $(CONFIG)/obj/ejsc.o

$(CONFIG)/bin/ejsc.out: $(DEPS_65)
	@echo '      [Link] $(CONFIG)/bin/ejsc.out'
	$(CC) -o $(CONFIG)/bin/ejsc.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/ejsc.o" $(LIBS) -Wl,-r 
endif

ifeq ($(BIT_PACK_EJSCRIPT),1)
#
#   ejs.mod
#
DEPS_66 += src/deps/ejs/ejs.es
DEPS_66 += $(CONFIG)/inc/mpr.h
DEPS_66 += $(CONFIG)/inc/bit.h
DEPS_66 += $(CONFIG)/inc/bitos.h
DEPS_66 += $(CONFIG)/obj/mprLib.o
DEPS_66 += $(CONFIG)/bin/libmpr.out
DEPS_66 += $(CONFIG)/inc/pcre.h
DEPS_66 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_66 += $(CONFIG)/bin/libpcre.out
endif
DEPS_66 += $(CONFIG)/inc/http.h
DEPS_66 += $(CONFIG)/obj/httpLib.o
DEPS_66 += $(CONFIG)/bin/libhttp.out
DEPS_66 += $(CONFIG)/inc/ejs.h
DEPS_66 += $(CONFIG)/inc/ejs.slots.h
DEPS_66 += $(CONFIG)/inc/ejsByteGoto.h
DEPS_66 += $(CONFIG)/obj/ejsLib.o
DEPS_66 += $(CONFIG)/bin/libejs.out
DEPS_66 += $(CONFIG)/obj/ejsc.o
DEPS_66 += $(CONFIG)/bin/ejsc.out

$(CONFIG)/bin/ejs.mod: $(DEPS_66)
	$(LBIN)/ejsc --out ./$(CONFIG)/bin/ejs.mod --optimize 9 --bind --require null src/deps/ejs/ejs.es
endif

#
#   cgiHandler.o
#
DEPS_67 += $(CONFIG)/inc/bit.h
DEPS_67 += $(CONFIG)/inc/appweb.h

$(CONFIG)/obj/cgiHandler.o: \
    src/modules/cgiHandler.c $(DEPS_67)
	@echo '   [Compile] $(CONFIG)/obj/cgiHandler.o'
	$(CC) -c -o $(CONFIG)/obj/cgiHandler.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/modules/cgiHandler.c

ifeq ($(BIT_PACK_CGI),1)
#
#   libmod_cgi
#
DEPS_68 += $(CONFIG)/inc/mpr.h
DEPS_68 += $(CONFIG)/inc/bit.h
DEPS_68 += $(CONFIG)/inc/bitos.h
DEPS_68 += $(CONFIG)/obj/mprLib.o
DEPS_68 += $(CONFIG)/bin/libmpr.out
DEPS_68 += $(CONFIG)/inc/pcre.h
DEPS_68 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_68 += $(CONFIG)/bin/libpcre.out
endif
DEPS_68 += $(CONFIG)/inc/http.h
DEPS_68 += $(CONFIG)/obj/httpLib.o
DEPS_68 += $(CONFIG)/bin/libhttp.out
DEPS_68 += $(CONFIG)/inc/appweb.h
DEPS_68 += $(CONFIG)/inc/customize.h
DEPS_68 += $(CONFIG)/obj/config.o
DEPS_68 += $(CONFIG)/obj/convenience.o
DEPS_68 += $(CONFIG)/obj/dirHandler.o
DEPS_68 += $(CONFIG)/obj/fileHandler.o
DEPS_68 += $(CONFIG)/obj/log.o
DEPS_68 += $(CONFIG)/obj/server.o
DEPS_68 += $(CONFIG)/bin/libappweb.out
DEPS_68 += $(CONFIG)/obj/cgiHandler.o

$(CONFIG)/bin/libmod_cgi.out: $(DEPS_68)
	@echo '      [Link] $(CONFIG)/bin/libmod_cgi.out'
	$(CC) -r -o $(CONFIG)/bin/libmod_cgi.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/cgiHandler.o" $(LIBS) 
endif

#
#   ejsHandler.o
#
DEPS_69 += $(CONFIG)/inc/bit.h
DEPS_69 += $(CONFIG)/inc/appweb.h

$(CONFIG)/obj/ejsHandler.o: \
    src/modules/ejsHandler.c $(DEPS_69)
	@echo '   [Compile] $(CONFIG)/obj/ejsHandler.o'
	$(CC) -c -o $(CONFIG)/obj/ejsHandler.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/modules/ejsHandler.c

ifeq ($(BIT_PACK_EJSCRIPT),1)
#
#   libmod_ejs
#
DEPS_70 += $(CONFIG)/inc/mpr.h
DEPS_70 += $(CONFIG)/inc/bit.h
DEPS_70 += $(CONFIG)/inc/bitos.h
DEPS_70 += $(CONFIG)/obj/mprLib.o
DEPS_70 += $(CONFIG)/bin/libmpr.out
DEPS_70 += $(CONFIG)/inc/pcre.h
DEPS_70 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_70 += $(CONFIG)/bin/libpcre.out
endif
DEPS_70 += $(CONFIG)/inc/http.h
DEPS_70 += $(CONFIG)/obj/httpLib.o
DEPS_70 += $(CONFIG)/bin/libhttp.out
DEPS_70 += $(CONFIG)/inc/appweb.h
DEPS_70 += $(CONFIG)/inc/customize.h
DEPS_70 += $(CONFIG)/obj/config.o
DEPS_70 += $(CONFIG)/obj/convenience.o
DEPS_70 += $(CONFIG)/obj/dirHandler.o
DEPS_70 += $(CONFIG)/obj/fileHandler.o
DEPS_70 += $(CONFIG)/obj/log.o
DEPS_70 += $(CONFIG)/obj/server.o
DEPS_70 += $(CONFIG)/bin/libappweb.out
DEPS_70 += $(CONFIG)/inc/ejs.h
DEPS_70 += $(CONFIG)/inc/ejs.slots.h
DEPS_70 += $(CONFIG)/inc/ejsByteGoto.h
DEPS_70 += $(CONFIG)/obj/ejsLib.o
DEPS_70 += $(CONFIG)/bin/libejs.out
DEPS_70 += $(CONFIG)/obj/ejsHandler.o

$(CONFIG)/bin/libmod_ejs.out: $(DEPS_70)
	@echo '      [Link] $(CONFIG)/bin/libmod_ejs.out'
	$(CC) -r -o $(CONFIG)/bin/libmod_ejs.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/ejsHandler.o" $(LIBS) 
endif

#
#   phpHandler.o
#
DEPS_71 += $(CONFIG)/inc/bit.h
DEPS_71 += $(CONFIG)/inc/appweb.h

$(CONFIG)/obj/phpHandler.o: \
    src/modules/phpHandler.c $(DEPS_71)
	@echo '   [Compile] $(CONFIG)/obj/phpHandler.o'
	$(CC) -c -o $(CONFIG)/obj/phpHandler.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" "-I$(BIT_PACK_PHP_PATH)" "-I$(BIT_PACK_PHP_PATH)/main" "-I$(BIT_PACK_PHP_PATH)/Zend" "-I$(BIT_PACK_PHP_PATH)/TSRM" src/modules/phpHandler.c

ifeq ($(BIT_PACK_PHP),1)
#
#   libmod_php
#
DEPS_72 += $(CONFIG)/inc/mpr.h
DEPS_72 += $(CONFIG)/inc/bit.h
DEPS_72 += $(CONFIG)/inc/bitos.h
DEPS_72 += $(CONFIG)/obj/mprLib.o
DEPS_72 += $(CONFIG)/bin/libmpr.out
DEPS_72 += $(CONFIG)/inc/pcre.h
DEPS_72 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_72 += $(CONFIG)/bin/libpcre.out
endif
DEPS_72 += $(CONFIG)/inc/http.h
DEPS_72 += $(CONFIG)/obj/httpLib.o
DEPS_72 += $(CONFIG)/bin/libhttp.out
DEPS_72 += $(CONFIG)/inc/appweb.h
DEPS_72 += $(CONFIG)/inc/customize.h
DEPS_72 += $(CONFIG)/obj/config.o
DEPS_72 += $(CONFIG)/obj/convenience.o
DEPS_72 += $(CONFIG)/obj/dirHandler.o
DEPS_72 += $(CONFIG)/obj/fileHandler.o
DEPS_72 += $(CONFIG)/obj/log.o
DEPS_72 += $(CONFIG)/obj/server.o
DEPS_72 += $(CONFIG)/bin/libappweb.out
DEPS_72 += $(CONFIG)/obj/phpHandler.o

LIBS_72 += -lphp5
LIBPATHS_72 += -L$(BIT_PACK_PHP_PATH)/libs

$(CONFIG)/bin/libmod_php.out: $(DEPS_72)
	@echo '      [Link] $(CONFIG)/bin/libmod_php.out'
	$(CC) -r -o $(CONFIG)/bin/libmod_php.out $(LDFLAGS) $(LIBPATHS)  "$(CONFIG)/obj/phpHandler.o" $(LIBPATHS_72) $(LIBS_72) $(LIBS_72) $(LIBS) 
endif

#
#   sslModule.o
#
DEPS_73 += $(CONFIG)/inc/bit.h
DEPS_73 += $(CONFIG)/inc/appweb.h

$(CONFIG)/obj/sslModule.o: \
    src/modules/sslModule.c $(DEPS_73)
	@echo '   [Compile] $(CONFIG)/obj/sslModule.o'
	$(CC) -c -o $(CONFIG)/obj/sslModule.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/modules/sslModule.c

ifeq ($(BIT_PACK_SSL),1)
#
#   libmod_ssl
#
DEPS_74 += $(CONFIG)/inc/mpr.h
DEPS_74 += $(CONFIG)/inc/bit.h
DEPS_74 += $(CONFIG)/inc/bitos.h
DEPS_74 += $(CONFIG)/obj/mprLib.o
DEPS_74 += $(CONFIG)/bin/libmpr.out
DEPS_74 += $(CONFIG)/inc/pcre.h
DEPS_74 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_74 += $(CONFIG)/bin/libpcre.out
endif
DEPS_74 += $(CONFIG)/inc/http.h
DEPS_74 += $(CONFIG)/obj/httpLib.o
DEPS_74 += $(CONFIG)/bin/libhttp.out
DEPS_74 += $(CONFIG)/inc/appweb.h
DEPS_74 += $(CONFIG)/inc/customize.h
DEPS_74 += $(CONFIG)/obj/config.o
DEPS_74 += $(CONFIG)/obj/convenience.o
DEPS_74 += $(CONFIG)/obj/dirHandler.o
DEPS_74 += $(CONFIG)/obj/fileHandler.o
DEPS_74 += $(CONFIG)/obj/log.o
DEPS_74 += $(CONFIG)/obj/server.o
DEPS_74 += $(CONFIG)/bin/libappweb.out
DEPS_74 += $(CONFIG)/obj/sslModule.o

$(CONFIG)/bin/libmod_ssl.out: $(DEPS_74)
	@echo '      [Link] $(CONFIG)/bin/libmod_ssl.out'
	$(CC) -r -o $(CONFIG)/bin/libmod_ssl.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/sslModule.o" $(LIBS) 
endif

#
#   authpass.o
#
DEPS_75 += $(CONFIG)/inc/bit.h
DEPS_75 += $(CONFIG)/inc/appweb.h

$(CONFIG)/obj/authpass.o: \
    src/utils/authpass.c $(DEPS_75)
	@echo '   [Compile] $(CONFIG)/obj/authpass.o'
	$(CC) -c -o $(CONFIG)/obj/authpass.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/utils/authpass.c

#
#   authpass
#
DEPS_76 += $(CONFIG)/inc/mpr.h
DEPS_76 += $(CONFIG)/inc/bit.h
DEPS_76 += $(CONFIG)/inc/bitos.h
DEPS_76 += $(CONFIG)/obj/mprLib.o
DEPS_76 += $(CONFIG)/bin/libmpr.out
DEPS_76 += $(CONFIG)/inc/pcre.h
DEPS_76 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_76 += $(CONFIG)/bin/libpcre.out
endif
DEPS_76 += $(CONFIG)/inc/http.h
DEPS_76 += $(CONFIG)/obj/httpLib.o
DEPS_76 += $(CONFIG)/bin/libhttp.out
DEPS_76 += $(CONFIG)/inc/appweb.h
DEPS_76 += $(CONFIG)/inc/customize.h
DEPS_76 += $(CONFIG)/obj/config.o
DEPS_76 += $(CONFIG)/obj/convenience.o
DEPS_76 += $(CONFIG)/obj/dirHandler.o
DEPS_76 += $(CONFIG)/obj/fileHandler.o
DEPS_76 += $(CONFIG)/obj/log.o
DEPS_76 += $(CONFIG)/obj/server.o
DEPS_76 += $(CONFIG)/bin/libappweb.out
DEPS_76 += $(CONFIG)/obj/authpass.o

$(CONFIG)/bin/authpass.out: $(DEPS_76)
	@echo '      [Link] $(CONFIG)/bin/authpass.out'
	$(CC) -o $(CONFIG)/bin/authpass.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/authpass.o" $(LIBS) -Wl,-r 

#
#   cgiProgram.o
#
DEPS_77 += $(CONFIG)/inc/bit.h

$(CONFIG)/obj/cgiProgram.o: \
    src/utils/cgiProgram.c $(DEPS_77)
	@echo '   [Compile] $(CONFIG)/obj/cgiProgram.o'
	$(CC) -c -o $(CONFIG)/obj/cgiProgram.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/utils/cgiProgram.c

ifeq ($(BIT_PACK_CGI),1)
#
#   cgiProgram
#
DEPS_78 += $(CONFIG)/inc/bit.h
DEPS_78 += $(CONFIG)/obj/cgiProgram.o

$(CONFIG)/bin/cgiProgram.out: $(DEPS_78)
	@echo '      [Link] $(CONFIG)/bin/cgiProgram.out'
	$(CC) -o $(CONFIG)/bin/cgiProgram.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/cgiProgram.o" $(LIBS) -Wl,-r 
endif

#
#   slink.c
#
src/server/slink.c: $(DEPS_79)
	cd src/server; [ ! -f slink.c ] && cp slink.empty slink.c ; true ; cd ../..

#
#   slink.o
#
DEPS_80 += $(CONFIG)/inc/bit.h
DEPS_80 += $(CONFIG)/inc/esp.h

$(CONFIG)/obj/slink.o: \
    src/server/slink.c $(DEPS_80)
	@echo '   [Compile] $(CONFIG)/obj/slink.o'
	$(CC) -c -o $(CONFIG)/obj/slink.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/server/slink.c

#
#   libslink
#
DEPS_81 += src/server/slink.c
DEPS_81 += $(CONFIG)/inc/mpr.h
DEPS_81 += $(CONFIG)/inc/bit.h
DEPS_81 += $(CONFIG)/inc/bitos.h
DEPS_81 += $(CONFIG)/obj/mprLib.o
DEPS_81 += $(CONFIG)/bin/libmpr.out
DEPS_81 += $(CONFIG)/inc/pcre.h
DEPS_81 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_81 += $(CONFIG)/bin/libpcre.out
endif
DEPS_81 += $(CONFIG)/inc/http.h
DEPS_81 += $(CONFIG)/obj/httpLib.o
DEPS_81 += $(CONFIG)/bin/libhttp.out
DEPS_81 += $(CONFIG)/inc/appweb.h
DEPS_81 += $(CONFIG)/inc/customize.h
DEPS_81 += $(CONFIG)/obj/config.o
DEPS_81 += $(CONFIG)/obj/convenience.o
DEPS_81 += $(CONFIG)/obj/dirHandler.o
DEPS_81 += $(CONFIG)/obj/fileHandler.o
DEPS_81 += $(CONFIG)/obj/log.o
DEPS_81 += $(CONFIG)/obj/server.o
DEPS_81 += $(CONFIG)/bin/libappweb.out
DEPS_81 += $(CONFIG)/inc/edi.h
DEPS_81 += $(CONFIG)/inc/esp-app.h
DEPS_81 += $(CONFIG)/inc/esp.h
DEPS_81 += $(CONFIG)/inc/mdb.h
DEPS_81 += $(CONFIG)/obj/edi.o
DEPS_81 += $(CONFIG)/obj/espAbbrev.o
DEPS_81 += $(CONFIG)/obj/espFramework.o
DEPS_81 += $(CONFIG)/obj/espHandler.o
DEPS_81 += $(CONFIG)/obj/espHtml.o
DEPS_81 += $(CONFIG)/obj/espTemplate.o
DEPS_81 += $(CONFIG)/obj/mdb.o
DEPS_81 += $(CONFIG)/obj/sdb.o
ifeq ($(BIT_PACK_ESP),1)
    DEPS_81 += $(CONFIG)/bin/libmod_esp.out
endif
DEPS_81 += $(CONFIG)/obj/slink.o

$(CONFIG)/bin/libslink.out: $(DEPS_81)
	@echo '      [Link] $(CONFIG)/bin/libslink.out'
	$(CC) -r -o $(CONFIG)/bin/libslink.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/slink.o" $(LIBS) 

#
#   appweb.o
#
DEPS_82 += $(CONFIG)/inc/bit.h
DEPS_82 += $(CONFIG)/inc/appweb.h
DEPS_82 += $(CONFIG)/inc/esp.h

$(CONFIG)/obj/appweb.o: \
    src/server/appweb.c $(DEPS_82)
	@echo '   [Compile] $(CONFIG)/obj/appweb.o'
	$(CC) -c -o $(CONFIG)/obj/appweb.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" src/server/appweb.c

#
#   appweb
#
DEPS_83 += $(CONFIG)/inc/mpr.h
DEPS_83 += $(CONFIG)/inc/bit.h
DEPS_83 += $(CONFIG)/inc/bitos.h
DEPS_83 += $(CONFIG)/obj/mprLib.o
DEPS_83 += $(CONFIG)/bin/libmpr.out
DEPS_83 += $(CONFIG)/inc/pcre.h
DEPS_83 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_83 += $(CONFIG)/bin/libpcre.out
endif
DEPS_83 += $(CONFIG)/inc/http.h
DEPS_83 += $(CONFIG)/obj/httpLib.o
DEPS_83 += $(CONFIG)/bin/libhttp.out
DEPS_83 += $(CONFIG)/inc/appweb.h
DEPS_83 += $(CONFIG)/inc/customize.h
DEPS_83 += $(CONFIG)/obj/config.o
DEPS_83 += $(CONFIG)/obj/convenience.o
DEPS_83 += $(CONFIG)/obj/dirHandler.o
DEPS_83 += $(CONFIG)/obj/fileHandler.o
DEPS_83 += $(CONFIG)/obj/log.o
DEPS_83 += $(CONFIG)/obj/server.o
DEPS_83 += $(CONFIG)/bin/libappweb.out
DEPS_83 += src/server/slink.c
DEPS_83 += $(CONFIG)/inc/edi.h
DEPS_83 += $(CONFIG)/inc/esp-app.h
DEPS_83 += $(CONFIG)/inc/esp.h
DEPS_83 += $(CONFIG)/inc/mdb.h
DEPS_83 += $(CONFIG)/obj/edi.o
DEPS_83 += $(CONFIG)/obj/espAbbrev.o
DEPS_83 += $(CONFIG)/obj/espFramework.o
DEPS_83 += $(CONFIG)/obj/espHandler.o
DEPS_83 += $(CONFIG)/obj/espHtml.o
DEPS_83 += $(CONFIG)/obj/espTemplate.o
DEPS_83 += $(CONFIG)/obj/mdb.o
DEPS_83 += $(CONFIG)/obj/sdb.o
ifeq ($(BIT_PACK_ESP),1)
    DEPS_83 += $(CONFIG)/bin/libmod_esp.out
endif
DEPS_83 += $(CONFIG)/obj/slink.o
DEPS_83 += $(CONFIG)/bin/libslink.out
DEPS_83 += $(CONFIG)/obj/appweb.o

$(CONFIG)/bin/appweb.out: $(DEPS_83)
	@echo '      [Link] $(CONFIG)/bin/appweb.out'
	$(CC) -o $(CONFIG)/bin/appweb.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/appweb.o" $(LIBS) -Wl,-r 

#
#   server-cache
#
src/server/cache: $(DEPS_84)
	cd src/server; mkdir -p cache ; cd ../..

#
#   testAppweb.h
#
$(CONFIG)/inc/testAppweb.h: $(DEPS_85)
	@echo '      [Copy] $(CONFIG)/inc/testAppweb.h'
	mkdir -p "$(CONFIG)/inc"
	cp test/src/testAppweb.h $(CONFIG)/inc/testAppweb.h

#
#   testAppweb.o
#
DEPS_86 += $(CONFIG)/inc/bit.h
DEPS_86 += $(CONFIG)/inc/testAppweb.h
DEPS_86 += $(CONFIG)/inc/mpr.h
DEPS_86 += $(CONFIG)/inc/http.h

$(CONFIG)/obj/testAppweb.o: \
    test/src/testAppweb.c $(DEPS_86)
	@echo '   [Compile] $(CONFIG)/obj/testAppweb.o'
	$(CC) -c -o $(CONFIG)/obj/testAppweb.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" test/src/testAppweb.c

#
#   testHttp.o
#
DEPS_87 += $(CONFIG)/inc/bit.h
DEPS_87 += $(CONFIG)/inc/testAppweb.h

$(CONFIG)/obj/testHttp.o: \
    test/src/testHttp.c $(DEPS_87)
	@echo '   [Compile] $(CONFIG)/obj/testHttp.o'
	$(CC) -c -o $(CONFIG)/obj/testHttp.o $(CFLAGS) $(DFLAGS) "-I$(CONFIG)/inc" "-I$(WIND_BASE)/target/h" "-I$(WIND_BASE)/target/h/wrn/coreip" test/src/testHttp.c

#
#   testAppweb
#
DEPS_88 += $(CONFIG)/inc/mpr.h
DEPS_88 += $(CONFIG)/inc/bit.h
DEPS_88 += $(CONFIG)/inc/bitos.h
DEPS_88 += $(CONFIG)/obj/mprLib.o
DEPS_88 += $(CONFIG)/bin/libmpr.out
DEPS_88 += $(CONFIG)/inc/pcre.h
DEPS_88 += $(CONFIG)/obj/pcre.o
ifeq ($(BIT_PACK_PCRE),1)
    DEPS_88 += $(CONFIG)/bin/libpcre.out
endif
DEPS_88 += $(CONFIG)/inc/http.h
DEPS_88 += $(CONFIG)/obj/httpLib.o
DEPS_88 += $(CONFIG)/bin/libhttp.out
DEPS_88 += $(CONFIG)/inc/appweb.h
DEPS_88 += $(CONFIG)/inc/customize.h
DEPS_88 += $(CONFIG)/obj/config.o
DEPS_88 += $(CONFIG)/obj/convenience.o
DEPS_88 += $(CONFIG)/obj/dirHandler.o
DEPS_88 += $(CONFIG)/obj/fileHandler.o
DEPS_88 += $(CONFIG)/obj/log.o
DEPS_88 += $(CONFIG)/obj/server.o
DEPS_88 += $(CONFIG)/bin/libappweb.out
DEPS_88 += $(CONFIG)/inc/testAppweb.h
DEPS_88 += $(CONFIG)/obj/testAppweb.o
DEPS_88 += $(CONFIG)/obj/testHttp.o

$(CONFIG)/bin/testAppweb.out: $(DEPS_88)
	@echo '      [Link] $(CONFIG)/bin/testAppweb.out'
	$(CC) -o $(CONFIG)/bin/testAppweb.out $(LDFLAGS) $(LIBPATHS) "$(CONFIG)/obj/testAppweb.o" "$(CONFIG)/obj/testHttp.o" $(LIBS) -Wl,-r 

ifeq ($(BIT_PACK_CGI),1)
#
#   test-testScript
#
DEPS_89 += $(CONFIG)/inc/bit.h
DEPS_89 += $(CONFIG)/obj/cgiProgram.o
DEPS_89 += $(CONFIG)/bin/cgiProgram.out

test/cgi-bin/testScript: $(DEPS_89)
	cd test; echo '#!../$(CONFIG)/bin/cgiProgram.out' >cgi-bin/testScript ; chmod +x cgi-bin/testScript ; cd ..
endif

ifeq ($(BIT_PACK_CGI),1)
#
#   test-cache.cgi
#
test/web/caching/cache.cgi: $(DEPS_90)
	cd test; echo "#!`type -p ejs`" >web/caching/cache.cgi ; cd ..
	cd test; echo 'print("HTTP/1.0 200 OK\nContent-Type: text/plain\n\n" + Date() + "\n")' >>web/caching/cache.cgi ; cd ..
	cd test; chmod +x web/caching/cache.cgi ; cd ..
endif

ifeq ($(BIT_PACK_CGI),1)
#
#   test-basic.cgi
#
test/web/auth/basic/basic.cgi: $(DEPS_91)
	cd test; echo "#!`type -p ejs`" >web/auth/basic/basic.cgi ; cd ..
	cd test; echo 'print("HTTP/1.0 200 OK\nContent-Type: text/plain\n\n" + serialize(App.env, {pretty: true}) + "\n")' >>web/auth/basic/basic.cgi ; cd ..
	cd test; chmod +x web/auth/basic/basic.cgi ; cd ..
endif

ifeq ($(BIT_PACK_CGI),1)
#
#   test-cgiProgram
#
DEPS_92 += $(CONFIG)/inc/bit.h
DEPS_92 += $(CONFIG)/obj/cgiProgram.o
DEPS_92 += $(CONFIG)/bin/cgiProgram.out

test/cgi-bin/cgiProgram.out: $(DEPS_92)
	cd test; cp ../$(CONFIG)/bin/cgiProgram.out cgi-bin/cgiProgram.out ; cd ..
	cd test; cp ../$(CONFIG)/bin/cgiProgram.out cgi-bin/nph-cgiProgram.out ; cd ..
	cd test; cp ../$(CONFIG)/bin/cgiProgram.out 'cgi-bin/cgi Program.out' ; cd ..
	cd test; cp ../$(CONFIG)/bin/cgiProgram.out web/cgiProgram.cgi ; cd ..
	cd test; chmod +x cgi-bin/* web/cgiProgram.cgi ; cd ..
endif

#
#   test.js
#
DEPS_93 += src/esp/esp-www/files/static/js

test/web/js: $(DEPS_93)
	@echo '      [Copy] test/web/js'
	mkdir -p "test/web/js"
	cp src/esp/esp-www/files/static/js/jquery.esp.js test/web/js/jquery.esp.js
	cp src/esp/esp-www/files/static/js/jquery.js test/web/js/jquery.js
	cp src/esp/esp-www/files/static/js/jquery.min.js test/web/js/jquery.min.js
	cp src/esp/esp-www/files/static/js/jquery.simplemodal.js test/web/js/jquery.simplemodal.js
	cp src/esp/esp-www/files/static/js/jquery.tablesorter.js test/web/js/jquery.tablesorter.js

#
#   installBinary
#
installBinary: $(DEPS_94)

#
#   install
#
DEPS_95 += compile
DEPS_95 += installBinary

install: $(DEPS_95)
	


#
#   uninstall
#
DEPS_96 += build
DEPS_96 += compile

uninstall: $(DEPS_96)
	rm -f "$(BIT_VAPP_PREFIX)/appweb.conf"
	rm -f "$(BIT_VAPP_PREFIX)/esp.conf"
	rm -f "$(BIT_VAPP_PREFIX)/mine.types"
	rm -f "$(BIT_VAPP_PREFIX)/install.conf"
	rm -fr "$(BIT_VAPP_PREFIX)/inc/appweb"

#
#   genslink
#
genslink: $(DEPS_97)
	cd src/server; esp --static --genlink slink.c --flat compile ; cd ../..


#
#   run
#
DEPS_98 += compile

run: $(DEPS_98)
	cd src/server; sudo ../../$(CONFIG)/bin/appweb -v ; cd ../..

