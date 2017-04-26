# README #

Name: agario_clone
Authors: Kajetan Śpionek, Wojciech Przybysz

### Set up environment ###

*Install Boost Library

1) download and unzip www.boost.org, then 'bootstrap.sh', then './b2'
2) copy the subdirectory 'boost' to '/usr/local/include', copy all the files from the 'stage/lib' to '/usr/local/lib' and put the line 'export LD_LIBRARY_PATH=/usr/local/lib' into /etc/bash.profile or ~/.bashrc

### Dependencies ###

Beast Websocket C++ library
https://github.com/vinniefalco/Beast
Added staticly to sources
