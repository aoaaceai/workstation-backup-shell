SRCS=$(wildcard src/*.cc)

OBJS=$(SRCS:.cc=.o)

SSHD_CONFIGS=$(wildcard system_configs/sshd_config_*)

SYSTEMD_SERVICES=$(wildcard system_configs/*.service)

SSH_CONFIG_DIR=/etc/ssh/

SYSTEMD_SERVICE_DIR=/usr/lib/systemd/system/

all: $(OBJS)
	g++ $(OBJS) -o build/fsh

clean:
	rm src/*.o build/*

install:
	cp build/fsh /bin/
	cp $(SSHD_CONFIGS) $(SSH_CONFIG_DIR)
	cp $(SYSTEMD_SERVICES) $(SYSTEMD_SERVICE_DIR)

uninstall:
	rm /bin/fsh
	rm $(addprefix $(SSH_CONFIG_DIR), $(notdir ${SSHD_CONFIGS}))
	rm $(addprefix $(SYSTEMD_SERVICE_DIR), $(notdir $(SYSTEMD_SERVICES)))