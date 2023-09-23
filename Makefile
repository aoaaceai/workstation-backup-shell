SRCS=$(wildcard src/*.cc)

OBJS=$(SRCS:.cc=.o)

SSHD_CONFIGS=$(wildcard system_configs/sshd_config_*)

SYSTEMD_SERVICES=$(wildcard system_configs/*.service)

SSH_CONFIG_DIR=/etc/ssh/

SYSTEMD_SERVICE_DIR=/usr/lib/systemd/system/

RUNTIME_DIRS=$(addprefix /tmp2/,backup_list backup_locks backup_output)

all: $(OBJS)
	g++ $(OBJS) -o build/fsh

clean:
	rm src/*.o build/*

install:
	install -m 755 build/fsh /bin/
	cp $(SSHD_CONFIGS) $(SSH_CONFIG_DIR)
	cp $(SYSTEMD_SERVICES) $(SYSTEMD_SERVICE_DIR)
	mkdir -p $(RUNTIME_DIRS)
	chmod 1777 $(RUNTIME_DIRS)

uninstall:
	rm /bin/fsh
	rm $(addprefix $(SSH_CONFIG_DIR), $(notdir ${SSHD_CONFIGS}))
	rm $(addprefix $(SYSTEMD_SERVICE_DIR), $(notdir $(SYSTEMD_SERVICES)))