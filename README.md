block-walker
============

simple block-walker of mtf 


how to build
============
Ubuntu:

	mkdir test
	cd test
	cmake ..
	make
Windows (VS):

	mkdir test
	cd test
	cmake ..
then blockwalker.sln can you find, build it.


how to test
===========

Ubuntu:

	./blockwalker ../tester/Backup.bkf

Windows:
	find the blockwalker.exe in test\Debug or test\Release and CD to that directory, then run

	blockwalker.exe ..\..\tester\Backup.bkf

you can test other mtf files as you want. 


