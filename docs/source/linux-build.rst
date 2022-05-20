
..
   Comment: include Global RST File for Includes
.. include:: global.rst

.. image:: http://i.imgur.com/AWyEuN3.jpg
   :alt: Screenshot

Linux (Ubuntu/Mint) Setup and Build Guide
=========================================

.. list-table:: Latest Content Changes
   :header-rows: 1

   * - Date
     - Update Notes
   * - 07/26/2019
     - Initial Write up with new command line build script.
   * - 05/20/2022
     - Updated guide to .RST format

.. important:: Latest Builds Tested on Mint 20.3 (Cinnamon 64-Bit) and Ubuntu 22.04 (LTS 64-Bit)

--------------------
Libraries to Install
--------------------

.. note:: Boost is only used for the menu and prompt conversion utilities. 
  Future plans are to remove this dependency.

:List of Dependencies:
  sqlite :raw-html:`<br />`
  yaml-cpp :raw-html:`<br />`
  openssl :raw-html:`<br />`
  libssh :raw-html:`<br />`
  ossp-uuid :raw-html:`<br />`
  boost :raw-html:`<br />`
  unittest-cpp :raw-html:`<br />`

**Install Packages**

::

  sudo apt-get install build-essential
  sudo apt-get install libyaml-cpp-dev
  sudo apt-get install libssh-dev
  sudo apt-get install libboost-all-dev
  sudo apt-get install libsqlite3-dev
  sudo apt-get install uuid-dev
  sudo apt-get install libunittest++-dev


Optional Dependencies depending on your Development Environment
  Install GIT to pull down project:

::

  apt-get install git

**Install latest compiler with c++11 compatibility.**
  Most systems within the last several years are recent enough to support C++11 Compatibility.  Any version above GCC 4.8.1.

.. note:: Only Need to install or upgrade if your compiler is ancient dirt. :)


**Clone the project - This will checkout and create project in an ``Oblivion2-XRM`` folder.**

::

  git clone https://github.com/M-griffin/Oblivion2-XRM.git

::

  cd Oblivion2-XRM
  cd build
  ./build-linux.sh -j2

-- note:: -j# is number of cpu cores for faster compiling

| ``Build Script`` compiles each part of the application and it's utilities and Unit Tests
| The executable programs are copied to the build folder for running.

::

  ./xrm-server              - to start the bbs
  ./xrm-unittest            - to run the unit tests

**Helper Applications** Only used if you are converting legacy items or mods.
::

  ./xrm-menu-convert        - convert Legacy Obv/2 menus to yaml
  ./xrm-menu-prompt-convert - convert Legacy Obv/2 prompts to yaml

.. note:: Only Tested on default Menu's and prompt files. Should work for menu and prompt mods too.

**Running the BBS**

| ``Default Telnet Port``: 6023

| Ports and General Setup can be updated in the ``xrm-server.yaml`` configuration file
| The **configuration** gets auto-generated once you start the system the first time.


All data files, and configuration file are generated on the initial startup of the system.
If you goof anything up, simply delete any of the ``.yaml`` file(s) you made a mistake and want to start over.
Than re-start the program and the files regenerate to defaults.

**Starting the BBS**
::

  ./xrm-server  -  the main configuration file generates in the root bbs folder.

I'm playing around with a yaml logging format, any feedback is welcome. =)
