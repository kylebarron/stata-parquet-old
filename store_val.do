* On Linux, compiled with:
* gcc -shared -fPIC -DSYSTEM=OPUNIX stplugin.c store_val.c -o store_val.plugin

* On Mac, compiled with:
* gcc -bundle -DSYSTEM=APPLEMAC stplugin.c store_val.c -o store_val.plugin

clear all

cap program drop store_val
program store_val

    program _store_val, plugin using("./store_val.plugin")
    set obs 1
    gen newvar = .

    plugin call _store_val newvar

end