CC = gcc

PLUGIN_NAME = FastTrack.so

#DEBUGFLAGS = -DDEBUG -DHEAVY_DEBUG
#DEBUGFLAGS = -DDEBUG
DEBUGFLAGS = 

FST_SOURCES = \
	fst_fasttrack.c \
	fst_download.c \
	fst_hash.c \
	fst_http.c \
	fst_meta.c \
	fst_node.c \
	fst_packet.c \
	fst_search.c \
	fst_session.c \
	fst_stats.c \
	fst_utils.c \
	md5.c

CRYPT_SOURCES = \
	crypt/fst_crypt.c \
	crypt/enc_type_1.c \
	crypt/enc_type_2.c \
	crypt/enc_type_20.c

SOURCES = $(FST_SOURCES) $(CRYPT_SOURCES)

FST_OBJS = $(FST_SOURCES:%.c=%.o)
CRYPT_OBJS = $(CRYPT_SOURCES:%.c=%.o)

OBJS = $(FST_OBJS) $(CRYPT_OBJS)

LIBGIFT_CFLAGS = $(shell pkg-config --cflags libgift)
LIBGIFT_LDFLAGS = $(shell pkg-config --libs libgift) -lgiftproto

CFLAGS += \
	$(LIBGIFT_CFLAGS) \
	$(DEBUGFLAGS) \
	-I. \
	-Icrypt \
	-Wall \
	-Wno-unused \
	-fomit-frame-pointer

LDFLAGS += \
	$(LIBGIFT_LDFLAGS) \
	-lm

GIFT_INSTALL_DIR=$(HOME)/.giFT
FST_INSTALL_DIR=$(GIFT_INSTALL_DIR)/FastTrack

all ... : SO_CFLAGS += -shared
mac ... : SO_CFLAGS += -dynamiclib -flat_namespace -undefined suppress

all mac: $(OBJS)
	@echo
	$(CC) $(SO_CFLAGS) -o $(PLUGIN_NAME) $(CFLAGS) $(LDFLAGS) $(OBJS)
	@echo
	@echo "*************************************************************"
	@echo Your giFT settings path is $(GIFT_INSTALL_DIR) and the plugin will be installed into $(FST_INSTALL_DIR).
	@echo "If this is correct you can run 'make install' now."
	@echo "If giFT is NOT installed in the above place you MUST change the path in the Makefile before running 'make install'"
	@echo "*************************************************************"

install:
	@echo "*************************************************************"
	@echo "Copying files..."
	@echo "*************************************************************"
	@echo
	@echo Creating dir $(FST_INSTALL_DIR) if it doesn\'t already exist
	@mkdir -p $(FST_INSTALL_DIR)
	@echo moving ./$(PLUGIN_NAME) to /usr/local/lib/giFT/$(PLUGIN_NAME)
	@mv ./$(PLUGIN_NAME) /usr/local/lib/giFT/$(PLUGIN_NAME)
#	@echo copying ./data/nodes to $(FST_INSTALL_DIR)/nodes
#	@cp ./data/nodes $(FST_INSTALL_DIR)/nodes
	@echo
	@echo "*************************************************************"
	@echo "Ok we're done."
	@echo "Start giFT with 'giFT -p /usr/local/lib/giFT/$(PLUGIN_NAME)' to give it a try."
	@echo "Alternatively you can add '/usr/local/lib/giFT/$(PLUGIN_NAME)' to gift.conf"
	@echo "*************************************************************"


clean:
	rm -f $(OBJS)
	rm -f $(PLUGIN_NAME)
