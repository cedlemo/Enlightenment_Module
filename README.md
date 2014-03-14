###Enligthenment module skeleton

This is the skeleton I use in order to create my E18 module. 

####Description:
*    hello_world: the classic example which displays the hello world string in your shelf.
*    skel: the basic skeleton diretory tree used by the script.
*    genmod.rb: a script that create all the needed files to start a new module.

####Usage:
*   use genmod.rb in order to create a new module:
```
genmod.rb --author your_name --mail your_mail new_proj
```
*   Modify the files for your needs.
*   Generate the module:
```
cd new_proj
./autogen.sh --enable-homedir-install
make install
```
*    activate the module in "configuration" > "control panel" > "extensions" then
  "modules" and add the module to the shelf.
