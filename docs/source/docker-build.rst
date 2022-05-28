
..
   Comment: include Global RST File for Includes
.. include:: global.rst

.. image:: http://i.imgur.com/AWyEuN3.jpg
   :alt: Screenshot

Docker Image Creation and Run Guide
===================================

.. list-table:: Latest Content Changes
   :widths: 25 75
   :header-rows: 1

   * - Date
     - Update Notes
   * - 05/21/2022
     - Initial Guide in .RST format

.. important:: Latest Tested in Windows 10 64 bit Docker with Linux Containers.

--------------------
Clone the project
--------------------

This will checkout and create project in an ``Oblivion2-XRM`` folder where you run the command.

::

  git clone https://github.com/M-griffin/Oblivion2-XRM.git

::

  cd Oblivion2-XRM

Next from the root project folder you can run the following docker commands.

--------------------
Docker
--------------------

.. Important:: This is a preview of the development work, It's not a fully functional system
  You can browse Menu's, and some initial sysop functions such as the User, Menu, Message and Level editors.
  With the Full Screen Message editor in the works.

.. Note:: Being this is a preview the default setup here is to build and run a preview, the ``container``
  or ``image`` will not save any new users or configuration changes made while adventuring around.
  Once the application is more ready for release then some updated documentation will be created for using
  volumes to run your container's from.

---------------
Docker-Compose
---------------
The following commands should be run from the root ``Oblivion2-XRM`` folder.

First time setup or on changes ``build`` is used to create or update the docker image.
recreating from any changes or updates when pulling down the branch.

Build and Startup Commands
::

  docker-compose build && docker-compose up


Startup Command - All other times to startup the container you only need to run:
::

  docker-compose up


Connecting to the BBS
---------------------

| ``Default Telnet Port``: 6023
| The **configuration** gets auto-generated once you start the system the first time.

| New User Password: ``newuser`` The first created user becomes a SysOp set to level 255
| All other users are normal access levels and can be adjusted in the | Sysop Menu | User Editor



Logging into the container
--------------------------

| Logging in replace the `<CONTAINER ID>` below with the actual id of the container that is running.
| use the ``docker ps`` to get a list of running containers.

::

  docker ps
  docker exec -it <CONTAINER ID> /bin/bash

You should now be in the logged in and in the following folder ``/usr/src/Oblivion2-XRM/build``
The root of the BBS executable folder.  This folder also contains core of the bbs application.
You'll find the main ``xrm-server.yaml`` configuration file with other data files in the ``data`` folder.


.. note:: The BBS is running by default.

**List of Executables**
::

  ./xrm-server              - to start the bbs
  ./xrm-unittest            - to run the unit tests

**Helper Applications** Only used if you are converting legacy items or mods.
::

  ./xrm-menu-convert        - convert Legacy Obv/2 menus to yaml
  ./xrm-menu-prompt-convert - convert Legacy Obv/2 prompts to yaml

.. note:: Converters only tested on default Menu's and prompt files. Should work for menu and prompt mods too.

