
.. include GLobal RST File for Includes
.. include:: global.rst

.. image:: http://i.imgur.com/AWyEuN3.jpg
   :alt: Screenshot

Oblivion/2 XRM - OS/X Setup and Build Guide
===========================================

.. list-table:: Latest Content Changes
   :widths: 25 75
   :header-rows: 1

   * - Date
     - Update Notes
   * - 09/16/2018
     - Initial Write up on Latest High Sierra distribution.
   * - 07/26/2019
     - Updated dependency paths and build scripts with documentation
   * - 05/08/2022
     - Updated dependencies that were missed also fixed Makefiles
   * - 05/17/2022
     - Updated guide to .RST format

.. important:: Latest Builds Tested on OS/X Catalina

--------------------
Libraries to Install
--------------------

.. note:: Boost is only used for the menu and prompt conversion utilities. 
  Future plans are to remove this dependency.

:List of Dependencies:
  /usr/local/opt/sqlite :raw-html:`<br />`
  /usr/local/opt/yaml-cpp :raw-html:`<br />`
  /usr/local/opt/openssl :raw-html:`<br />`
  /usr/local/opt/libssh :raw-html:`<br />`
  /usr/local/opt/ossp-uuid :raw-html:`<br />`
  /usr/local/opt/boost :raw-html:`<br />`
  /usr/local/opt/unittest-cpp :raw-html:`<br />`

.. note:: Some might not use Homebrew for their package management.
  However, I needed to setup specific Include file locations in the ".mk" Makefiles
  if you do NOT use Homebrew, then these Include paths would need to be updated manually.

.. Important:: Requirements, you should have XCode w/ Commandline Tools installed.
  If not, you can usually install this from the Apple Store.

**Install Homebrew:**

::

  ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

**Update Homebrew - And Possibly your OS to the latest version:**
::

  brew doctor
  brew update

Install Dependencies via Brew:
  All required libs are installed to **/usr/local/opt**

::

  brew install sqlite3
  brew install yaml-cpp
  brew install ossp-uuid
  brew install libssh
  brew install openssl@3
  brew install unittest-cpp
  brew install boost

Optional Dependencies depending on your Development Envrionment
  Install GIT to pull down project:

::

  brew install git

**Install latest compiler with c++11 compatibility as NEEDED!**
  You should have at least the following version installed or higher

::

  Apple LLVM version 9.0.0 (clang-900.0.39.2)
  Target: x86_64-apple-darwin17.4.0
  Thread model: Posix
  InstalledDir: /Library/Developer/CommandLineTools/usr/bin

.. note:: Only Need to install or upgrade if your compiler if the version is lower then this!

::

  brew install clang

| You can check check ``/usr/local/Cellar`` for program versions installed
| All Lib and Include files are linked to ``/usr/local/opt``


**Clone the project - This will checkout and create project in current folder.**

::

  git clone https://github.com/M-griffin/Oblivion2-XRM.git

::

  cd build
  ./build-osx.sh -j2  (Notes, -j# is number of cpu cores for faster compiling)

| ``Build Script`` compiles each part of the application and it's utilities and Unit Tests
| The executable programs are copied to the build folder for running.

::

  ./xrm-server              - to start the bbs
  ./xrm-unittest            - to run the unit tests

**Helper Applications** Only used if you are converting legacy items or mods.
::

  ./xrm-menu-convert        - convert Legacy Obv/2 menus to yaml format for XRM.
  ./xrm-menu-prompt-comvert - convert Legacy Obv/2 prompts to yaml format for XRM.

.. note:: Only Tested on default Menu's and prompt files. Should work for menu and prompt mods too.

**Running the BBS**

| ``Default Telnet Port``: 6023

| Ports and General Setup can be updated in the ``xrm-server.yaml`` configuration file
| The **configuration** gets auto-generated once you start the system the first time.


All data files, and configuration file are generated on the initial startup of the system.
If you goof anything up, simply delete any of the ``.yaml`` file(s) you made a mistake and want to start over.
Than re-start the program and the files regenerate to defaults.

Starting the BBS
::

  ./xrm-server  -  the main configuration file generates in the root bbs folder.

I'm playing around with a yaml logging format, any feedback is welcome. =)
