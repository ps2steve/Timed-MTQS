#   For Multi-thread Hetergeneous Emulator Based on QEMU 2.5
##  The qemu board setup  
+ Check out the "start.sh" file for more information.
+ Qemu looks only support up-to 4 cores emulation in arm-based.

## Contact me
+ CY
+ ps2steve2005@hotmail.com

#   Git instruction memo

##  Command line instructions

### 1.  Git global setup
+   
  +  git config --global user.name "Your user name." 
  +  git config --global user.email "Your email."
+   
###     2.  Create a new repository

+   git clone https://git.greensocs.com/CY/MTHQ.git
+   cd MTHQ
+   touch README.md
+   git add README.md
+   git commit -m "add README"
+   git push -u origin master
+   Existing folder or Git repository
+   cd existing_folder
+   git init
+   git remote add origin https://git.greensocs.com/CY/MTHQ.git
+   
### 3. Only for master 
+   git add .
+   git commit
+   git push -u origin master
****
+   CY 2015 12 24