
CC = gcc

#DEBUGFLAGS = -DDEBUG -DHEAVY_DEBUG
#DEBUGFLAGS = -DDEBUG
DEBUGFLAGS = 

CFLAGS += -DHAVE_CONFIG_H $(DEBUGFLAGS) -I../src -I../lib -I../plugin -I. -I.. -Icrypt -Wall -Wno-unused -fomit-frame-pointer
LDFLAGS += -lm

GIFT_INSTALL_DIR=$(HOME)/.giFT
FST_INSTALL_DIR=$(GIFT_INSTALL_DIR)/FastTrack

FST_SOURCES = fst_fasttrack.c fst_download.c fst_hash.c fst_http.c fst_meta.c fst_node.c fst_packet.c fst_search.c fst_session.c fst_stats.c fst_utils.c md5.c
CRYPT_SOURCES = crypt/fst_crypt.c crypt/enc_type_1.c crypt/enc_type_2.c crypt/enc_type_20.c

FST_OBJS = fst_fasttrack.o fst_download.o fst_hash.o fst_http.o fst_meta.o fst_node.o fst_packet.o fst_search.o fst_session.o fst_stats.o fst_utils.o md5.o
CRYPT_OBJS = crypt/fst_crypt.o crypt/enc_type_1.o crypt/enc_type_2.o crypt/enc_type_20.o

all: $(FST_OBJS) $(CRYPT_OBJS)
	@echo
	$(CC) -shared -o FastTrack.so $(CFLAGS) $(LDFLAGS) $(DEBUGFLAGS) $(FST_OBJS) $(CRYPT_OBJS)
	@echo
	@echo "*************************************************************"
	@echo Your giFT settings path is $(GIFT_INSTALL_DIR) and the plugin will be installed into $(FST_INSTALL_DIR).
	@echo "If this is correct you can run 'make install' now."
	@echo "If giFT is NOT installed in the above place you MUST change the path in the Makefile before running 'make install'"
	@echo "*************************************************************"


mac: $(FST_OBJS) $(CRYPT_OBJS)
	@echo
	$(CC) -dynamiclib -flat_namespace -undefined suppress -o FastTrack.so  $(CFLAGS) $(LDFLAGS) $(DEBUGFLAGS) $(FST_OBJS) $(CRYPT_OBJS)
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
	@echo moving ./FastTrack.so to /usr/local/lib/giFT/FastTrack.so
	@mv ./FastTrack.so /usr/local/lib/giFT/FastTrack.so
#	@echo copying ./data/nodes to $(FST_INSTALL_DIR)/nodes
#	@cp ./data/nodes $(FST_INSTALL_DIR)/nodes
	@echo
	@echo "*************************************************************"
	@echo "Ok we're done."
	@echo "Start giFT with 'giFT -p /usr/local/lib/giFT/FastTrack.so' to give it a try."
	@echo "Alternatively you can add '/usr/local/lib/giFT/FastTrack.so' to gift.conf"
	@echo "*************************************************************"


clean:
	rm -f *.o
	rm -f crypt/*.o
