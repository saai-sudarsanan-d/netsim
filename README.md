# Installing NS3

I would call this the toughest part in your NS3 Journey!!

* Download the NS3 Tarball from here : https://www.nsnam.org/releases/ns-3-35/
* Extract it to your home folder.

If you are on ubuntu 20 or its flavours run :

    sudo apt install build-essential autoconf automake libxmu-dev python-pygraphviz cvs mercurial bzr git cmake p7zip-full python-matplotlib python-tk python-dev python-kiwi python-gnome2 python-gnome2-desktop qt4-dev-tools qt4-qmake qt4-qmake qt4-default gnuplot-x11 wireshark   

Else visit : https://www.nsnam.org/releases/ns-3-35/ and install the prerequisite dependancies.

---
After you are sure that you have installed all the dependancies.

        cd ~/ns-allinone-3.35
        ./build.py
**This is a time taking process as NS3 has to compile all the source files. Once this is done take a copy of the ns3-allinone-3.35 folder for precautionary purposes.**

!["How the terminal looks on successful build"](/install/build-successful.png)

And you should be able to view this list!

!["Terminal Output that shows you what modules were built"](/install/modules-built.png)

Modules not built are not errors, there is not really any need for you to worry about it.

## Run Tests

    cd ns-3.35
    ./test.py

If you get an output saying waf in not configured to run tests or if no tests were run (testing process takes about half an hour, so if it takes abnormally lesser than that...)

    ./waf configure --enable-tests --enable-examples
    ./test.py

Now it should work. You will get this on a successful test run!

!["Terminal Output that shows you tests were run successfully"](/install/test-successful.png)

It is even better if those 3 weren't skipped.

You will also need `tracemetrics.jar` file for running ASCII Trace metrics.
 