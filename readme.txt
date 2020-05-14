this is a tetris brick game. it is copy from book,just fixxed some bugs and make it available on my ubuntu.
you need gtk lib before use below command to run the game.
you know ,I name the executeble file 'fangkuai'. so you know where I come from:)
just for GTK study, and most important ,have fun...

gcc -o fangkuai backcal.c display.c menu.c interface.c `pkg-config --cflags --libs gtk+-2.0`

