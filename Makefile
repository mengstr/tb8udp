.PHONY:	clean check docker

all: tb8udp

tb8udp: tb8udp.c
	$(CC) -Wall -Wextra -Werror=unused-result -Werror=uninitialized -Werror -O2 -o $@ $<

check: tb8udp

# Compile the code inside a docker container using this Makefile again
docker:	
	docker run --rm -v "$(PWD)":/usr/src/myapp -w /usr/src/myapp gcc:latest make

clean:
	rm -f tb8udp *.log
  
