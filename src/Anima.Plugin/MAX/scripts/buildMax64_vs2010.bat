cd obj/x64
del /Q *.*
cd ../..
NMAKE ./AnimaMax.mak MAX=2013 VER=64 WRK=../import/
cd obj/x64
del /Q *.*
cd ../..
NMAKE ./AnimaMax.mak MAX=2014 VER=64 WRK=../import/