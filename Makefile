all:
	cd panel-minilogue;     make clean; make
	cd panel-nts1;          make clean; make
	cd panel-prologue;      make clean; make

clean:
	cd panel-minilogue;     make clean
	cd panel-nts1;          make clean
	cd panel-prologue;      make clean
