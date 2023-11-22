#SUBDIRS = isa_sim
all:
	make -C ./src

clean:
	make -C ./src clean	