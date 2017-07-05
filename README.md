transball
====

![transball build status](https://api.travis-ci.org/ptitSeb/transball.png "transball build status")


Port of TransballGL to the OpenPandora (using glshim) - Status: WIP


To compile on the Pandora, using the Codeblocks Command Line PND, just type
`cd TGL && make`

To compile on the ODROID, type
`cd TGL && make ODROID=1`

To compile on x86 Linux, type
`cd TGL && make LINUX=1`

Other platform can probably be added easily (like RPi), but will need some slight 
changes in the Makefile (contact me or do a pull request if you want / have a new platform)
