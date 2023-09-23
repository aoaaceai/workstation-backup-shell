FSH_SRCS=$(wildcard src/fsh/*.cc) $(wildcard src/common/*.cc)

FSH_OBJS=$(FSH_SRCS:.cc=.o)

PRIVLINK_SRCS=$(wildcard src/privlink/*.cc) $(wildcard src/common/*.cc)

PRIVLINK_OBJS=$(PRIVLINK_SRCS:.cc=.o)

SSHD_CONFIGS=$(wildcard system_configs/sshd_config_*)

SYSTEMD_SERVICES=$(wildcard system_configs/*.service)

SSH_CONFIG_DIR=/etc/ssh/

SYSTEMD_SERVICE_DIR=/usr/lib/systemd/system/

RUNTIME_DIRS=$(addprefix /tmp2/,backup_list backup_locks backup_output)

READONLY_DIRS=/tmp2_rootonly/

all: $(FSH_OBJS) $(PRIVLINK_OBJS)
	g++ $(FSH_OBJS) -o build/fsh
	g++ $(PRIVLINK_OBJS) -o build/privlink

clean:
	rm build/* || true
	find . -name '*.o' -exec rm -rf {} + || true

install:
	install -m 755 build/fsh /bin/
	install -m 4755 build/privlink /bin/
	cp $(SSHD_CONFIGS) $(SSH_CONFIG_DIR)
	cp $(SYSTEMD_SERVICES) $(SYSTEMD_SERVICE_DIR)
	mkdir -p $(RUNTIME_DIRS) $(READONLY_DIRS)
	chmod 1777 $(RUNTIME_DIRS)
	chmod 755 $(READONLY_DIRS)

uninstall:
	rm /bin/fsh
	rm /bin/privlink
	rm $(addprefix $(SSH_CONFIG_DIR), $(notdir ${SSHD_CONFIGS}))
	rm $(addprefix $(SYSTEMD_SERVICE_DIR), $(notdir $(SYSTEMD_SERVICES)))